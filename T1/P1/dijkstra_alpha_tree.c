
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
#include "alpha_tree/alpha.h"

dijkstra_vertice *
create_node_vertice(gint64 value, guint32 nV){
   dijkstra_vertice* v = malloc(sizeof(dijkstra_vertice));
   v->value = value;
   v->dist= INT_MAX;
   v->sptSet =0;
   v->sdjCount=0;
   v->adjs = malloc(sizeof(dijkstra_vertice *)*nV);
   v->weights = malloc(sizeof(gint64)*nV);
   for(guint32 i=0; i< nV; i++){
     v->adjs[i]=NULL;
     v->weights[i]=0;
   }
   return v;
}

void
insert_adj_in_node(dijkstra_vertice *v,dijkstra_vertice *w, gint64 p, guint32 nV){
   for(guint32 i=0; i< nV; i++){
     if(v->adjs[i]==NULL){
         v->adjs[i]= w;
         v->sdjCount++;
         v->weights[i]=p;
         break;
     }
   }
}

/*
alpha_node_t*
get_min_distance_alpha(alpha_tree_t *tree, gint64 nV){
    int min = INT_MAX, min_index;
     for (int v = 0; v < nV; v++){
         alpha_node_t* node =  alpha_find( tree, v ); 
         if(node->v->sptSet == 0 && node->v->dist < min )
            min = node->v->dist, min_index = v;
     }
     return alpha_find( tree, min_index);
}
*/

void 
print_path(gint64*  parent, gint64 j){
    if (parent[j]==-1)
        return;
 
    print_path(parent, parent[j]);
    printf("%ld ", j);
}
 
void
print_solution(dijkstra_vertice ** dv, gint64 nV, gint64*  parent, guint32 src){
    printf("Vertex\t  Distance\tPath");
    for (gint64 i = 1; i < nV; i++){
        if(dv[i]->dist ==INT_MAX)
          printf("\n%d -> %ld \t\t INF\t\t", src, i);
        else 
          printf("\n%d -> %ld \t\t %ld\t\t%d ", src, i, dv[i]->dist, src);
        print_path(parent, i);
    }
}

void 
initialize_parent(gint64* parent, gint64 nV){
    for (int i = 0; i < nV; i++){
        parent[i] = -1;
    }
} 

void 
dijkstra(alpha_tree_t *tree, dijkstra_vertice **dv, gint64 src, gint64 nV){
     
     gint64 *parent = malloc((sizeof(gint64))*nV);
     initialize_parent(parent, nV);

     dv[src]->dist=0;
     alpha_insert(tree, dv[src]);  

     while(!tree->is_empty){
        dijkstra_vertice* v = get_min_node_alpha(tree);
        if(v==NULL)
             break;

        v->sptSet=1;

     //   printf("peguei %d \n", v->value);
       
        for(guint32 i=0;i< v->sdjCount;i++){
           dijkstra_vertice * w = dv[v->adjs[i]->value];
           if(!w->sptSet && v->dist + v->weights[i] < w->dist ){
                w->dist = v->dist + v->weights[i];
                parent[w->value] = v->value;
                alpha_insert(tree, dv[w->value]);
           } 
        } 
     }
     print_solution(dv, nV, parent, src);
} 



int main(int argc, char *argv[]){
    
    STP_DOCUMENT *doc = stp_new();  
 //   stp_get_content(doc, "input/sample.stp");
   stp_get_content(doc, "input/ALUE/alue2087.stp");
    
    alpha_tree_t *tree = NULL;
    tree = alpha_create();

    dijkstra_vertice **dv = malloc( sizeof(dijkstra_vertice*)*(doc->nodes+1));

    for(guint32 i=0; i< doc->nodes+1; i++){
        dv[i] = create_node_vertice(i,doc->nodes+1);
    }

    for(gint64 i=0; i< doc->edges; i++){
       insert_adj_in_node(dv[doc->e[i].node1],
                          dv[doc->e[i].node2],
                          doc->e[i].c,
                          doc->nodes+1);                                     
    } 

    dijkstra(tree,dv, 1, doc->nodes+1);
    
    printf("\n\nTotal nodes: %d", doc->nodes);
    printf("\nAlpha factor: %f",ALPHA_FACTOR );
    printf("\nTotal balances: %d", balance_count);
    printf("\nMax height: %d \n", max_height);
 
    return 0;
}
