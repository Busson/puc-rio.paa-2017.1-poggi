
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
#include "avl_tree/avl.h"



dijkstra_vertice *
create_node_vertice(gint64 value,guint32 nV){
   dijkstra_vertice* v = malloc(sizeof(dijkstra_vertice));
   v->value = value;
   v->dist= INT_MAX;
   v->parent = -1;
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

avl_node_t*
get_min_distance_avl(avl_tree_t *tree, gint64 nV){
    int min = INT_MAX, min_index;
     for (int v = 0; v < nV; v++){
         avl_node_t* node =  avl_find( tree, v ); 
         if(node->v->sptSet == 0 && node->v->dist < min )
            min = node->v->dist, min_index = v;
     }
     return avl_find( tree, min_index);
}

void 
print_path(gint64*  parent, gint64 j){
    if (parent[j]==-1)
        return;
 
    print_path(parent, parent[j]);
    printf("%ld ", j);
}
 
void
print_solution(avl_tree_t *tree, gint64 nV, gint64*  parent, guint32 src){
    printf("Vertex\t  Distance\tPath");
    for (gint64 i = 1; i < nV; i++){
        if(avl_find( tree, i)->v->dist==INT_MAX)
          printf("\n%d -> %ld \t\t INF\t\t", src, i);
        else 
          printf("\n%d -> %ld \t\t %ld\t\t%d ", src, i, avl_find( tree, i)->v->dist, src);
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
dijkstra(STP_DOCUMENT *doc, avl_tree_t *tree, gint64 src, gint64 nV){
     
    gint64 *parent = malloc((sizeof(gint64))*nV);
    initialize_parent(parent, nV);

    avl_node_t* node =  avl_find( tree, src ); 
    node->v->dist = 0;
      
    for (gint64 count = 0; count < nV-1; count++){
        node =  get_min_distance_avl(tree,nV);
        if(node==NULL)continue;
        node->v->sptSet=1;
    
        for(guint32 i=0;i<node->v->sdjCount;i++){
            if(!node->v->adjs[i]->sptSet && node->v->dist + node->v->weights[i] < node->v->adjs[i]->dist ){
            node->v->adjs[i]->dist = node->v->dist + node->v->weights[i]; 
            parent[node->v->adjs[i]->value] = node->value;
            }
        }
     }

    print_solution(tree, nV, parent, src);
}

int main(int argc, char *argv[]){
    
    STP_DOCUMENT *doc = stp_new();  
    stp_get_content(doc, "input/sample.stp");
  // stp_get_content(doc, "input/ALUE/alue2087.stp");
    
    avl_tree_t *tree = NULL;
    tree = avl_create();
    
    for(guint32 i=0; i< doc->nodes+1; i++){
       avl_insert( tree, i, create_node_vertice(i,doc->nodes+1));
    }

    for(gint64 i=0; i< doc->edges; i++){
     // printf("E %d %d %d\n",doc->e[i].node1,doc->e[i].node2,doc->e[i].c); 
        insert_adj_in_node(avl_find( tree,  doc->e[i].node1 )->v, 
                         avl_find( tree, doc->e[i].node2 )->v,
                         doc->e[i].c,
                         doc->nodes+1);               
    }

    dijkstra(doc, tree, 1, doc->nodes+1);
 
    return 0;
}
