/************************************************************
*  文件名：MyClientService.h
*  描述：  头文件，客户端业务层函数声明
*************************************************************/
#ifndef _MYCLIENTSERVICE_H_
#define _MYCLIENTSERVICE_H_
#include "../common/MyMsg.h"
void connectServer();

void mainProc();
//消息接收线程
void *myRecvThread(void *args);
//消息响应处理函数,处理后对状态标志进行设置
void procLoginMsgResponse(struct LoginMsgResponse *msg);
void procLogoutMsgResponse(struct LogoutMsgResponse *msg);
void procFriendQqMsgResponse(struct FriendQqMsgResponse *msg);
void procFriendQqListMsgResponse(struct FriendQqListMsgResponse *msg);
void procTalkMsg(struct TalkMsg *msg);

/** 状态处理函数 **/
void statusConnected();
void statusLogined();
void statusTalk();

#endif
