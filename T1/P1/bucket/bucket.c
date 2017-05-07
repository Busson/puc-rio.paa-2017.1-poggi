
#include "bucket.h"
#include <stdio.h>

guint32 count_n_operations;
guint32 count_m_operations;


bucket*
create_bucket(guint32 maxPath){
    bucket* buc = (bucket*)malloc(sizeof(bucket));
    buc->size = maxPath;
    buc->count = 0;
    buc->buckets = (bucket_node*)malloc( sizeof(bucket_node)*maxPath );
    for(guint32 i=0; i< maxPath; i++){
        buc->buckets[i].list = NULL;
        buc->buckets[i].value = i;
    }
    return buc;
}

void
bucket_add(bucket* buc, dijkstra_vertice* v){
  
    if(buc->buckets[v->dist].list == NULL){
        buc->buckets[v->dist].list = g_slist_alloc ();
    }
    buc->buckets[v->dist].list = g_slist_append(buc->buckets[v->dist].list, v); 
    buc->count++;
}

gboolean
bucket_is_empty(bucket* buc){
    if(buc->count <=0)return TRUE;
    return FALSE;
}

dijkstra_vertice* 
bucket_remove_min(bucket* buc){
 
   for(guint32 i=0; i< buc->size; i++ ){
        count_n_operations++;
        if(buc->buckets[i].list != NULL){
             
            GSList * l =  g_slist_last(buc->buckets[i].list);
            dijkstra_vertice* v = (dijkstra_vertice*)l->data;
            if(v!=NULL){
               buc->buckets[i].list = g_slist_remove(buc->buckets[i].list,v);
               buc->count--;
            }
            else {
               g_slist_free(buc->buckets[i].list);
               buc->buckets[i].list=NULL;
            }

            return v;
     }
   }
   return NULL;
}