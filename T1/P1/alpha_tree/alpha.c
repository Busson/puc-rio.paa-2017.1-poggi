#include <stdlib.h>
#include <stdio.h>

#include "alpha.h"


alpha_tree_t* 
alpha_create(){
    alpha_tree_t *tree = malloc(sizeof(alpha_tree_t));
	tree->root = NULL;
	return tree;	
}

alpha_node_t*
alpha_create_node() {
	alpha_node_t *node = malloc(sizeof(alpha_node_t));
	
	node->size = 1;
	node->count =0;
	node->pivot =0;
	node->left = NULL;
	node->right = NULL;
	node->vertices = g_slist_alloc ();

	return node;	
}

void
alpha_balance_node(alpha_node_t *node){
  
   printf("balance: %p L: %p R: %p \n",node, node->left, node->right);

   if(node->left!=NULL){
	   printf("balanceamento: %p - size %d | %p - size %d \n",node->left, node->left->size, node, (guint32)(ALPHA_FACTOR*node->size) );
	   if(node->left->size > (guint32)(ALPHA_FACTOR*node->size) ){
		  alpha_balance_node(node->left); 
		  GSList * l = g_slist_last(node->left->vertices);
          dijkstra_vertice* v = (dijkstra_vertice*)l->data;
          if(v!=NULL){
			node->left->vertices = g_slist_remove(node->left->vertices,v);
			node->left->count--;
			node->vertices = g_slist_append(node->vertices, v); 
			node->count++;
			printf("roubei o %d \n",v->value);
		  }
		  else{
			node->left=NULL;  
		  }
	   }
   }

   if(node->right!=NULL){
	   printf("balanceamento: %p - size %d | %p - size %d \n",node->right, node->right->size, node, (guint32)(ALPHA_FACTOR*node->size) );
	   if(node->right->size > (guint32)(ALPHA_FACTOR*node->size) ){
          alpha_balance_node(node->right); 
		  GSList * l = g_slist_last(node->right->vertices);
          dijkstra_vertice* v = (dijkstra_vertice*)l->data;
          if(v!=NULL){
			node->right->vertices = g_slist_remove(node->left->vertices,v);
			node->left->count--;
			node->vertices = g_slist_append(node->vertices, v); 
			node->count++;
			printf("roubei o %d \n",v->value);
		  }
		  else{
			node->right=NULL;  
		  } 
	   }
   }
       
}

void 
alpha_update_nodes_size(alpha_node_t* node){
    
	if(node->left == NULL && node->right == NULL)
	   node->size=1;

    guint32 size =1;
	if(node->left!=NULL){
		alpha_update_nodes_size(node->left);
		size += node->left->size;
	}
	if(node->right!=NULL){
		alpha_update_nodes_size(node->right);
		size += node->right->size;
	}   
    node->size=size; 
}

void
alpha_balance(alpha_tree_t* tree){
   if(tree==NULL)
      return;

   alpha_balance_node(tree->root);
   alpha_update_nodes_size(tree->root);
}

void 
alpha_node_add(alpha_node_t *node, dijkstra_vertice* v){
	 if(node==NULL)
	    return;
     node->pivot+= v->dist/2;
	 node->vertices = g_slist_append(node->vertices, v); 
	 node->count++; 

	 printf("add %d no %p - the size is %d \n",v->value,node, node->size );
}


dijkstra_vertice* 
get_min_node_alpha(alpha_tree_t* tree){
   alpha_node_t* target =  tree->root;
    
   GSList * l = g_slist_last(target->vertices);
   dijkstra_vertice* v = (dijkstra_vertice*)l->data;

   if(v!=NULL){
       target->vertices = g_slist_remove(target->vertices,v);  
	    target->count--;
		//update pivot       
   }

   alpha_balance(tree);
   
   return v;
}

void
alpha_insert(alpha_tree_t* tree, dijkstra_vertice* v){
    
	alpha_node_t* target=NULL;  
	alpha_node_t* last=NULL; 

	if(tree->root == NULL){
        tree->root = alpha_create_node();
		target = tree->root;  	
	}
	else{
       target = tree->root;

	   while(target!=NULL){
          
          if( v->dist < target->pivot ){
           //   printf("- PIVOT: %d ADD %ld \n",target->pivot,v->dist); 
		      if(target->count < target->size){
                  break;
			  }
			  else{
				 last = target;  
                 target = target->left;
				 if(target==NULL){
					target = alpha_create_node();
					last->left = target;
					break;
				 }
			  }
		  }
		  else{
			 //  printf("o size eh: %d \n", target->size);
			  if(target->count < target->size){
                  break;
			  }
			  else{
				 last = target; 
                 target = target->right;
				 if(target==NULL){
					target = alpha_create_node();
					last->right = target;
					break;
				 }
			  }
             // printf("+ PIVOT: %d ADD %ld \n",target->pivot,v->dist);
		  } 
		 // break;
	   }        
	}

    alpha_node_add(target,v);	
    alpha_update_nodes_size(tree->root);
	alpha_balance(tree);
}



