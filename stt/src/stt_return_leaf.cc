#include "stt_class.h"

void SttGenerator::ReturnLeaf(QuadTreeNode** p_tree){
	QuadTreeNode* current_node = *p_tree;
	if (current_node->children[0]==NULL && current_node->children[1]==NULL && 
		current_node->children[2]==NULL && current_node->children[3]==NULL && 
		current_node->out_ok==true){
		array_out_tri_pointer_.push_back(current_node);
		return;
	}
	else{
		for (int i = 0; i < 4; i++){
			if (current_node->children[i]!=NULL)
				ReturnLeaf(&(current_node->children[i]));
		}
		return;
	}
}