
#ifndef ALPHA_H
#define ALPHA_H

#include <glib.h>

#define ALPHA_FACTOR 0.5

extern guint32 max_height;
extern guint32 cur_height;
extern guint32 balance_count;
extern guint32 count_n_operations;
extern guint32 count_m_operations;

extern guint32* histogram;

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
  gboolean is_empty;
	struct alpha_node_s *root;
};

typedef struct alpha_tree_s alpha_tree_t;

typedef struct{
   gint32 min;
   gint32 min_index;
   dijkstra_vertice *v;
}node_it;

alpha_tree_t* alpha_create();

void alpha_insert(alpha_tree_t* tree, dijkstra_vertice* v);

dijkstra_vertice * get_min_node_alpha(alpha_tree_t* tree);

void create_histogram();

void clear_histogram();

void insert_histogram(guint32 v);

void print_histogram();

#endif