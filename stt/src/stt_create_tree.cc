#include "stt_class.h"

void SttGenerator::CreateTree(int tree_id,int t_ids0,int t_ids1,int t_ids2,QuadTree* p_tree){
	if (max_depth_ == 0){
		p_tree->root->id = 0;
		p_tree->root->depth = 0;
		p_tree->root->tri->ids[0] = t_ids0;//将上一节点的三角形顶点索引赋值给currNode内的triangle.ids,因此每一层节点实际上都保存了其本身的三角形顶点索引
		p_tree->root->tri->ids[1] = t_ids1;
		p_tree->root->tri->ids[2] = t_ids2;
		for(int i=0;i<4;i++)
		{
			p_tree->root->children[i] = NULL;
		}
	}
	else
	{
		CreateBranch(0,tree_id,0,t_ids0,t_ids1,t_ids2,0,&(p_tree->root));//以根节点开始创建四叉树
	}
}