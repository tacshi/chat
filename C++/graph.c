#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Status LocateVex(ALGraph *g,VertexType ch)
{
	int i;
    for(i = 0; i < MAXVEX; i++)
    {
        if(ch == g->adjList[i].data)
        {
            break;
        }
    }
    if(i >= MAXVEX)
    {
        fprintf(stderr,"there is no vertex.\n");
        return -1;
    }
    return i;
}

void CreateALGraph(ALGraph *g)
{
	int i, k, m, n;
	char p, q;
    ArcNode *e;
    ArcNode *f;
    printf("输入顶点数和边数:");
    scanf("%d%d", &g->numVertexes, &g->numArcs);
    for(i = 0; i < g->numVertexes; i++)
    {
        printf("请输入顶点%d:", i);
        g->adjList[i].data = getchar();          //输入顶点信息
        while(g->adjList[i].data == '\n')
        {
            g->adjList[i].data = getchar();
        }
		g->adjList[i].firstedge = NULL;          //将边表置为空表
    }
    //建立边表
    for(k = 0; k < g->numArcs; k++)
    {
        printf("输入边(vi,vj)上的顶点序号:");
//      char p, q;
        p = getchar();
        while(p == '\n')
        {
            p = getchar();
        }
        q = getchar();
        while(q == '\n')
        {
            q = getchar();
        }
//		int m, n;
        m = LocateVex(g, p);
        n = LocateVex(g, q);
        if(m == -1 || n == -1)
        {
            return;
        }
        //向内存申请空间，生成边表结点
        e = (ArcNode *)malloc(sizeof(ArcNode));
        if(e == NULL)
        {
            fprintf(stderr, "malloc() error.\n");
            return;
        }
		//邻接序号为n
        e->adjvex = n;
        //将e指针指向当前顶点指向的结构
        e->next = g->adjList[m].firstedge;
        //将当前顶点的指针指向e
        g->adjList[m].firstedge = e;
        f = (ArcNode *)malloc(sizeof(ArcNode));
        if(f == NULL)
        {
            fprintf(stderr, "malloc() error.\n");
            return;
        }
		//邻接序号为m
        f->adjvex = m;
        f->next = g->adjList[n].firstedge;
        g->adjList[n].firstedge = f;
    }
}

void DFS(ALGraph g,int i)
{
    ArcNode *p;
    visited[i] = TRUE;
    printf("%c ", g.adjList[i].data);   //打印顶点，也可以其他操作
    p = g.adjList[i].firstedge;
    while(p)
    {
        if(!visited[p->adjvex])
        {
            DFS(g, p->adjvex);           //对访问的邻接顶点递归调用
        }
        p = p->next;
    }
}

void DFSTraverse(ALGraph g)
{
	int i;
    for(i = 0; i < g.numVertexes; i++)
    {
        visited[i] = FALSE;
    }
    for(i = 0; i < g.numVertexes; i++)
    {
		printf("顶点%d的度数为%d\t",i,GDegree(g,i));
        if(!visited[i])
        {
            DFS(g, i);
        }
    }
}

Status GDegree(ALGraph g,int n)
{
	int i;
	int m=0;
	ArcNode an;
	for(i=0;i<g.numVertexes;++i)
	{
		an=*g.adjList[i].firstedge;
		while(an.adjvex)
		{
			m++;
			an=*an.next;
		}
	}
	return m;
}