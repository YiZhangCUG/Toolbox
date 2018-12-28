#include "stt_class.h"

void SttGenerator::CutOutline(QuadTreeNode** p_tree){
	//切割范围多边形之外的四叉树节点 从深到浅执行
	QuadTreeNode* current_node = *p_tree;
	//当前节点是叶子节点 进行判断
	if (current_node->children[0]==NULL && current_node->children[1]==NULL && 
		current_node->children[2]==NULL && current_node->children[3]==NULL){
		//如果节点三角形在范围多边形之外 删除节点三角形 同时初始化指针
		if (OutPolyOutline(current_node)){
			current_node->out_ok = false;
			return;
		}
		else return;
	}
	else{
		for (int i = 0; i < 4; i++){
			//顺序访问当前节点的四个子节点 先处理子节点的情况 当然前提是节点存在
			if (current_node->children[i] != NULL){
				CutOutline(&(current_node->children[i]));
			}
			else continue;
		}
	}
	return;
}