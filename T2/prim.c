#include "input_reader/input_reader.h"
#include "cpu_timer/CPUTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FILE "inputs/TE80-1.TXT"


guint32 MAX_CAP = 10;
gint64 initialTime = 0; 
guint32 depth=0; 

gboolean undo_insert = false;

guint mstweight = 0;
guint mstweight_best = INT_MAX;
guint mstweight_lower = INT_MAX;
EDGE** mst;
EDGE** mst_best;
guint32* CAP_best;
guint8* relax_array_best;

gint64 
currentTime(){
    return  (g_get_real_time()-initialTime)/1000000; //microseconds to seconds 
}

void 
printCAP(guint32* CAP, guint32 numNodes){
   printf("SUB_TREE: "); 
   for(guint32 i=0; i < numNodes; i++){
       printf("%d - %d | ",i, CAP[i]+1);
   }
   printf("\n");
}

void
printCSV(EDGE** mst, guint32 numNodes){
 printf("Source,Target,Type,Id,Label,Weight\n");
 for(guint32 i=0; i < numNodes; i++){
   if(mst[i]==NULL)
      continue;

   printf("%d,%d,Undirected,%d,%d,%d\n", mst[i]->ori, mst[i]->dest, mst[i]->ori, mst[i]->ori, mst[i]->c);
 }
}

void 
printMST(EDGE** mst, guint32 numNodes){
    printf("MST: ");
    for(guint32 i=0; i < numNodes; i++){
        if(mst[i]==NULL)
           continue;

       printf("E-%d(%d,%d,%d) | ",i+1, mst[i]->ori, mst[i]->dest, mst[i]->c);
   } 
   printf("\n");
}

void 
copyCAP(guint32* CAP1, guint32* CAP2, guint32 numNodes){
   for(guint32 i=0; i < numNodes; i++)
       CAP1[i] = CAP2[i];
}

void 
copyMST(EDGE** mst1, EDGE** mst2, guint32 numNodes){
     for(guint32 i=0; i < numNodes; i++){
          mst1[i] = mst2[i];
     }
}

void 
copyRelaxArray(guint8* relax_array1, guint8* relax_array2, guint32 numEdges){
   for(guint32 i=0; i < numEdges; i++)
       relax_array1[i] = relax_array2[i];
}

