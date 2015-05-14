/************************************************************
*  文件名：MyTCP.h
*  描述：  共用的文件，TCP通信头文件
*************************************************************/
#ifndef _MYTCP_H_
#define _MYTCP_H_

int initSock(int IsServer);
int tcpConnect(const char *serverIP, unsigned short port );
int tcpAccept();
int tcpSend(int sock, const char *sendBuf, int sendBufLen );
int tcpRecv(int sock, char *recvBuf, int recvBufLen );
 
#endif
