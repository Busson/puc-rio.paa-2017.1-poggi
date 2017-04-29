
#include <stdio.h>
#include <limits.h>
 
#define V 9
 
int 
minDistance(int dist[], int sptSet[])
{
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

void 
printPath(int parent[], int j)
{
    if (parent[j]==-1)
        return;
 
    printPath(parent, parent[j]);
    printf("%d ", j);
}
 
void
printSolution(int dist[], int n, int parent[])
{
    int src = 0;
    printf("Vertex\t  Distance\tPath");
    for (int i = 1; i < V; i++)
    {
        printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
        printPath(parent, i);
    }
}

void dijkstra(int graph[V][V], int src)
{
    int dist[V];  
    int sptSet[V];
    int parent[V];
 
    for (int i = 0; i < V; i++)
    {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }
 
    
    dist[src] = 0;

    for (int count = 0; count < V-1; count++)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = 1;

        for (int v = 0; v < V; v++)
        
            if (!sptSet[v] && graph[u][v]>-1 &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }  
    }

    printSolution(dist, V, parent);
}

int main(){
    
    int graph[V][V] = {{-1, 4, -1, -1, -1, -1, -1, 8, -1},
                       {4, -1, 8, -1, -1, -1, -1, 11, -1},
                       {-1, 8, -1, 7, -1, 4, -1, -1, 2},
                       {-1, -1, 7, -1, 9, 14, -1, -1, -1},
                       {-1, -1, -1, 9, -1, 10, -1, -1, -1},
                       {-1, -1, 4, 14, 10, -1, 2, -1, -1},
                       {-1, -1, -1, -1, -1, 2, -1, 1, 6},
                       {8, 11, -1, -1, -1, -1, 1, -1, 7},
                       {-1, -1, 2, -1, -1, -1, 6, 7, -1}
                      };
 
    dijkstra(graph, 0);
 
    return 0;
}