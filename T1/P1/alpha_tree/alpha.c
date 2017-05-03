#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#include "alpha.h"

/* Create a new AVL tree. */
alpha_tree_t *alpha_create() {
	alpha_tree_t *tree = NULL;

	if( ( tree = malloc( sizeof( alpha_tree_t ) ) ) == NULL ) {
		return NULL;
	}

	tree->root = NULL;
	return tree;
}

/* Initialize a new node. */
alpha_node_t *alpha_create_node() {
	alpha_node_t *node = NULL;
	
	if( ( node = malloc( sizeof( alpha_node_t ) ) ) == NULL ) {
		return NULL;
	}

	node->left = NULL;
	node->right = NULL;
	node->value = 0;

	return node;	
}

/* Find the height of an ALPHA node recursively */
int alpha_node_height( alpha_node_t *node ) {
	int height_left = 0;
	int height_right = 0;

	if( node->left ) height_left = alpha_node_height( node->left );
	if( node->right ) height_right = alpha_node_height( node->right );

	return height_right > height_left ? ++height_right : ++height_left;
}

/* Find the balance of an ALPHA node */
int alpha_balance_factor( alpha_node_t *node ) {
	int bf = 0;

	if( node->left  ) bf += alpha_node_height( node->left );
	if( node->right ) bf -= alpha_node_height( node->right );

	return bf ;
}

/* Left Left Rotate */
alpha_node_t *alpha_rotate_leftleft( alpha_node_t *node ) {
 	alpha_node_t *a = node;
	alpha_node_t *b = a->left;
	
	a->left = b->right;
	b->right = a;

	return( b );
}

/* Left Right Rotate */
alpha_node_t *alpha_rotate_leftright( alpha_node_t *node ) {
	alpha_node_t *a = node;
	alpha_node_t *b = a->left;
	alpha_node_t *c = b->right;
	
	a->left = c->right;
	b->right = c->left; 
	c->left = b;
	c->right = a;

	return( c );
}

/* Right Left Rotate */
alpha_node_t *alpha_rotate_rightleft( alpha_node_t *node ) {
	alpha_node_t *a = node;
	alpha_node_t *b = a->right;
	alpha_node_t *c = b->left;
	
	a->right = c->left;
	b->left = c->right; 
	c->right = b;
	c->left = a;

	return( c );
}

/* Right Right Rotate */
alpha_node_t *alpha_rotate_rightright( alpha_node_t *node ) {
	alpha_node_t *a = node;
	alpha_node_t *b = a->right;
	
	a->right = b->left;
	b->left = a; 

	return( b );
}

/* Balance a given node */
alpha_node_t *alpha_balance_node( alpha_node_t *node ) {
	alpha_node_t *newroot = NULL;

	/* Balance our children, if they exist. */
	if( node->left )
		node->left  = alpha_balance_node( node->left  );
	if( node->right ) 
		node->right = alpha_balance_node( node->right );

	if(node->left!=NULL){
		if( node->value*ALPHA_FACTOR < node->left->value ) {
			/* Left Heavy */	
			if( alpha_balance_factor( node->left ) <= -1 ) 
				newroot = alpha_rotate_leftright( node );
			else 
				newroot = alpha_rotate_leftleft( node );

		}  
	}
	if(node->right!=NULL){	
		if( node->value*ALPHA_FACTOR < node->right->value ) {
			/* Right Heavy */
			if( alpha_balance_factor( node->right ) >= 1 )
				newroot = alpha_rotate_rightleft( node );
			else 
				newroot = alpha_rotate_rightright( node );
		} 
	}

	newroot = node;
	

	return( newroot );	
}

/* Balance a given tree */
void alpha_balance( alpha_tree_t *tree ) {

	alpha_node_t *newroot = NULL;

	newroot = alpha_balance_node( tree->root );

	if( newroot != tree->root )  {
		tree->root = newroot; 
	}
}

/* Insert a new node. */
void alpha_insert( alpha_tree_t *tree, int value, dijkstra_vertice* v) {
	alpha_node_t *node = NULL;
	alpha_node_t *next = NULL;
	alpha_node_t *last = NULL;

	/* Well, there must be a first case */ 	
	if( tree->root == NULL ) {
		node = alpha_create_node();
		node->value = value;
        node->v = v;
		tree->root = node;

	/* Okay.  We have a root already.  Where do we put this? */
	} else {
		next = tree->root;

		while( next != NULL ) {
			last = next;

			if( value < next->value ) {
				next = next->left;

			} else if( value > next->value ) {
				next = next->right;

			/* Have we already inserted this node? */
			} else if( value == next->value ) {
				/* This shouldn't happen. */	
			}
		}

		node = alpha_create_node();
		node->value = value;
        node->v = v;

		if( value < last->value ) last->left = node;
		if( value > last->value ) last->right = node;
		
	}

	alpha_balance( tree );
}

/* Find the node containing a given value */
alpha_node_t *alpha_find( alpha_tree_t *tree, int value ) {
	alpha_node_t *current = tree->root;

	while( current && current->value != value ) {
		if( value > current->value )
			current = current->right;
		else
			current = current->left;
	}

	return current;
}

/* Do a depth first traverse of a node. */
void alpha_traverse_node_dfs( alpha_node_t *node, int depth ) {
	int i = 0;

	if( node->left ) alpha_traverse_node_dfs( node->left, depth + 2 );

	for( i = 0; i < depth; i++ ) putchar( ' ' );
	printf( "%d: %d\n", node->value, alpha_balance_factor( node ) );

	if( node->right ) alpha_traverse_node_dfs( node->right, depth + 2 );
}

/* Do a depth first traverse of a tree. */
void alpha_traverse_dfs( alpha_tree_t *tree ) {
	alpha_traverse_node_dfs( tree->root, 0 );
}
