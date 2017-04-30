
#ifndef STP_LIB_H
#define STP_LIB_H

#include <glib.h>

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

STP_DOCUMENT* stp_new();
void stp_get_content(STP_DOCUMENT* doc, const gchar* path);

#endif
