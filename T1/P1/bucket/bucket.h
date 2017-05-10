#ifndef BUCKET_H
#define BUCKET_H

#include <stdlib.h>
#include <glib.h>

extern guint32 count_n_operations;
extern guint32 count_m_operations;

typedef struct{
   guint32 value; 
   GSList * list;
}bucket_node;

typedef struct{
   guint32 size;
   guint32 count;
   guint32 lastPos;
   bucket_node* buckets;
}bucket; 

struct dijkstra_vertice_s{ 
  gint64 value;
  gint64 dist;  
  gint64 parent;
  gint64 sptSet;   
  guint32 sdjCount;
  struct dijkstra_vertice_s ** adjs;
  gint64* weights;
};

typedef struct dijkstra_vertice_s dijkstra_vertice;

bucket* create_bucket(guint32 maxPath);

void bucket_add(bucket* buc, dijkstra_vertice* v);

dijkstra_vertice* bucket_remove_min(bucket* buc);

gboolean bucket_is_empty(bucket* buc);

#endif

