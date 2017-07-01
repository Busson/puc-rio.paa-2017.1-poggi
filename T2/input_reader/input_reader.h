#include <gio/gio.h>
#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
   guint32 ori; 
   guint32 dest;
   guint32 c;
}EDGE;

EDGE*** createEdges(guint32 size);
EDGE*** read_content(const gchar* path,guint *numNodes);