/************************************************************
*  文件名：MyServer.c
*  描述：  用户操作界面模块
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "MyService.h"

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
	return 0;
}

char ch;     //菜单选择
/*  系统菜单  */
void displayMenu()
{
    printf("\t************************************************\n");
    printf("\t*                                              *\n");
    printf("\t*    南工程智能聊天服务器                      *\n");
    printf("\t*         V1.0                                 *\n");
    printf("\t*                                              *\n");
    printf("\t*      1) 启动 (s)                             *\n");
    printf("\t*      2) 退出 (e)                             *\n");
    printf("\t*                                              *\n");
    printf("\t*                                              *\n");
    printf("\t*                             By:石棒          *\n");
    printf("\t************************************************\n");
    printf("\n");
    printf("\t您的选择是:");
    ch = getchar();
    while(ch == '\n')
    {
    	ch = getchar();
    }
}

void procMenu()
{
    switch(ch)
    {
    case 's':
    case 'S':
    case '1':
        startServer();
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
    printf("\t系统已经退出！\n");
    exit(0);
}

void defProc()
{
    printf("\t输入字符错误！\n");
}
