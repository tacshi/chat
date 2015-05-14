/************************************************************
*  文件名：MyService.h
*  描述：  头文件，服务器业务层函数申明等
*************************************************************/
#ifndef _MYSERVICE_H_
#define _MYSERVICE_H_
#include <pthread.h>    //多线程
#include "../common/MyMsg.h"

#define SERVER

struct ClientInfo
{
	char qq[6];           //客户端ID号
	int sock;             //客户端已经建立连接的套接字
	unsigned char isFree; //空闲标志
	pthread_t threadId;   //为处理该客户端建立的接收线程ID
};

void startServer();
void *myAcceptThread(void *args);
void *myRecvThread(void *args);
void procLoginMsg(struct ClientInfo * pcinfo, struct LoginMsg *msg);
void procLogoutMsg(struct ClientInfo * pcinfo,struct LogoutMsg *msg);
void procFriendQqMsg(struct ClientInfo * pcinfo, struct FriendQqMsg *msg);
void procFriendQqListMsg(struct ClientInfo * pcinfo, struct FriendQqListMsg *msg);
void procTalkMsg(struct TalkMsg *msg);
int findIdelCinfo();

#endif
