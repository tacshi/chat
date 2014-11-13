#include<stdio.h>
#define hashlen 11
#define p 11

int hash_create(int h[],int status[],int data)/*哈希造表*/
{
	int address;
	int di;
	address=data%p;
	if(status[address]==0)
	{
		h[address]=data;
		status[address]=1;
	}
	else
	{
		// 冲突，线性探测再散列
		for(di=1;di<=hashlen-1;di++)
		{
			address=((data%p)+di)%hashlen;
			if(status[address]==0)
			{
				h[address]=data;
				status[address]=1;
				break;
			}
		}
	}
	return 1;
}

int hash_search(int h[],int status[],int data)
{
	int address;
	int di;
	int num;
	address=data%p;
	if(h[address]==data)
		num=1;
	else
	{
		for(di=1,num=1;di<=hashlen-1;di++)
		{
			address=((data%p)+di)%hashlen;
			if(h[address]!=data)
				num++;
			else
			{
				num++;
				break;
			}
		}
	}
	return num;
}

void main()
{
	int h[hashlen]={0};
	int status[hashlen]={0};
	int count=0;
	int data[9]={20,30,70,15,8,12,18,63,19};
	int i;
	for(i=0;i<9;i++)
		hash_create(h,status,data[i]);
	printf("哈希表输出：\n");
	for(i=0;i<hashlen;i++)
		printf("%5d",i);
	printf("\n");
	for(i=0;i<hashlen;i++)
		printf("%5d",h[i]);
	printf("\n");
	for(i=0;i<9;i++)
		count+=hash_search(h,status,data[i]);
	printf("查找次数：%d\n",count);
	//count=count/9;
	printf("平均查找长度：%f\n",(float)count/9);
}
