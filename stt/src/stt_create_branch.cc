#include "stt_class.h"

void SttGenerator::CreateBranch(int upper_id,int order_id,int depth,int t_ids0,int t_ids1,int t_ids2,int phy_group,QuadTreeNode** node)
{
	Vertex local_vert[6];
	QuadTreeNode* current_node;

	*node = new QuadTreeNode; //初始化一个新的四叉树节点
	current_node = *node;
	current_node->tri->ids[0] = t_ids0;//将上一节点的三角形顶点索引赋值给current_node内的triangle.ids,因此每一层节点实际上都保存了其本身的三角形顶点索引
	current_node->tri->ids[1] = t_ids1;
	current_node->tri->ids[2] = t_ids2;
	current_node->tri->physic_group = phy_group; //继承上层的物理组
	current_node->id = upper_id*10+order_id;//写入四叉树节点编号
	current_node->depth = depth;//记录四叉树深度

	//额外生长条件 满足其一即可生长 在局部加密模型的过程中 不同物理组的赋值顺序前后顺序为圈 多边形 线 点
	if ((depth < tree_depth_ //基本生长条件 所有节点都能达到的深度
	 || InTriangleCircle(current_node)
	 || InTrianglePolygon(current_node)
	 || InTriangleLine(current_node)
	 || InTrianglePoint(current_node))
	 && depth < max_depth_) //最大深度限制 所有节点不能超过的深度
	{
		ivd_ = map_id_vertex_.find(t_ids0);//利用map_ID映射找到四叉树节点的前三个点，这三个节点是上一层四叉树产生的，必然存在
		local_vert[0] = ivd_->second;
		ivd_ = map_id_vertex_.find(t_ids1);
		local_vert[1] = ivd_->second;
		ivd_ = map_id_vertex_.find(t_ids2);
		local_vert[2] = ivd_->second;

		for(int i = 0; i < 3; i++)//循环产生三个新的顶点，位于节点三角形的三条边的中点，给每一个新产生的节点赋索引值与坐标，并保存到顶点链表中
		{
			local_vert[i+3].posic = 0.5*(local_vert[i%3].posic+local_vert[(i+1)%3].posic);//计算新顶点坐标,这里需要注意，如果顶点已经存在则需要将顶点索引重置，不增加顶点计数
			ivm_ = map_str_vertex_.find(GetStringIndex(local_vert[i+3]));
			if(ivm_ != map_str_vertex_.end())//利用map_vert查到当前顶点是否存在,这里需要注意，如果顶点已经存在则只需要将顶点索引置为已存在顶点的索引，不增加顶点计数
			{
				local_vert[i+3].id = ivm_->second.id;
			}
			else//若为新的顶点则将其增加到两个映射和一个链表中
			{
				local_vert[i+3].id = array_stt_vert_.size();//新的顶点索引等于顶点集的数量
				local_vert[i+3].posis = Cartesian2Sphere(local_vert[i+3].posic);
				array_stt_vert_.push_back(local_vert[i+3]);//将新产生的顶点保存到顶点链表中
				map_id_vertex_[local_vert[i+3].id] = local_vert[i+3];//将新产生的顶点保存到顶点索引映射中
				map_str_vertex_[GetStringIndex(local_vert[i+3])] = local_vert[i+3];//将新产生的顶点保存到顶点位置映射中
			}
		}

		CreateBranch(current_node->id,1,depth+1,local_vert[0].id,local_vert[3].id,local_vert[5].id,current_node->tri->physic_group,&(current_node->children[0]));
		CreateBranch(current_node->id,2,depth+1,local_vert[1].id,local_vert[4].id,local_vert[3].id,current_node->tri->physic_group,&(current_node->children[1]));
		CreateBranch(current_node->id,3,depth+1,local_vert[2].id,local_vert[5].id,local_vert[4].id,current_node->tri->physic_group,&(current_node->children[2]));
		CreateBranch(current_node->id,4,depth+1,local_vert[3].id,local_vert[4].id,local_vert[5].id,current_node->tri->physic_group,&(current_node->children[3]));
	}
	return;
}