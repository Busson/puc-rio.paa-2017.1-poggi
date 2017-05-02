
#include <stdio.h>
#include <limits.h>
#include <glib.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
#include "fibonacci_heap/heap.h"

gint64 
get_min_distance(gint64* dist, gint64* sptSet, gint64 nV){
    int min = INT_MAX, min_index;
    for (int v = 0; v < nV; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

void 
print_path(gint64*  parent, gint64 j){
    if (parent[j]==-1)
        return;
 
    print_path(parent, parent[j]);
    printf("%ld ", j);
}
 
void
print_solution(gint64* dist, gint64 nV, gint64*  parent, guint32 src){
    printf("Vertex\t  Distance\tPath");
    for (gint64 i = 1; i < nV; i++){
        if(dist[i]==INT_MAX)
          printf("\n%d -> %ld \t\t INF\t\t", src, i);
        else 
          printf("\n%d -> %ld \t\t %ld\t\t%d ", src, i, dist[i], src);
        print_path(parent, i);
    }
}

void 
initialize_vectors(gint64* dist, gint64* sptSet, gint64* parent, gint64 nV){
    for (int i = 0; i < nV; i++){
        parent[i] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }
} 


void 
dijkstra(STP_DOCUMENT *doc, gint64 src, gint64 nV){

    gint64 *dist = malloc((sizeof(gint64))*nV);
    gint64 *sptSet = malloc((sizeof(gint64))*nV);
    gint64 *parent = malloc((sizeof(gint64))*nV);
    
    initialize_vectors(dist,sptSet,parent,nV);

    dist[src] = 0;

    for (gint64 count = 0; count < nV-1; count++){
        gint64 u = get_min_distance(dist, sptSet,nV);

        sptSet[u] = 1;
        
        //for earch edge
        for(gint64 v=0; v< doc->edges; v++)
            if(doc->e[v].node1==u && !sptSet[doc->e[v].node2]
               && dist[doc->e[v].node1] + doc->e[v].c < dist[doc->e[v].node2]){
                parent[doc->e[v].node2] = doc->e[v].node1;
                dist[doc->e[v].node2] = dist[doc->e[v].node1] + doc->e[v].c; 
            }

    }

    print_solution(dist, nV, parent, src);
}

void print_data(data d){
    printf("%d\n", d.key);
}

int main(){
    heap* myheap = heap_init();

    int maxKey = 100000; //100 to 100 thousand by power of 10
    for (int i = 0; i<maxKey; i+=5){
        heap_insert(&myheap, i, NULL);
    }
    for (int i = 1; i<maxKey; i+=5){
        heap_insert(&myheap, i, NULL);
    }
    for (int i = 2; i<maxKey; i+=5){
        heap_insert(&myheap, i, NULL);
    }
    for (int i = 3; i<maxKey; i+=5){
        heap_insert(&myheap, i, NULL);
    }
    for (int i = 4; i<maxKey; i+=5){
        heap_insert(&myheap, i, NULL);
    }
    while (!is_empty(myheap)){
        print_data(heap_extract_min(&myheap));
    }
    heap_free(&myheap);
    return 0;
}