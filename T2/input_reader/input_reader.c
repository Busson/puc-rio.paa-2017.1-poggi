#include "input_reader.h"

EDGE*** 
createEdges(guint32 size){
    EDGE*** edges;
    edges = (EDGE***)malloc(sizeof(EDGE**)*(size+1));
    for(guint32 i=0; i< (size+1); i++){
        edges[i] = (EDGE**)malloc(sizeof(EDGE*)*(size+1));
        for(guint32 j=0; j< (size+1); j++){
            edges[i][j] = (EDGE*)malloc(sizeof(EDGE));
            edges[i][j]->ori = -1;
            edges[i][j]->dest = -1;
            edges[i][j]->c=0;
        }
    }
   
   return edges; 
}

void
insertEdge(EDGE*** edges, guint32 size, guint32 lin, guint32 col, guint32 c){  
  //  printf("%d - %d v: %d |", lin, col, c);  
    edges[lin][col]->ori = lin;
    edges[lin][col]->dest = col;
    edges[lin][col]->c = c;
}

void 
printEdges(EDGE*** edges, guint32 size){
   for(guint32 i=0; i<=size; i++){
      for(guint32 j=0; j<=size; j++){
           printf("%d ", edges[i][j]->c);
      }
      printf("\n");
   }
}

EDGE***
read_content(const gchar* path, guint *nNodes){
    GError* error=NULL;
    gsize *length=NULL;
    GFile* file = g_file_new_for_path(path);
    g_assert_nonnull(file);
    GFileInputStream* inputStream = g_file_read(file, NULL, &error);
    g_assert_nonnull(inputStream);
    GDataInputStream* data = g_data_input_stream_new((GInputStream*)inputStream); 
    g_assert_nonnull(data);
    gchar* line = g_data_input_stream_read_line(data, length, NULL, &error);  
    guint32 numNodes;
    gboolean gflag= FALSE;
    EDGE*** edges;
    guint32 lin=0;
    guint32 col=0;
    while(line!=NULL){
        gchar** str_arr = g_strsplit (line," ",-1);
        int ss = g_strv_length(str_arr);
        for(guint32 i=0; i < ss; i++ ){
           if(str_arr[i][0] < '0' || str_arr[i][0] > '9')
             continue;
           if(gflag==FALSE){
               numNodes = g_ascii_strtoll(str_arr[i],NULL,10);
               gflag = TRUE;
               edges = createEdges(numNodes);
               *nNodes = (numNodes+1);
           }
           else{
               insertEdge( edges, numNodes, lin, col, g_ascii_strtoll(str_arr[i],NULL,10));
               col++;
               if(col > numNodes){
                 col=0;
                 lin++;   
               }
           }
        }
        line = g_data_input_stream_read_line(data, length, NULL, &error);  
    }
    g_object_unref(data);
    g_object_unref(inputStream);
    g_object_unref(file);
    printEdges(edges, numNodes);
    return edges;
}