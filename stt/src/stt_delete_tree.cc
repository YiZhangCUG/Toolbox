#include "stt_class.h"

void SttGenerator::DeleteTree(QuadTreeNode** p_tree){
	QuadTreeNode* current_node = *p_tree;
	if (current_node == NULL){
		return;
	}
	else{
		for (int i = 0; i < 4; i++){
			DeleteTree(&(current_node->children[i]));
		}
		delete current_node->tri; //构造函数初始化 一定存在
		delete current_node; //上面已经判断证否 一定存在
		current_node = NULL;
		return;
	}
}