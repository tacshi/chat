/************************************************************
*  文件名：MyClient.c
*  描述：  客户端主界面
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "MyClientService.h"

void displayMenu();
void procMenu();
void exitProc();
void defProc();

int main(int agrc, char *argv[])
{
	system("clear");
	displayMenu();	
	procMenu();
	while(1)
	{
		sleep(1000);
	}
}

char ch;     //菜单选择
void displayMenu()
{
	printf("\t************************************************\n");
	printf("\t*                                              *\n");
	printf("\t*    南工程聊天客户端程序                      *\n");
	printf("\t*            V1.0                              *\n");
	printf("\t*                                              *\n");
	printf("\t*      1) 登录(L)                              *\n");
	printf("\t*      2) 退出(e)                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                            By:石棒           *\n");
	printf("\t************************************************\n");
  	printf("\n");
  	printf("\t您的选择是:");
  	ch = getchar();
}

void procMenu()
{
	switch(ch)
	{
	case 'l':
	case 'L':
	case '1':
		connectServer();
		break;

	case 'e':
	case 'E':
	case '2':
		exitProc();
		break;
	
	default:
		defProc();
	}
}

void exitProc()
{
	printf("\t客户端程序已经终止！\n");
	exit(0);
}

void defProc()
{
	printf("\t 输入字符错误！\n");
}
