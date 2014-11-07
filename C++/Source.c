#include <stdio.h>
#include "graph.h"

int main()
{
	ALGraph g;
	CreateALGraph(&g);
	DFSTraverse(g);
	return 0;
}