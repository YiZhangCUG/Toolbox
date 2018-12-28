#include "stt_class.h"

int SttGenerator::ReturnBranchDepth(QuadTreeNode** p_tree){
	int node_depth, max_depth = 0;
	QuadTreeNode* current_node = *p_tree;
	if (current_node->children[0]==NULL && current_node->children[1]==NULL && 
		current_node->children[2]==NULL && current_node->children[3]==NULL){
		return current_node->depth;
	}
	else{
		for (int i = 0; i < 4; i++){
			if (current_node->children[i] != NULL){
				node_depth = ReturnBranchDepth(&(current_node->children[i]));
				if(node_depth > max_depth) max_depth = node_depth;
			}
		}
		return max_depth;
	}
}