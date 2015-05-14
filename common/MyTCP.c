/************************************************************

*  描述：  共用的文件，TCP通信
*************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "MyTools.h"

#define PORT 5001
int ls;   //侦听套接字

/*   描述: 初始化服务器或客户端, 该函数在使用中必需首先调用,客户端和服务器 */
int initSock(int IsServer)		//IsServer为1表示服务器,为0表示客户端
{
   ls = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if ( ls < 0)
   {
   		if(IsServer != 0)
   			printMsgExit_S("创建套接字失败!");
   		else
   			printMsgExit("创建套接字失败!");
   }

   if ( IsServer != 0)
   {
   		//服务器
       	struct sockaddr_in servAddr;

       	memset(&servAddr, 0, sizeof (servAddr));
		servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl (INADDR_ANY);
		servAddr.sin_port = htons(PORT);

       	if ( bind (ls, (struct sockaddr *)&servAddr, sizeof(servAddr))<0)
  		{
	   		printMsgExit_S("bind套接字失败!\n");
		}

       	if ( listen (ls, 10)<0)
  		{
	   		printMsgExit_S("listen套接字失败!\n");
		}
   }

   return 0;
}

/*   描述: 仅用于客户端, 连接服务器 */
int tcpConnect(const char *serverIP, unsigned short port )
{
   	struct sockaddr_in servAddr;
   	memset ( &servAddr, 0, sizeof (servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIP, &servAddr.sin_addr.s_addr);
	servAddr.sin_port = htons(port);
    
   	if ( connect (ls, (struct sockaddr *)&servAddr, sizeof(servAddr))<0)
	{
		printMsgExit("连接服务器失败！\n");
	}
	
	return ls;
}

/*  描述: 仅用于服务器, 等待客户端连接 */
int tcpAccept()
{
	struct sockaddr_in cliAddr;
 	socklen_t cliAddrLen = sizeof(cliAddr);

    int s = accept(ls, (struct sockaddr *)&cliAddr, &cliAddrLen);
    if(s < 0)
    {
    	printMsgExit_S("服务器Accept异常！");
    }
	
	char ip[20] = {0};
	char tmp[40] = {0};
	inet_ntop(AF_INET, &cliAddr.sin_addr.s_addr, ip, sizeof(ip));
	sprintf(tmp,"客户端进入,IP为:%s\n", ip);
	printMsg_S(tmp);
	printInt_S(s);
	return s;
}

/* 描述: 发送数据 */
int tcpSend (int sock, const char *sendBuf, int sendBufLen )
{
	int len = send (sock, sendBuf, sendBufLen, 0);
    return len;
}

/* 描述: 接收数据 */
int tcpRecv (int sock, char *recvBuf, int recvBufLen )
{
	int len = recv (sock, recvBuf, recvBufLen, 0);
    return len;
}
