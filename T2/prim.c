#include "input_reader/input_reader.h"
#include "cpu_timer/CPUTimer.h"
#include "limits.h"
#include "fibonacci_heap/heap.h"

#define MAX_CAP 3
#define ROOT_NODE 0

gboolean undo_insert = false;


void 
printCAP(guint32* CAP, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++)
       printf("%d - %d | ",i, CAP[i]);
}

void 
printMST(EDGE** mst, guint32 numNodes){
    printf("Edge(N2,N1,W) \n");
    for(guint32 i=0; i < numNodes; i++){
        if(mst[i]==NULL)
           continue;

       printf("Edge(%d,%d,%d) | ", mst[i]->ori, mst[i]->dest, mst[i]->c);
   } 
}

void 
clearCAP(guint32* CAP, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++)
        CAP[i] = 0;
}

void 
clearMST(EDGE** mst, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++)
        mst[i] = NULL;
}

void
clearBlocks(gboolean* blocks, guint32 numNodes){
  for(guint32 i=0; i < numNodes; i++)
     blocks[i] = false;
}

void
clearKeys(gint32* key, guint32 numNodes){
  for(guint32 i=0; i < numNodes; i++)
    key[i] = INT_MAX;
}

void 
updateCaps(EDGE** mst, guint32* CAP, guint32 dest, guint32 numNodes, gboolean* blocks){
 // printf("Update ");  
  for(guint32 i=0; i < numNodes; i++){
      if(mst[i]!=NULL && mst[i]->dest == dest){
        CAP[mst[i]->ori]++;
        if(CAP[mst[i]->ori] >= MAX_CAP && mst[i]->ori != ROOT_NODE){
            undo_insert=true;
        }
   //     printf(" %d -", mst[i]->ori);
        updateCaps(mst,CAP,mst[i]->ori,numNodes, blocks);
        if(undo_insert){
            CAP[mst[i]->ori]--;
          //  blocks[mst[i]->ori]=true;
        } 
      }
  }
}

void 
insertEdgeInMST(EDGE*** edges,EDGE** mst, guint32 lin, guint32 col, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++){
       if(mst[i] == NULL){
           mst[i] = edges[lin][col];
           break;
       }
   }
}

void 
removeEdgeFromMST(EDGE*** edges,EDGE** mst, guint32 lin, guint32 col, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++){
       if(mst[i]->dest == edges[lin][col]->dest &&  mst[i]->ori == edges[lin][col]->ori){
           mst[i] = NULL;
           break;
       }
   } 
}

void
getMinEdge(EDGE*** edges, EDGE** mst, gint32* key,guint32 numNodes, guint32* lin, guint32* col, gboolean* blocks){
   guint32 min = INT_MAX;
   for(guint32 i=0; i< numNodes; i++){
       if(key[i] >= INT_MAX || blocks[i])
           continue;

       for(guint32 j=0; j< numNodes; j++){
             if(edges[i][j]->c < min && key[j]>= INT_MAX){
                 min = edges[i][j]->c;
                 *lin = i;
                 *col = j;
             }
       } 

   }
   key[*col] = min;
}

void 
relax(EDGE*** edges, guint32 numNodes){
  
  
}

int 
main(){
  
  guint mstweight= 0;
  guint32 numNodes;
  EDGE*** edges;
  EDGE** mst;
  guint16 src =0;
  //edges = read_content("inputs/TEST.TXT", &numNodes);
  edges = read_content("inputs/TE16.txt", &numNodes);
  
  //init
  mst = (EDGE**)malloc(sizeof(EDGE*)*numNodes);
  gint32* key = (gint32*)malloc(sizeof(gint32)*numNodes); 
  guint32* CAP = (guint32*)malloc(sizeof(guint32)*numNodes);
  gboolean* blocks = (gboolean*)malloc(sizeof(gboolean)*numNodes);
  
  for(guint32 i=0; i< numNodes; i++){
      mst[i] = NULL;
      key[i] = INT_MAX;
      CAP[i] = 0;
      blocks[i]=false;
     // printf("%d - ",i);
  }
  
 
/* for(guint32 m=0; m< numNodes; m++){
     for(guint32 n=0; n< numNodes; n++){

            for(guint32 k=0; k< numNodes; k++){
                for(guint32 l=0; l< numNodes; l++){

                    printf("%d - %d \n",k,l);  */
                    key[ROOT_NODE]=0;
                    
              //      if(k!=n && l!=n)  
              //          edges[k][l]->c= 1000;  

                        for(guint32 i=0; i< numNodes-1; i++){
                            guint32 lin;
                            guint32 col;
                            getMinEdge(edges,mst, key,numNodes,&lin,&col,blocks);
                    //      printf("I got Edge(%d,%d,%d) \n", edges[lin][col]->ori, edges[lin][col]->dest, edges[lin][col]->c);
                            insertEdgeInMST(edges, mst, lin, col, numNodes);
                            updateCaps(mst,CAP,edges[lin][col]->dest, numNodes, blocks);
                            if(undo_insert){
                        //         printf("\n I removed Edge(%d,%d,%d) ", edges[lin][col]->ori, edges[lin][col]->dest, edges[lin][col]->c);
                                removeEdgeFromMST(edges, mst, lin, col, numNodes);
                    //          printf(" Bloquiei o %d e liberei o %d \n",lin, col);
                                blocks[lin] = true;
                                key[col] = INT_MAX;
                                undo_insert=false;
                                i--;
                            }
                            else 
                            mstweight += edges[lin][col]->c;
                        //    printf("\n"); 
                    }
                    
                    printf("MST TOTAL WEIGHT: %d \n", mstweight);
                    printMST(mst, numNodes);
                //    printCAP(CAP, numNodes);
        /*            mstweight = 0;
                    clearCAP(CAP, numNodes);
                    clearMST(mst, numNodes);
                    clearBlocks(blocks, numNodes);
                    clearKeys(key, numNodes);
                }
            }
            edges = read_content("inputs/TE16.txt", &numNodes);
      }
   }  */

}