
#include <stdio.h>
#include <limits.h>
#include <glib.h>
#include <stdlib.h>
 
guint32 
get_min_distance(guint32* dist, guint32* sptSet, guint32 nV){
    int min = INT_MAX, min_index;
    for (int v = 0; v < nV; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

void 
print_path(guint32*  parent, guint32 j){
    if (parent[j]==-1)
        return;
 
    print_path(parent, parent[j]);
    printf("%d ", j);
}
 
void
print_solution(guint32* dist, guint32 nV, guint32*  parent){
    int src = 0;
    printf("Vertex\t  Distance\tPath");
    for (int i = 1; i < nV; i++){
        printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
        print_path(parent, i);
    }
}

void 
initialize_vectors(guint32* dist, guint32* sptSet, guint32* parent, guint32 nV){
    for (int i = 0; i < nV; i++){
        parent[i] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }
} 

void 
dijkstra(int graph[9][9], guint32 src, guint32 nV){

    guint32 *dist = malloc((sizeof(guint32))*nV);
    guint32 *sptSet = malloc((sizeof(guint32))*nV);
    guint32 *parent = malloc((sizeof(guint32))*nV);
    
    initialize_vectors(dist,sptSet,parent,nV);

    dist[src] = 0;

    for (guint32 count = 0; count < nV-1; count++){
        guint32 u = get_min_distance(dist, sptSet,nV);

        sptSet[u] = 1;

        for (guint32 v = 0; v < nV; v++)
            if (!sptSet[v] && graph[u][v]>-1 &&
                dist[u] + graph[u][v] < dist[v]){
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }  
    }

    print_solution(dist, nV, parent);
}

int main(){

    int graph[9][9] = {{-1, 4, -1, -1, -1, -1, -1, 8, -1},
                       {4, -1, 8, -1, -1, -1, -1, 11, -1},
                       {-1, 8, -1, 7, -1, 4, -1, -1, 2},
                       {-1, -1, 7, -1, 9, 14, -1, -1, -1},
                       {-1, -1, -1, 9, -1, 10, -1, -1, -1},
                       {-1, -1, 4, 14, 10, -1, 2, -1, -1},
                       {-1, -1, -1, -1, -1, 2, -1, 1, 6},
                       {8, 11, -1, -1, -1, -1, 1, -1, 7},
                       {-1, -1, 2, -1, -1, -1, 6, 7, -1}
                      };
 
    dijkstra(graph, 0, 9);
 
    return 0;
}