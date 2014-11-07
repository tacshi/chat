#ifndef GRAPH_H_
#define GRAPH_H_
#define MAXVEX 100			//最大顶点数
#define TRUE 1
#define FALSE 0
typedef int Status;
typedef char VertexType;	//顶点类型应由用户定义
typedef int Boolean;            //Boolean 是布尔类型，其值是TRUE 或FALSE
Boolean visited[MAXVEX];        //访问标志数组

typedef struct ArcNode		//边表结点
{
    int adjvex;				//邻接点域，存储该顶点对应的下标
    struct ArcNode *next;	//链域，指向下一个邻接点
}ArcNode;

typedef struct VertexNode	//顶点表结构
{
    VertexType data;		//顶点域，存储顶点信息
    ArcNode *firstedge;		//边表头指针
}VertexNode, AdjList[MAXVEX];

typedef struct
{
    AdjList adjList;
    int numVertexes, numArcs;  //图中当前顶点数和边数
}ALGraph;

Status LocateVex(ALGraph *g,VertexType ch);
void CreateALGraph(ALGraph *g);
void DFS(ALGraph g,int i);
void DFSTraverse(ALGraph g);
Status GDegree(ALGraph g,int n);

#endif