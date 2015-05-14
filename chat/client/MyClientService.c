/************************************************************
*  文件名：MyClientService.c
*  描述：  客户端业务实现
*************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>    //多线程
#include "MyClientService.h"
#include "../common/MyTCP.h"
#include "../common/MyTools.h"

//#define _DEBUG

#define IP "127.0.0.1"
#define PORT 5001

char qq[6] = {0};
char fqq[6] = {0};
pthread_t threadId;
int sock;

/** 客户端状态 **/
enum
{
	STUS_START,
	STUS_CONNECTED,
	STUS_WAIT_LOGIN_RES,
	STUS_LOGINED,
	STUS_LOGOUTED,
	STUS_WAIT_LOGOUT_RES,
	STUS_WAIT_QUERY_FQQ_RES,
	STUS_WAIT_QUERY_FQQLIST_RES,
	STUS_TALK
};

int status = STUS_START;

void connectServer()
{
	printMsg("开始连接服务器......\n");
	initSock(0);  //客户端
	sock = tcpConnect(IP,PORT);
	printMsg("连接服务器成功！\n");

	//创建一个线程用于接收服务器发送的信息
	pthread_create(&threadId, NULL, myRecvThread,NULL);
	//进入主处理程序,进行消息循环
	mainProc();
}
//********************************************************************************************************
/*根据传入的套接字接收数据**
**接收线程无限循环,通过套接字从服务器消息队列中取出消息,**
**根据消息ID进行相应的消息相应,**
**在消息相应中改变当前客户端状态*/
void *myRecvThread(void *args)
{
	char buf[MAXSIZE] = {0};
	printMsg("开始myRecvThread线程.........\n");
	status = STUS_CONNECTED;	//客户端链接成功后改变状态为STUS_CONNECTED,在主线程中发送登录消息告知服务器,
	while(1)					//并将状态改变为STUS_WAIT_LOGIN_RES等待服务器回复
	{
		memset(buf, 0, sizeof(buf));	
		tcpRecv(sock, buf, sizeof(buf));	//接收线程从服务器接收消息
		unsigned char id = (unsigned char)buf[0];
		switch(id)
		{
		case LOGIN_MSG_RES:		/*客户端收到服务器发来的登录响应消息后通过msg->isOK标志判断是否登录成功,
								**登录成功则设置状态为STUS_LOGINED,由主线程列出客户菜单,至此,登录过程结束*/
			procLoginMsgResponse((struct LoginMsgResponse *)buf);	//提示登录成功或失败
			break;
			
		case LOGOUT_MSG_RES:
			procLogoutMsgResponse((struct LogoutMsgResponse *)buf);
			break;

		case QUERY_FQQ_RES:
			procFriendQqMsgResponse((struct FriendQqMsgResponse *)buf);	//提示好友是否在线
			break;
			
		case QUERY_FQQLIST_RES:
			procFriendQqListMsgResponse((struct FriendQqListMsgResponse *)buf);
			break;

		case TALK_MSG:
			procTalkMsg((struct TalkMsg *)buf);		//显示接收到的聊天消息
			break;
		default:
			break;
		}
	}
	sprintf(buf,"结束myRecvThread线程.........\n");
	printMsg(buf);
	exit(0);
}

void procLoginMsgResponse(struct LoginMsgResponse *msg)
{
	if(msg->isOK==1)
	{
		printMsg("登录成功！\n");
		status = STUS_LOGINED;//已登录状态
	}
	else
	{
		printMsg("登录失败！\n");
		printMsgExit(msg->reason);
		printf("\n");
	}
}

void procLogoutMsgResponse(struct LogoutMsgResponse *msg)
{
	if(strcmp(msg->qq,qq) == 0)
	{
		if(msg->isOK == 1)
		{
			printMsg("下线成功!\n");
			exit(0);
		}
		else
		{
			printMsg("下线失败!\n");
		}
	}
	else if(strcmp(msg->qq, fqq) == 0)
	{
		char tmp[25];
		printf("\n");
		sprintf(tmp, "系统提示: 对方[QQ号:%s]已下线!", fqq);
		printMsg(tmp);
	}
	status = STUS_LOGINED;		//被通知客户端的状态没有改变,因为它的状态此时还是STUS_TALK
	#ifdef _DEBUG
	printf("procLogoutMsgResponse--->status = %d", status);
	#endif
}

void procFriendQqMsgResponse(struct FriendQqMsgResponse *msg)
{
	if(msg->isOK==1)
	{
		printMsg("好友目前在线，可以开始聊天...\n");
		status = STUS_TALK;
	}
	else
	{
		printMsg("好友不存在啊，再换个人试试！\n");
		status = STUS_LOGINED;
	}
}

void procFriendQqListMsgResponse(struct FriendQqListMsgResponse *msg)
{
	int i;
	char str[30];
	sprintf(str, "共有 %d 个用户在线:\n", msg->len);
	printMsg(str);
	for(i = 0; i < msg->len; i++)
	{
		printf("\t\t第%d个用户: %s\n", i+1, msg->qqList[i].qq);
	}
	status = STUS_LOGINED;
}

