/************************************************************
*  文件名：MyService.c
*  描述：  服务器业务实现
*************************************************************/
#include <stdio.h>
#include <string.h>
#include "MyService.h"
#include "../common/MyTCP.h"
#include "../common/MyTools.h"

pthread_t threadId;
struct ClientInfo cinfo[10];  //客户端信息记录区

/** 启动服务器 **/
void startServer()
{
	initSock(1); //服务器
    
    //创建一个线程专用于接受客户端的连接    
    pthread_create(&threadId, NULL, myAcceptThread,NULL);
	return;
}

void *myAcceptThread(void *args)
{
	printMsg_S("开始myAcceptThread线程......\n");
	while(1)
	{
		int cs = tcpAccept();
		//当有一个客户端连接上以后,为其分配一个空间
		long int ind = findIdelCinfo();		//空闲的客户端
		if(ind == -1)
		{//服务器溢出
			continue;
		}
		cinfo[ind].isFree = 1; //已经占用标志
		cinfo[ind].sock = cs;	//将已接的收客户绑定到空闲的客户端结构体,并赋予它已接收的套接字
	    //对每一个连接创建一个线程
	    pthread_create(&cinfo[ind].threadId, NULL, myRecvThread,(void *)ind);	
		
	}
	
	printMsg_S("退出Accept线程.........\n");
	return NULL;
}

/** 在客户端信息记录区查找空闲的元素下标 **/
int findIdelCinfo()
{
	int i;
	for(i=0;i<sizeof(cinfo)/sizeof(struct ClientInfo); i++)
	{
		if(cinfo[i].isFree == 0)
		{
			return i;
		}
	}
	return -1;
}

//根据传入的套接字接收数据
void *myRecvThread(void *args)
{
	int ind = (long)args;	
	struct ClientInfo * pcinfo = &cinfo[ind];
	char buf[MAXSIZE] = {0};
	
	sprintf(buf,"开始myRecvThread线程:%d.........\n",(int)pcinfo->threadId);
	printMsg_S(buf);

	while(1)
	{
		memset(buf, 0, sizeof(buf));	
		int len = tcpRecv(pcinfo->sock, buf, sizeof(buf));	//接收客户端发来的消息
		if(len==0) break;
		
		unsigned char id = (unsigned char)buf[0];
		switch(id)
		{
		case LOGIN_MSG:	//服务器接收到客服端发来的登录消息后将其保存到服务器缓冲区,并向客户端返回登录响应消息,
			procLoginMsg(pcinfo,(struct LoginMsg *)buf);	//保存登录的QQ号并向客户端发送登录成功的响应消息
			break;
			
		case LOGOUT_MSG:
			procLogoutMsg(pcinfo,(struct LogoutMsg *)buf);
			break;

		case QUERY_FQQ:
			procFriendQqMsg(pcinfo,(struct FriendQqMsg *)buf);	//查找并判断指定QQ号的好友是否在线,并向客户端发送好友在线状态消息
			break;
			
		case QUERY_FQQLIST:
			procFriendQqListMsg(pcinfo,(struct FriendQqListMsg *)buf);
			break;

		case TALK_MSG:
			procTalkMsg((struct TalkMsg *)buf);
			break;
		}	
	}
	
	pcinfo->isFree = 0;
	sprintf(buf,"结束myRecvThread线程:%d.........\n",(int)pcinfo->threadId);
	printMsg_S(buf);
}

void procLoginMsg(struct ClientInfo * pcinfo, struct LoginMsg *msg)
{
	strcpy(pcinfo->qq, msg->qq);  //保存qq到服务器缓冲区
	
	//发送登录成功消息
	struct LoginMsgResponse res;
	res.id = LOGIN_MSG_RES;
	res.isOK = 1;		//表示客户端登录成功
	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
}

void procLogoutMsg(struct ClientInfo * pcinfo, struct LogoutMsg *msg)
{
	int i;
	struct LogoutMsgResponse res;
	bzero(&res, sizeof(res));
	res.id = LOGOUT_MSG_RES;
	res.isOK = 1;
	strcpy(res.qq, msg->qq);
	//给下线者发送
	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
	//给另一个人发送
	for(i=0; i < sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if(cinfo[i].isFree == 0 ) continue;
		if(strcmp(cinfo[i].qq, msg->fqq) == 0)
		{
			//将下线消息转发给好友
			tcpSend(cinfo[i].sock, (const char *)&res, sizeof(res));
		}
	}
}

void procFriendQqMsg(struct ClientInfo * pcinfo, struct FriendQqMsg *msg)
{
	int i;
	
	struct FriendQqMsgResponse res;
	res.id = QUERY_FQQ_RES;
	res.isOK = 0;  //没有找到
        
	for(i=0;i<sizeof(cinfo)/sizeof(struct ClientInfo); i++)
	{
		if(cinfo[i].isFree == 0 ) continue;
		if(strcmp(cinfo[i].qq,msg->qq) == 0)
		{
			res.isOK = 1; //好友在线
		}
	}
	//发送好友QQ查找结果消息
	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
}

void procFriendQqListMsg(struct ClientInfo * pcinfo, struct FriendQqListMsg *msg)
{
	int i;
	struct FriendQqListMsgResponse res;
	res.id = QUERY_FQQLIST_RES;
	res.len = 0;
	
	for(i=0;i<sizeof(cinfo)/sizeof(struct ClientInfo); i++)
	{
		if(cinfo[i].isFree == 0 || !strcmp(cinfo[i].qq, msg->qq)) continue;	//好友列表里面应该不包含自己
		strcpy(res.qqList[res.len].qq,cinfo[i].qq);
		res.len += 1;
	}
	//发送好友QQ查找结果消息
	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
}

void procTalkMsg(struct TalkMsg *pmsg)
{
	int i;
	int n = sizeof(cinfo) / sizeof(struct ClientInfo);
	for(i = 0; i < n; i++)
	{
		if(cinfo[i].isFree == 0 ) continue;
		if(strcmp(cinfo[i].qq, pmsg->fqq) == 0)
		{
			//将聊天消息转发给好友
			tcpSend(cinfo[i].sock, (const char *)pmsg, sizeof(*pmsg));
		}
	}
	return;
}

