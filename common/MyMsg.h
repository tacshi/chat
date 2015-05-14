/************************************************************
*  文件名：MyMsg.h
*  描述：  共用的头文件，消息定义
*************************************************************/
#ifndef _MYMSG_H_
#define _MYMSG_H_

enum 
{
	LOGIN_MSG,           //登录
	LOGIN_MSG_RES,       //登录响应
	LOGOUT_MSG,			//好友下线
	LOGOUT_MSG_RES,		//好友下线的响应
	QUERY_FQQ,           //查询单个好友QQ
	QUERY_FQQ_RES,       //查询单个好友QQ的响应
	QUERY_FQQLIST,		//获取QQ好友列表消息
	QUERY_FQQLIST_RES,	//获取QQ好友列表的响应消息
	TALK_MSG             //聊天
};

struct LoginMsg			//登录消息
{
	unsigned char id;  //消息编号
	char qq[6];        //登录ID号
};

struct LoginMsgResponse		//登录响应消息
{
	unsigned char id;		//消息编号
	unsigned char isOK;		//是否登录成功
	char reason[100];		//如果失败，则在此写入失败原因
};

struct LogoutMsg
{
	unsigned char id;
	char qq[6];
	char fqq[6];
};

struct LogoutMsgResponse
{
	unsigned char id;
	unsigned char isOK;
	char qq[6];
};

struct FriendQqMsg		//好友QQ
{
	unsigned char id;  //消息编号
	char qq[6];
};

struct FriendQqMsgResponse
{
	unsigned char id;  //消息编号
	unsigned char isOK;		//好友是否在线
};

struct FriendQqListMsg	//获取好友列表消息
{
	unsigned char id;
	char qq[6];			//这个是自己的QQ,目的是在服务器查找好友时除去自己
};

struct FriendQq
{
	char qq[6];
};

struct FriendQqListMsgResponse	//好友列表消息
{
	unsigned char id;
	int len;
	struct FriendQq qqList[10];
};

struct TalkMsg
{
	unsigned char id;  //消息编号
	char qq[6];
	char fqq[6];
	char info[200];
};

#endif
