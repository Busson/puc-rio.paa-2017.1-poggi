
#ifndef ALPHA_H
#define ALPHA_H

#include <glib.h>

#define ALPHA_FACTOR 0.5


struct dijkstra_vertice_s{ 
  guint32 value;
  gint64 dist;  
  gint64 sptSet;   
  guint32 sdjCount;
  struct dijkstra_vertice_s ** adjs;
  gint64* weights;
};

typedef struct dijkstra_vertice_s dijkstra_vertice;

struct alpha_node_s {
	struct alpha_node_s *left;
	struct alpha_node_s *right;
  GSList * vertices;
  guint32 size;
  guint32 count;
  guint32 pivot;
};

typedef struct alpha_node_s alpha_node_t;

struct alpha_tree_s {
	struct alpha_node_s *root;
};

typedef struct alpha_tree_s alpha_tree_t;

alpha_tree_t* alpha_create();

void alpha_insert(alpha_tree_t* tree, dijkstra_vertice* v);

dijkstra_vertice* get_min_node_alpha(alpha_tree_t* tree);

#endif