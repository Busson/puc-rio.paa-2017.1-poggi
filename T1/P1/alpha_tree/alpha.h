
#ifndef alpha_H
#define alpha_H

#include <glib.h>

#define ALPHA_FACTOR 0.75
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

struct alpha_node_s {
	struct alpha_node_s *left;
	struct alpha_node_s *right;
	int value;
    struct dijkstra_vertice_s* v;
};

typedef struct alpha_node_s alpha_node_t;

struct alpha_tree_s {
	struct alpha_node_s *root;
};

typedef struct alpha_tree_s alpha_tree_t;


alpha_tree_t *alpha_create();

void alpha_insert( alpha_tree_t *tree, int value, dijkstra_vertice* v);

void alpha_traverse_dfs( alpha_tree_t *tree );

alpha_node_t *alpha_find( alpha_tree_t *tree, int value );

#endif