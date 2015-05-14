/************************************************************
*  文件名：MyTools.c
*  描述：  共用的文件，工具函数定义
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "MyTools.h"

void printMsgExit_S(char *msg)
{
	printf("【南工服务器】error:%s", msg);
	fflush(stdout);
	exit(1);
}

void printMsgExit(char *msg)
{
	printf("【南工客户端】error:%s", msg);
	fflush(stdout); 
	exit(1);
}

void printMsg_S(const char *msg)
{
	printf("【南工服务器】%s", msg);
	fflush(stdout);
}

void printMsg(const char *msg)
{
	printf("【南工客户端】%s", msg);
	fflush(stdout);
}

void printInt_S(int value)
{
	printf("【南工服务器】%d\n", value);
}
