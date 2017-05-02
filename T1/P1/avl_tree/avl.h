
#ifndef AVL_H
#define AVL_H

#include <glib.h>

#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux. See feature_test_macros(7) */

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

struct avl_node_s {
	struct avl_node_s *left;
	struct avl_node_s *right;
	int value;
    struct dijkstra_vertice_s* v;
};

typedef struct avl_node_s avl_node_t;

struct avl_tree_s {
	struct avl_node_s *root;
};

typedef struct avl_tree_s avl_tree_t;


avl_tree_t *avl_create();

void avl_insert( avl_tree_t *tree, int value, dijkstra_vertice* v);

void avl_traverse_dfs( avl_tree_t *tree );

avl_node_t *avl_find( avl_tree_t *tree, int value );

#endif