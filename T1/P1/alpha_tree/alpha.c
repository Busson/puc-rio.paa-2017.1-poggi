#include <stdlib.h>
#include <stdio.h>

#include "alpha.h"

guint32 max_height;
guint32 cur_height;
guint32 balance_count;
guint32 count_n_operations;
guint32 count_m_operations;

guint32* histogram;

void 
create_histogram(){
   histogram = (guint32*)malloc(sizeof(guint32)*50);
      for(guint32 i=0; i< 50;i++)
          histogram[i]=0;
}

void 
clear_histogram(){
   for(guint32 i=0; i< 50;i++){
      histogram[i]=0;
   }
}
void 
insert_histogram(guint32 v){
   histogram[v]++;
}

void 
print_histogram(){

   for(guint32 i=0; i< 40;i=i+5){
   printf("%d - %d | %d - %d | %d - %d | %d - %d | %d - %d \n",
   i,histogram[i],
   i+1,histogram[i+1],
   i+2,histogram[i+2],
   i+3,histogram[i+3],
   i+4,histogram[i+4]);
   }
}


alpha_tree_t* 
alpha_create(){
    alpha_tree_t *tree = (alpha_tree_t*)malloc(sizeof(alpha_tree_t));
	tree->root = NULL;
	tree->is_empty=FALSE; 
	return tree;	
}

alpha_node_t*
alpha_create_node() {
	alpha_node_t *node = (alpha_node_t*)malloc(sizeof(alpha_node_t));
	
	node->size = 1;
	node->count =0;
	node->pivot =0;
	node->left = NULL;
	node->right = NULL;
	node->vertices = g_slist_alloc ();

	return node;	
}

void 
for_earch_node(gpointer data, gpointer user_data){
     dijkstra_vertice* v = (dijkstra_vertice*)data;
	
    if(v!=NULL) {
		node_it* it = (node_it*)user_data;
		if(v->sptSet == 0 && v->dist < it->min ){
          it->min = v->dist;
		  it->min_index = v->value;
		  it->v = v;
		}
	}
}


void
alpha_balance_node(alpha_node_t *node){
  
 //  printf("balance: %p L: %p R: %p \n",node, node->left, node->right);

   if(node->left!=NULL){
//	   printf("balanceamento L: %p - size %d | %p - size %d \n",node->left, node->left->size, node, (guint32)(ALPHA_FACTOR*node->size) );
	   if(node->left->size > (guint32)(ALPHA_FACTOR*node->size) ){
		  balance_count++; /* debug */
	//	  printf("balance\n");
		  alpha_balance_node(node->left); 
		  node_it* it = (node_it*)malloc(sizeof(node_it));
   		  it->min=INT_MAX;
  		  it->min_index=-1;
 		  it->v= NULL;
		  g_slist_foreach(node->left->vertices, (GFunc)for_earch_node, it);
          if(it->v!=NULL){
			node->left->vertices = g_slist_remove(node->left->vertices, it->v);
			node->left->count--;
			node->vertices = g_slist_append(node->vertices, it->v); 
			node->count++;
//			printf("roubei o %d \n",v->value);
		  }
		  else{
			node->left=NULL;  
		  }
	   }
   }

   if(node->right!=NULL){
//	   printf("balanceamento R: %p - size %d | %p - size %d \n",node->right, node->right->size, node, (guint32)(ALPHA_FACTOR*node->size) );
	   if(node->right->size > (guint32)(ALPHA_FACTOR*node->size) ){
		  balance_count++; /* debug */
	//	  printf("balance\n");
          alpha_balance_node(node->right); 
		  node_it* it = (node_it*)malloc(sizeof(node_it));
   		  it->min=INT_MAX;
  		  it->min_index=-1;
 		  it->v= NULL;
		  g_slist_foreach(node->right->vertices, (GFunc)for_earch_node, it);
          if(it->v!=NULL){
			node->right->vertices = g_slist_remove(node->right->vertices,it->v);
			node->right->count--;
			node->vertices = g_slist_append(node->vertices, it->v); 
			node->count++;
//			printf("roubei o %d \n",v->value);
		  }
		  else{
			node->right=NULL;  
		  } 
	   }
   }
  //  printf("bal. end: %p", node);    
}

void 
alpha_update_nodes_size(alpha_node_t* node, guint32 height){
    
	if(node->left == NULL && node->right == NULL)
	   node->size=1;

    guint32 size =1;
	if(node->left!=NULL){
	   alpha_update_nodes_size(node->left,++height);
	   size += node->left->size;
	}
	if(node->right!=NULL){
	   alpha_update_nodes_size(node->right,++height);
	   size += node->right->size;
	}   
    node->size=size; 
	
	if(height > cur_height)
	   cur_height = height; 
}

void
alpha_balance(alpha_tree_t* tree){
   if(tree==NULL)
      return;

   alpha_balance_node(tree->root);
   alpha_update_nodes_size(tree->root,1);
}

void 
alpha_node_add(alpha_node_t *node, dijkstra_vertice* v){
	 if(node==NULL)
	    return;
     node->pivot+= v->dist/2;
	 node->vertices = g_slist_append(node->vertices, v); 
	 node->count++; 

//	 printf("add %d no %p - the size is %d \n",v->value,node, node->size );
}

void 
calcule_min_node(alpha_node_t* node, node_it* it){
	 g_slist_foreach(node->vertices, (GFunc)for_earch_node, it);
	 if(node->left!=NULL)
       calcule_min_node(node->left, it);
	 if(node->right!=NULL)
	   calcule_min_node(node->right, it);
}

dijkstra_vertice* 
get_min_node_alpha(alpha_tree_t* tree){
   alpha_node_t* target =  tree->root;
    
 //  printf("node content: ");
   node_it* it = (node_it*)malloc(sizeof(node_it));
   it->min=INT_MAX;
   it->min_index=-1;
   it->v= NULL;
   calcule_min_node(target, it);
   if(it->min_index <0) tree->is_empty=TRUE;
   
   if(it->v!=NULL){
	   target->vertices = g_slist_remove(target->vertices,it->v);
   }
   
   return it->v;
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
	cur_height=0;
    alpha_update_nodes_size(tree->root,1);
	alpha_balance(tree);
	insert_histogram(cur_height);
	//printf("CUR H: %d \n",cur_height);
}



