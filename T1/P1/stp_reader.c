
//gnome libs
#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>

enum STP_SESSION{
   NONE,
   COMMENT,
   GRAPH,
   TERMINALS,
   COORDINATES
};

typedef struct{
   gchar * name;
   gchar * creator;
   gchar * remark;
}STP_COMMENT;

typedef struct{
   guint32 node1;
   guint32 node2;
   guint32 c;
}STP_EDGE;

typedef struct{
   guint32 node;
}STP_TERMINAL;

typedef struct{
   guint32 node;
   gint64 x;
   gint64 y;
}STP_COORDINATE;

typedef struct {
   STP_COMMENT comment;
   guint32 nodes;
   guint32 edges;
   STP_EDGE* e;
   guint32 terminals;
   STP_TERMINAL* t;
   STP_COORDINATE* dd;
}STP_DOCUMENT;

STP_DOCUMENT*
stp_new(){
   STP_DOCUMENT* doc = malloc (sizeof (STP_DOCUMENT));
   doc->comment.name = NULL;
   doc->comment.creator = NULL;
   doc->comment.remark = NULL;
   doc->nodes = 0; doc->edges = 0;
   doc->e = NULL; doc->terminals = 0;
   doc->t=NULL; doc->dd = NULL;
   return doc;
}

void 
processCommentLine(STP_DOCUMENT* doc, gchar * line){
                                
}

void 
processGraphLine(STP_DOCUMENT* doc, gchar * line){
   gchar** str_arr = g_strsplit (line," ",-1);  
   if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "nodes") == 0){
     doc->nodes = (guint32)g_ascii_strtoll(str_arr[1],NULL,10);
     doc->dd = malloc (sizeof(STP_COORDINATE)*doc->nodes);
     guint32 i; for(i=0; i< doc->nodes; i++){
       doc->dd[i].node= 0;
     }
   }
   else if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "edges") == 0){ 
     doc->edges = (guint32)g_ascii_strtoll(str_arr[1],NULL,10);
     doc->e = malloc (sizeof(STP_EDGE)*doc->edges);
     guint32 i; for(i=0; i< doc->edges; i++){
       doc->e[i].node1= 0;
     }
   }
   else if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "e") == 0) {
     guint32 i; for(i=0; i< doc->edges; i++){
       if(doc->e[i].node1==0){
         doc->e[i].node1 = (guint32)g_ascii_strtoll(str_arr[1],NULL,10);
         doc->e[i].node2 = (guint32)g_ascii_strtoll(str_arr[2],NULL,10);
         doc->e[i].c = (guint32)g_ascii_strtoll(str_arr[3],NULL,10);
         break;
       }
     }
   }                      
}

void 
processTerminalsLine(STP_DOCUMENT* doc, gchar * line){
   gchar** str_arr = g_strsplit (line," ",-1);  
   if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "terminals") == 0){ 
     doc->terminals = (guint32)g_ascii_strtoll(str_arr[1],NULL,10); 
     doc->t = malloc (sizeof(STP_EDGE)*doc->terminals);
     guint32 i; for(i=0; i< doc->terminals; i++){
       doc->t[i].node = 0;
     }
   } 
   else if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "t") == 0){
     guint32 i; for(i=0; i< doc->terminals; i++){
       if(doc->t[i].node == 0){
         doc->t[i].node = (guint32)g_ascii_strtoll(str_arr[1],NULL,10);
         break; 
       }
     }
   }                             
}

void 
processCoordinatesLine(STP_DOCUMENT* doc, gchar * line){
   gchar** str_arr = g_strsplit (line," ",-1);  
   if(g_strcmp0(g_ascii_strdown(str_arr[0], -1), "dd") == 0) {
     guint32 i; for(i=0; i< doc->nodes; i++){
       if(doc->dd[i].node==0){
         doc->dd[i].node = (guint32)g_ascii_strtoll(str_arr[1],NULL,10);
         doc->dd[i].x = g_ascii_strtoll(str_arr[2],NULL,10);
         doc->dd[i].y = g_ascii_strtoll(str_arr[3],NULL,10);
         break;
       }
     }   
   }                          
}

void 
stp_get_content(STP_DOCUMENT* doc, const gchar* path){
   GError* error=NULL;
   gsize *length=NULL;  
   enum STP_SESSION session = NONE;
   GFile* file = g_file_new_for_path(path);
   g_assert_nonnull(file);
   GFileInputStream* inputStream = g_file_read(file, NULL, &error);
   g_assert_nonnull(inputStream);
   GDataInputStream* data = g_data_input_stream_new((GInputStream*)inputStream); 
   g_assert_nonnull(data);
   gchar* line = g_data_input_stream_read_line(data, length, NULL, &error);
   while(line!=NULL){
      if(g_strcmp0(g_ascii_strdown(line, -1), "section comment") == 0)
         session = COMMENT;
      else if(g_strcmp0(g_ascii_strdown(line, -1), "section graph") == 0)
         session = GRAPH;
      else if(g_strcmp0(g_ascii_strdown(line, -1), "section terminals") == 0)
         session = TERMINALS;   
      else if(g_strcmp0(g_ascii_strdown(line, -1), "section coordinates") == 0)
         session = COORDINATES;
      else if(g_strcmp0(g_ascii_strdown(line, -1), "end") == 0)
         session = NONE;   
      
      if(session == COMMENT)
         processCommentLine(doc, line);
      else if(session == GRAPH)
         processGraphLine(doc, line);
      else if(session == TERMINALS)
         processTerminalsLine(doc, line);   
      else if(session == COORDINATES)
         processCoordinatesLine(doc, line);      

      line = g_data_input_stream_read_line(data, length, NULL, &error);
   } 
   g_object_unref(data);
   g_object_unref(inputStream);
   g_object_unref(file);
}

int main(){

    STP_DOCUMENT *doc = stp_new();  
    stp_get_content(doc, "input/dmxa0296.stp");
   
    printf("SECTION Graph\nNodes %d\nEdges %d\n", doc->nodes, doc->edges);
    guint32 i; for(i=0; i< doc->edges; i++){
      printf("E %d %d %d\n", doc->e[i].node1,doc->e[i].node2,doc->e[i].c); 
    }
    printf("END\n\n"); 
    printf("SECTION Terminals\nTerminals %d\n", doc->terminals);
    for(i=0; i< doc->terminals; i++){
      printf("T %d\n", doc->t[i].node); 
    }
    printf("END\n\n");
    printf("SECTION Coordinates\n");
    for(i=0; i< doc->nodes; i++){
      printf("DD %d %ld %ld\n",doc->dd[i].node, doc->dd[i].x, doc->dd[i].y); 
    }
    printf("END\n\nEOF");
    
}