void procTalkMsg(struct TalkMsg *msg)
{
	char buf[MAXSIZE] = {0};
	
	if(strcmp(msg->qq, fqq)==0)
	{//表示是好友发起的聊天
		printf("\n");
		sprintf(buf,"[好友] %s 说>> %s",fqq,msg->info);
		printMsg(buf);
		
	}
	else
	{//是陌生人发起的聊天
		printf("\n");
		sprintf(buf,"[陌生人] %s 说>> %s",msg->qq,msg->info);
		printMsg(buf);
		printf("\n\t[提示] 加对方为好友后才能与其聊天(回车继续)");
		fflush(stdout);
		/*char c = getchar();
		while(c != '\n')
			c = getchar();*/
	}
}

//**********************************************************************************************************
void mainProc()
{
	while(1)
	{
		switch(status)
		{
		case STUS_CONNECTED:
			statusConnected();
			break;

		case STUS_WAIT_LOGIN_RES:
			printMsg("等待服务器响应.....\n");
			sleep(1);
			break;

		case STUS_LOGINED:
			statusLogined();
			break;
			
		case STUS_WAIT_LOGOUT_RES:
			printMsg("正在下线...\n");
			sleep(1);
			break;
		
		case STUS_WAIT_QUERY_FQQ_RES:
			printMsg("等待服务器响应......\n");
			sleep(1);
			break;
			
		case STUS_WAIT_QUERY_FQQLIST_RES:
			printMsg("等待服务器响应......\n");
			sleep(1);
			break;
		
		case STUS_TALK:
			statusTalk();
			break;
		default:
			break;
		}
	}
}

void statusConnected()
{	
	printMsg("请输入登录信息...\n");
	while(1)
	{
		printMsg("本机的QQ号(最多不超过5位)：");
		char tmp[MAXSIZE];
		scanf("%s", tmp);
		if(strlen(tmp)>5)
		{
			printMsg("您输入的qq号长度大于5个，请重新输入\n");
		}
		else
		{
			strcpy(qq, tmp);
			break;
		}
	}
	//登录成功后,把登录消息发送给服务器
	struct LoginMsg msg;
	msg.id = LOGIN_MSG;
	strcpy(msg.qq, qq);
	tcpSend(sock, (const char *)&msg, sizeof(msg));
	status = STUS_WAIT_LOGIN_RES;
	
	return;
}

void statusLogined()
{
	printMsg("显示在线好友(L) | 找好友聊天(T) | 退出(E): ");
	setbuf(stdin, NULL);   //清空键盘缓冲区
	int c = getchar();
	if(c == 'T' || c == 't')
	{
		printMsg("选择聊天好友......\n");
		while(1)
		{
			printMsg("输入聊天好友QQ号(最多不超过5位)：");
			char tmp[MAXSIZE];
			scanf("%s", tmp);
			if(strlen(tmp)>5)
			{
				printMsg("您输入的qq号长度大于5个，请重新输入\n");
			}
			else if( strcmp(tmp,qq) == 0)
			{
				printMsg("别闹,你输入的是自己的QQ号,请重新输入\n");				
			}
			else
			{	//将好友QQ号消息传入服务器套接字
				struct FriendQqMsg msg;
				msg.id = QUERY_FQQ;
				strcpy(msg.qq,tmp);
				strcpy(fqq, tmp);		//用于传入procTalkMsg(struct TalkMsg *msg)的参数
				tcpSend(sock,(const char *)&msg, sizeof(msg));	//将好友QQ号发送给服务器套接字
				status = STUS_WAIT_QUERY_FQQ_RES;
				break;
			}
		}		
	}
	else if(c == 'L' || c == 'l')
	{
		c = getchar();
		struct FriendQqListMsg msg;
		msg.id = QUERY_FQQLIST;
		strcpy(msg.qq, qq);
		tcpSend(sock,(const char *)&msg,sizeof(msg));
		status = STUS_WAIT_QUERY_FQQLIST_RES;
	}
	else if(c=='E' || c=='e')
	{
		printMsg("客户端已经退出！\n");
		exit(0);
	}
	else
	{
		while(c != '\n')
			c = getchar();
		status = STUS_LOGINED;
	}
}

void statusTalk()
{
	printMsg("我说>>");
	setbuf(stdin, NULL);
	char buf[MAXSIZE] = {0};
	scanf("%s", buf);
	if(strcmp(buf, "exit")==0)
	{
		struct LogoutMsg msg;
		msg.id = LOGOUT_MSG;
		strcpy(msg.qq, qq);
		strcpy(msg.fqq, fqq);
		tcpSend(sock, (const char *)&msg, sizeof(msg));
		status = STUS_WAIT_LOGOUT_RES;
	}
	else
	{
		struct TalkMsg msg;
		msg.id = TALK_MSG;
		strcpy(msg.qq, qq);
		strcpy(msg.fqq, fqq);
		strcpy(msg.info, buf);
		tcpSend(sock, (const char *)&msg, sizeof(msg));
		if(status == STUS_TALK)
			status == STUS_TALK;
		#ifdef _DEBUG
		printf("statusTALK--->status: %d\n", status);
		#endif
		
	}
	return;
}

