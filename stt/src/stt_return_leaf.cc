#include "stt_class.h"

void SttGenerator::ReturnLeaf(QuadTreeNode** p_tree){
	QuadTreeNode* current_node = *p_tree;
	if (current_node->children[0]==NULL && current_node->children[1]==NULL && 
		current_node->children[2]==NULL && current_node->children[3]==NULL && 
		current_node->out_ok==true){
		// bug fix 这里将输出的节点的邻居重置为NULL 否则在输出局部stt的邻居列表时将产生一个bug 即边缘位置的三角形会默认与0号三角形相邻
		// 这是因为在之前的闭合三角面过程中我们已全部对邻居列表赋值，所以所有节点三角形的邻居列表全不为空，因此一定会输出三个邻居，但对局部
		// stt而言，因为在输出环节的邻居排序中并不能找到对应的三角形索引，所以会输出一个默认值，即0。
		current_node->neighbor[0] = NULL;
		current_node->neighbor[1] = NULL;
		current_node->neighbor[2] = NULL;
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