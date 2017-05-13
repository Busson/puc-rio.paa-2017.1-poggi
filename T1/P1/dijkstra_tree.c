
#include <glib.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
#include "cpu_timer/CPUTimer.h"

static CPUTimer totaltime; 
guint32 count_process_nodes;
guint32 count_n_operations;
guint32 count_m_operations;

struct dijkstra_vertice_s{ 
  gint64 value;
  gint64 dist;  
  gint64 sptSet;   
  guint32 sdjCount;
  struct dijkstra_vertice_s ** adjs;
  gint64* weights;
};

typedef struct dijkstra_vertice_s dijkstra_vertice;

dijkstra_vertice *s;

dijkstra_vertice *
create_node_vertice(gint64 value,guint32 nV){
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
reset_node(dijkstra_vertice * v){
   if(v->dist!=INT_MAX) {
       count_process_nodes++;
   }
   v->dist= INT_MAX;
   v->sptSet =0;
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

gint
cmp_func(gpointer a, gpointer b){
   
    gint64 aa = ((dijkstra_vertice *)a)->dist;
    gint64 bb = ((dijkstra_vertice *)b)->dist;

   if(aa<bb) return -1;
   else if(aa==bb) return 0;
   else return 1;
}



gint
search_func(gpointer a, gpointer b){

    s = (dijkstra_vertice *)a;

    return -1;
}


void 
dijkstra(dijkstra_vertice **dv, gint64 src, gint64 nV){
    
    gint64 *parent = (gint64*)malloc((sizeof(gint64))*nV);
    initialize_parent(parent, nV);
    
    GTree * tree = g_tree_new ((GCompareFunc)cmp_func);

    dv[src]->dist=0;
    g_tree_insert(tree,dv[src], dv[src]);

    gint64 a = 0;

    while( g_tree_height (tree)>0){
       
       g_tree_search(tree,(GCompareFunc)search_func,&a);
       count_n_operations+=g_tree_height (tree)*2;
       s->sptSet=1;
       g_tree_remove(tree,s);  

       for(guint32 i=0;i< s->sdjCount;i++){ /* 1.3 */
           dijkstra_vertice * w = dv[s->adjs[i]->value];
           if(!w->sptSet && s->dist + s->weights[i] < w->dist ){
                w->dist = s->dist + s->weights[i];
                parent[w->value] = s->value;
                g_tree_insert(tree,dv[w->value], dv[w->value]);
                count_m_operations+=g_tree_height (tree);
           } 
       }
      
    }

  //  print_solution(dv, nV, parent, src);
}

int main(int argc, char *argv[]){
    
    STP_DOCUMENT *doc = stp_new();  
 //   stp_get_content(doc, "input/sample.stp");
    stp_get_content(doc, "input/DMXA/dmxa0848.stp");
    
     dijkstra_vertice **dv = (dijkstra_vertice**)malloc( sizeof(dijkstra_vertice*)*(doc->nodes+1));
    
    for(guint32 i=0; i< doc->nodes+1; i++){
       dv[i] = create_node_vertice(i,doc->nodes+1);
    }

    for(gint64 i=0; i< doc->edges; i++){
       insert_adj_in_node(dv[doc->e[i].node1],
                          dv[doc->e[i].node2],
                          doc->e[i].c,
                          doc->nodes+1);                  
    } 

  

    
    guint32 k =1;
    totaltime.reset();

    while( totaltime.getCPUTotalSecs() < 5 ){
      count_n_operations=0;
      count_m_operations=0;  
      count_process_nodes=0;
     
      totaltime.start(); 
      dijkstra(dv, 1, doc->nodes+1);
      totaltime.stop();
 
      k++;
      for(guint32 i=0; i< doc->nodes+1; i++){
        reset_node(dv[i]);
      }
    } 


    printf("\nGraph: %d nodes %d edges",doc->nodes,doc->edges );
    printf("\nProcessed graph size: %d nodes",count_process_nodes);
    printf("\n n: %d m: %d",count_n_operations,count_m_operations);
    printf("\nDijkstra : %f  k=%d total: %lf\n", totaltime.getCPUTotalSecs()/k, k, totaltime.getCPUTotalSecs() );


    return 0;
}
