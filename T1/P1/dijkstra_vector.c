
#include <stdio.h>
#include <limits.h>
#include <glib.h>
#include <stdlib.h>
#include "stp/stp_reader.h" 
 
struct dijkstra_vertice_s{ 
  gint64 value;
  gint64 dist;  
  gint64 sptSet;   
  guint32 sdjCount;
  struct dijkstra_vertice_s ** adjs;
  gint64* weights;
};

typedef struct dijkstra_vertice_s dijkstra_vertice;

dijkstra_vertice *
create_node_vertice(gint64 value,guint32 nV){
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

dijkstra_vertice*
get_min_distance(dijkstra_vertice **dv, gint64 nV){
    int min = INT_MAX, min_index;
    for (int v = 0; v < nV; v++)
        if (dv[v]->sptSet == 0 && dv[v]->dist <= min)
            min = dv[v]->dist, min_index = v;
 
    return dv[min_index];
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
initialize_vector(gint64* parent, gint64 nV){
    for (int i = 0; i < nV; i++){
        parent[i] = -1;
    }
} 


void 
dijkstra(dijkstra_vertice **dv, gint64 src, gint64 nV){

    gint64 *parent = malloc((sizeof(gint64))*nV);
    
    initialize_vector(parent,nV);

    dv[src]->dist=0;

    for (gint64 count = 0; count < nV-1; count++){
       dijkstra_vertice* v = get_min_distance(dv,nV);
       if(v==NULL)continue;
       v->sptSet=1;
       
       for(guint32 i=0;i< v->sdjCount;i++){
           dijkstra_vertice * w = dv[v->adjs[i]->value];
           if(!w->sptSet && v->dist + v->weights[i] < w->dist ){
                w->dist = v->dist + v->weights[i];
                parent[w->value] = v->value;
           } 
       }
    }

    print_solution(dv, nV, parent, src);
}

int main(int argc, char *argv[]){
    
    STP_DOCUMENT *doc = stp_new();  
    stp_get_content(doc, "input/sample.stp");
   
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
 
    dijkstra(dv, 1, doc->nodes+1);
 
    return 0;
}