void 
printRelaxArray(guint8* relax_array, guint32 numEdges){
   for(guint32 i=0; i < numEdges; i++){
       if(relax_array[i]==0)
         break;
       printf("%d ", relax_array[i]);
   }

   printf("\n");    
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
terminate(guint32 numNodes){

  printf("BEST CMST: %d  DEPTH:%d LOWE-B:%d\n", mstweight_best, depth, mstweight_lower);
  printMST(mst_best,numNodes);
  printCAP(CAP_best, numNodes); 

  exit(0);
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
updateCaps(EDGE** mst, guint32* CAP, guint32 dest, guint32 numNodes, gboolean* blocks, guint32 root){
 // printf(" %d ", dest);
 if(dest==root)
   return; 

  for(guint32 i=0; i < numNodes; i++){
      if(mst[i]!=NULL && mst[i]->dest == dest){
        CAP[mst[i]->ori]++;
        if(CAP[mst[i]->ori] >= MAX_CAP && mst[i]->ori != root){
            undo_insert=true;
        }
   //     printf(" %d -", mst[i]->ori);
        updateCaps(mst,CAP,mst[i]->ori,numNodes, blocks, root);
        if(undo_insert){
            CAP[mst[i]->ori]--;
          //  blocks[mst[i]->ori]=true;
        } 
      }
  }
}

void 
insertEdgeInMST(EDGE*** edges,EDGE** mst, guint32 lin, guint32 col, guint32 numNodes){
  // printf("insert %d - %d ", lin, col); 
   for(guint32 i=0; i < numNodes; i++){
       if(mst[i] == NULL){
           mst[i] = edges[lin][col];
           break;
       }
   }
}

void forceEdgeInMST(EDGE*** edges,EDGE** mst, guint32 lin, guint32 col, guint32 numNodes){
   
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
getMinEdge(EDGE*** edges, EDGE** mst, gint32* key, guint8* relax_array, guint32 numNodes, guint32* lin, guint32* col, gboolean* blocks){
   guint32 min = INT_MAX;
   for(guint32 i=0; i< numNodes; i++){
       if(key[i] >= INT_MAX || blocks[i])
           continue;

       for(guint32 j=0; j< numNodes; j++){
             if(  (edges[i][j]->c < min && key[j]>= INT_MAX) ){
                 min = edges[i][j]->c;
                 *lin = i;
                 *col = j;
             }
       } 

   }
   key[*col] = min;
}

gboolean 
cap_mst(EDGE*** edges, EDGE** mst, guint32* CAP, gint32* key, guint8* relax_array, gboolean* blocks, guint32 numNodes, guint32 numEdges){
  
 // printRelaxArray(relax_array, numEdges);
  
  for(guint i=0; i< numEdges; i++){
    
   // printf("(%d,%d) ",i,i);
    if(relax_array[i]!=1)
       continue;

     guint32 lin = i/numNodes;
     guint32 col = i%numNodes;
  
    if(lin == col){   
       return false;
    }
    
    if(key[lin] >= INT_MAX || key[col] >= INT_MAX){
        
     //    printf("insert Edge(%d,%d,%d) ",lin, col, edges[lin][col]->c );

         insertEdgeInMST(edges, mst, lin, col, numNodes);
         if(key[lin] >= INT_MAX)
             key[lin] = edges[lin][col]->c;
          if(key[col] >= INT_MAX)
             key[col] = edges[lin][col]->c; 
         updateCaps(mst,CAP,edges[lin][col]->dest, numNodes, blocks, 0); 
         if(undo_insert){
            return false;
         } 
         mstweight += edges[lin][col]->c;  
          
     }
     else{
        
         mstweight = 0; 
         clearCAP(CAP, numNodes);
         clearMST(mst, numNodes);
         clearBlocks(blocks, numNodes); 
         clearKeys(key, numNodes); 
      //   printRelaxArray(relax_array, numEdges);
      //   printf("invalid mst");
         return false;
     } 
     
  } 
  
  key[0]=0;
  guint aux=0;
       
  guint32 lin;
  guint32 col; 
  for(guint32 i=0; i< numNodes-1; i++){
   //   printf("1-");
      getMinEdge(edges,mst, key, relax_array, numNodes,&lin,&col,blocks);
   //   printf("2-");
      insertEdgeInMST(edges, mst, lin, col, numNodes);
   //   printf("3-");
      updateCaps(mst,CAP,edges[lin][col]->dest, numNodes, blocks, 0);
   //   printf("4-");
      
    
      if(relax_array[(lin*numNodes) + col]==2){
           printf("(%d-%d-%d)", lin, col, (lin*numNodes) + col);
          goto end_func;
       }
  
      if(undo_insert){
          aux++;                  
          removeEdgeFromMST(edges, mst, lin, col, numNodes);
          blocks[lin] = true;
          key[col] = INT_MAX;
          undo_insert=false;
          i--;
          if(aux>=numNodes){
              if(CAP[0]== numNodes-1)
                 goto end_func;

              mstweight = 0; 
              goto end_func_error;
          }
            
      }
      else {
          aux=0;
          mstweight += edges[lin][col]->c;   
      }             
  }
                 

  end_func:

  if(CAP[0] < (numNodes-1)){
      clearCAP(CAP, numNodes); 
      return false;
  } 

  if(CAP[0]== numNodes-1 &&  mstweight_best > mstweight){
            mstweight_best = mstweight;
            copyMST(mst_best, mst, numNodes);
            copyCAP(CAP_best, CAP, numNodes);
            copyRelaxArray(relax_array_best, relax_array, numEdges);
  }
  
  mstweight = 0; 
  
  clearMST(mst, numNodes);
  clearBlocks(blocks, numNodes); 
  clearKeys(key, numNodes);

  clearCAP(CAP, numNodes); 
  return true;
  
  end_func_error:

  mstweight = 0; 
  
  clearMST(mst, numNodes);
  clearBlocks(blocks, numNodes); 
  clearKeys(key, numNodes);
  clearCAP(CAP, numNodes); 
  return false;

}

void 
branch_and_bound(EDGE*** edges, EDGE** mst, guint32* CAP, gint32* key, guint8* relax_array, gboolean* blocks, guint32 numNodes, guint32 numEdges, guint it){
  
 // printf("%ld \n",currentTime());
  if( currentTime() >= 3600)
     return;  

  if(it > depth)
      depth = it;

  if(it >= numEdges)
      return;
  
  cap_mst(edges, mst, CAP, key, relax_array, blocks, numNodes, it);

  if(mstweight > mstweight_best)
      return; 
  
  if(mstweight_lower == mstweight_best)
      terminate(numNodes);
     
  
  relax_array[it]=1; //force use
  branch_and_bound(edges, mst, CAP, key, relax_array, blocks, numNodes, numEdges, it+1);

  relax_array[it]=2; //no_use
  branch_and_bound(edges, mst, CAP, key, relax_array, blocks, numNodes, numEdges, it+1); 
     
  
  
}

int 
main(){

  guint32 numNodes;
  guint32 numEdges;
  EDGE*** edges;
  guint16 src =0;
  //edges = read_content("inputs/TEST.TXT", &numNodes);
  edges = read_content(FILE, &numNodes);
 //  edges = read_content("inputs/TC40-1.TXT", &numNodes);
  //edges = read_content("inputs/TC80-1.TXT", &numNodes);

  numEdges = (numNodes*numNodes);
  //init
  mst = (EDGE**)malloc(sizeof(EDGE*)*numNodes);
  mst_best = (EDGE**)malloc(sizeof(EDGE*)*numNodes);
  gint32* key = (gint32*)malloc(sizeof(gint32)*numNodes); 
  guint32* CAP = (guint32*)malloc(sizeof(guint32)*numNodes);
  CAP_best = (guint32*)malloc(sizeof(guint32)*numNodes);
  gboolean* blocks = (gboolean*)malloc(sizeof(gboolean)*numNodes);
  guint8* relax_array = (guint8*)malloc(sizeof(guint8)* numEdges);
  relax_array_best = (guint8*)malloc(sizeof(guint8)* numEdges);
  
  for(guint32 i=0; i< numNodes; i++){
      mst[i] = NULL;
      mst_best[i] = NULL;
      key[i] = INT_MAX;
      CAP[i] = 0;
      blocks[i] = false;
      relax_array[i] = 0;
      relax_array[i+numNodes] = 0;
 
  }

  MAX_CAP = numNodes;

  printf("FILE: %s  START C=%d  Nodes=%d \n",FILE, MAX_CAP,numNodes);
  
  //obtendo o lower-bound
  cap_mst(edges, mst, CAP, key, relax_array, blocks, numNodes, 0);
  mstweight_lower = mstweight_best;
  mstweight_best = INT_MAX;
  

  //percorrendo o branch and bound
  MAX_CAP = 10;
  initialTime = g_get_real_time();
  branch_and_bound(edges,mst,CAP, key, relax_array, blocks, numNodes, numEdges, 0);
  
  printf("NODES: %d \n", numNodes);
  printf("BEST CMST: %d  DEPTH:%d LOWE-B:%d\n", mstweight_best, depth, mstweight_lower);
  printMST(mst_best,numNodes);
  printCAP(CAP_best, numNodes);
 // printf("\n");
 // printCSV(mst_best,numNodes);

//  printRelaxArray(relax_array_best, numEdges);

}
