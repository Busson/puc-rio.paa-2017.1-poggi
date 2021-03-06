
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
#include "alpha_tree/alpha.h"
#include "cpu_timer/CPUTimer.h"

static CPUTimer totaltime; 
guint32 count_process_nodes;

dijkstra_vertice *
create_node_vertice(gint64 value, guint32 nV){
   dijkstra_vertice* v = (dijkstra_vertice*)malloc(sizeof(dijkstra_vertice));
   v->value = value;
   v->dist= INT_MAX;
   v->sptSet =0;
   v->sdjCount=0;
   v->adjs = (dijkstra_vertice **)malloc(sizeof(dijkstra_vertice *)*nV);
   v->weights = (gint64*)malloc(sizeof(gint64)*nV);
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

void 
reset_node(dijkstra_vertice * v){
   if(v->dist!=INT_MAX) {
       count_process_nodes++;
   }
   v->dist= INT_MAX;
   v->sptSet =0;
}

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
     
     gint64 *parent = (gint64*)malloc((sizeof(gint64))*nV);
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
  //   print_solution(dv, nV, parent, src);
} 



int main(int argc, char *argv[]){
    
    STP_DOCUMENT *doc = stp_new();  
 //   stp_get_content(doc, "input/sample.stp");
     stp_get_content(doc, "input/ALUE/alue7080.stp");
    
    alpha_tree_t *tree = NULL;
    tree = alpha_create();

    dijkstra_vertice **dv = (dijkstra_vertice **)malloc( sizeof(dijkstra_vertice*)*(doc->nodes+1));

    for(guint32 i=0; i< doc->nodes+1; i++){
        dv[i] = create_node_vertice(i,doc->nodes+1);
    }

    for(gint64 i=0; i< doc->edges; i++){
       insert_adj_in_node(dv[doc->e[i].node1],
                          dv[doc->e[i].node2],
                          doc->e[i].c,
                          doc->nodes+1);                                     
    } 
    
    create_histogram();

    guint32 k =0;
    totaltime.reset();

    while( totaltime.getCPUTotalSecs() < 5.0 ){
      count_n_operations=0;
      count_m_operations=0;  
      count_process_nodes=0;
      max_height=0;
      balance_count=0;
      clear_histogram();

      totaltime.start();
      dijkstra(tree,dv, 1, doc->nodes+1);
      totaltime.stop();
      k++;
      for(guint32 i=0; i< doc->nodes+1; i++){
        reset_node(dv[i]);
      }
      tree = alpha_create();
    }

    printf("\nGraph: %d nodes %d edges",doc->nodes,doc->edges );
    printf("\nProcessed graph size: %d nodes",count_process_nodes);
    printf("\nAlpha factor: %f",ALPHA_FACTOR );
    printf("\nTotal balances: %d", balance_count);
    printf("\nMax height: %d ", max_height);
    printf("\n n: %d m: %d",count_n_operations,count_m_operations);
    printf("\nDijkstra : %f  k=%d total: %lf\n", totaltime.getCPUTotalSecs()/k, k, totaltime.getCPUTotalSecs() );
    
 //   print_histogram();
    return 0;
}
