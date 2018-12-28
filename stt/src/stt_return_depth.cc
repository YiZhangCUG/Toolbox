#include "stt_class.h"

void SttGenerator::ReturnDepth(QuadTreeNode** p_tree,int back_depth){
	QuadTreeNode* current_node = *p_tree;
	if (current_node->depth == back_depth && back_depth <= max_depth_){
		array_out_tri_pointer_.push_back(current_node);
		return;
	}
	else{
		for (int i = 0; i < 4; i++){
			if (current_node->children[i] != NULL)
				ReturnDepth(&(current_node->children[i]),back_depth);
		}
		return;
	}
}