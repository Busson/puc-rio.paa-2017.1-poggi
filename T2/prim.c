#include "input_reader/input_reader.h"
#include "cpu_timer/CPUTimer.h"
#include "limits.h"
#include "fibonacci_heap/heap.h"


void
getMinEdge(EDGE*** edges, gint32* key,guint32 numNodes, guint32* lin, guint32* col){
   guint32 min = INT_MAX;
   for(guint32 i=0; i< numNodes; i++){
       if(key[i] >= INT_MAX)
           continue;

       for(guint32 j=0; j< numNodes; j++){
             if(edges[i][j]->c < min && key[j]>= INT_MAX){
                 min = edges[i][j]->c;
                 *lin = i;
                 *col = j;
             }
       } 

   }
   //edges[*lin][*col]->c = min;
   key[*col] = min;
}

int main(){
  
  guint32 numNodes;
  EDGE*** edges;
  guint16 src =0;
  //edges = read_content("inputs/TEST.TXT", &numNodes);
  edges = read_content("inputs/TC40-1.TXT", &numNodes);
  
  //init
  gint32* key = (gint32*)malloc(sizeof(gint32)*numNodes); 
  gint32* pred = (gint32*)malloc(sizeof(gint32)*numNodes); 

  for(guint32 i=0; i< numNodes; i++){
      pred[i] = -1;
      key[i] = INT_MAX;
     // printf("%d - ",i);
  }
  
  key[src]=0;
  
  printf("Edge(N2,N1,W) \n");

  for(guint32 i=0; i< numNodes; i++){
         guint32 lin;
         guint32 col;
         getMinEdge(edges,key,numNodes,&lin,&col);
        
         printf("Edge(%d,%d,%d) | ",lin, col, edges[lin][col]->c);
  }


}