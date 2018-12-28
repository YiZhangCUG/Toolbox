#include "stt_class.h"
#include "progress_bar.h"

void SttGenerator::CloseSurface(QuadTree** forest){
	int breaked,local_id,new_count;
	int newids[4][3] = {{0,3,5},{1,4,3},{2,5,4},{3,4,5}};
	Vertex temp_vert;
	Vertex local_vert[6];
	Triangle temp_tri,temp_tri2;
	QuadTree* current_tree;

	ProgressBar *bar = new ProgressBar(max_depth_,"Close surface");
	//首先我们返回三角形列表再确定当前层的相邻关系
	for (int d = 0; d < max_depth_; d++){
		bar->Progressed(d);
		if (!array_out_tri_pointer_.empty()) array_out_tri_pointer_.clear();
		for (int t = 0; t < 20; t++){
			current_tree = *(forest+t);
			ReturnDepth(&(current_tree->root),d);
		}
		SortNeighbor(array_out_tri_pointer_);

		//遍历所有三角形询问
		for (int t = 0; t < array_out_tri_pointer_.size(); t++){
			if (array_out_tri_pointer_[t]->children[0] != NULL) continue; //如果节点三角形已经存在子节点则跳过
			else{
				//询问节点三角形的邻居
				for (int n = 0; n < 3; n++){
					//邻居必须存在
					if (array_out_tri_pointer_[t]->neighbor[n] != NULL){
						//如果邻居最大深度大于等于三层当前节点的深度 则直接在所有邻居新建三个顶点 完成后跳出邻居循环
						//这样做的主要原因是如果相邻节点深度差距太大的话无法保证在表面闭合的过程中三角形的质量 同时可以增加缝合带的宽度 有利于后续的建模使用
						breaked = 0;
						if (ReturnBranchDepth(&(array_out_tri_pointer_[t]->neighbor[n])) - array_out_tri_pointer_[t]->depth >= 3){
							for (int q = 0; q < 3; q++){
								if (array_out_tri_pointer_[t]->neighbor[(n+q)%3] != NULL){
									for (int l = 0; l < 3; l++)
										temp_tri2.ids[l] = array_out_tri_pointer_[t]->neighbor[(n+q)%3]->tri->ids[l];
									for (int h = 0; h < 3; h++){
										//计算新顶点坐标
										temp_vert.posic = 0.5*(array_stt_vert_[temp_tri2.ids[h]].posic + array_stt_vert_[temp_tri2.ids[(h+1)%3]].posic);
										ivm_ = map_str_vertex_.find(GetStringIndex(temp_vert));
										//若为新的顶点则将其增加到两个映射和一个链表中
										if(ivm_ == map_str_vertex_.end()){
											temp_vert.id = array_stt_vert_.size();//新的顶点索引等于顶点集的数量
											temp_vert.posis = Cartesian2Sphere(temp_vert.posic);
											array_stt_vert_.push_back(temp_vert);//将新产生的顶点保存到顶点链表中
											map_id_vertex_[temp_vert.id] = temp_vert;//将新产生的顶点保存到顶点索引映射中
											map_str_vertex_[GetStringIndex(temp_vert)] = temp_vert;//将新产生的顶点保存到顶点位置映射中
										}
									}
								}
							}
							breaked = 1;
						}
						if(breaked) break;
						//如果邻居节点存在与该节点相邻的孙节点 则可能需要增加新的顶点 如果只存在子节点则不需要增加新的顶点 所以此时我们不考虑子节点的情况
						//遍历邻居的所有子节点
						for (int m = 0; m < 4; m++){
							//要存在可能相邻的孙节点则必须存在子节点
							if (array_out_tri_pointer_[t]->neighbor[n]->children[m] != NULL){
								//如果邻居的一个子节点存在子节点则需要判断这个子节点是否与当前的节点三角形相邻 相邻的条件是邻居的这个存疑的子节点应该与当前节点三角形共顶点
								if (array_out_tri_pointer_[t]->neighbor[n]->children[m]->children[0] != NULL){
									//循环匹配当前节点三角形的三个顶点与存疑的子节点的三个顶点 如果共顶点则寻找当前节点三角形中唯一不与邻居三角形连接的顶点(外点)
									//并在公共顶点与外顶点的中点尝试新建一个顶点 如果新的顶点不存在则添加 否则继续
									for (int l = 0; l < 3; l++)
										temp_tri.ids[l] = array_out_tri_pointer_[t]->neighbor[n]->children[m]->tri->ids[l];
									for (int l = 0; l < 3; l++)
										temp_tri2.ids[l] = array_out_tri_pointer_[t]->neighbor[n]->tri->ids[l];
									breaked = 0;
									for (int h = 0; h < 3; h++){
										for (int f = 0; f < 3; f++){
											//检验顶点索引 找到公共点 
											if (array_out_tri_pointer_[t]->tri->ids[h] == temp_tri.ids[f]){
												//找到外点
												local_id = LocalIndex(temp_tri.ids[f],temp_tri2);
												if (array_out_tri_pointer_[t]->tri->ids[(h+1)%3] != temp_tri2.ids[(local_id+1)%3] && 
													array_out_tri_pointer_[t]->tri->ids[(h+1)%3] != temp_tri2.ids[(local_id+2)%3]){
													//外点是array_out_tri_pointer_[t]->tri->ids[(h+1)%3]
													temp_vert.posic = 0.5*(array_stt_vert_[array_out_tri_pointer_[t]->tri->ids[h]].posic
													 + array_stt_vert_[array_out_tri_pointer_[t]->tri->ids[(h+1)%3]].posic);//计算新顶点坐标
													ivm_ = map_str_vertex_.find(GetStringIndex(temp_vert));
													//若为新的顶点则将其增加到两个映射和一个链表中
													if(ivm_ == map_str_vertex_.end()){
														temp_vert.id = array_stt_vert_.size();//新的顶点索引等于顶点集的数量
														temp_vert.posis = Cartesian2Sphere(temp_vert.posic);
														array_stt_vert_.push_back(temp_vert);//将新产生的顶点保存到顶点链表中
														map_id_vertex_[temp_vert.id] = temp_vert;//将新产生的顶点保存到顶点索引映射中
														map_str_vertex_[GetStringIndex(temp_vert)] = temp_vert;//将新产生的顶点保存到顶点位置映射中
													}
												}
												else
												{
													//外点是array_out_tri_pointer_[t]->tri->ids[(h+2)%3]
													temp_vert.posic = 0.5*(array_stt_vert_[array_out_tri_pointer_[t]->tri->ids[h]].posic
													 + array_stt_vert_[array_out_tri_pointer_[t]->tri->ids[(h+2)%3]].posic);//计算新顶点坐标
													ivm_ = map_str_vertex_.find(GetStringIndex(temp_vert));
													//若为新的顶点则将其增加到两个映射和一个链表中
													if(ivm_ == map_str_vertex_.end()){
														temp_vert.id = array_stt_vert_.size();//新的顶点索引等于顶点集的数量
														temp_vert.posis = Cartesian2Sphere(temp_vert.posic);
														array_stt_vert_.push_back(temp_vert);//将新产生的顶点保存到顶点链表中
														map_id_vertex_[temp_vert.id] = temp_vert;//将新产生的顶点保存到顶点索引映射中
														map_str_vertex_[GetStringIndex(temp_vert)] = temp_vert;//将新产生的顶点保存到顶点位置映射中
													}
												}
												breaked = 1; break; //跳出循环 并触发二次跳出
											}
										}
										if(breaked) break;
									}
								}
							}
						}
					}
				}
			}
		}
		//第二次遍历所有三角形 按照未连接顶点个数补全下一层的三角形
		for (int t = 0; t < array_out_tri_pointer_.size(); t++){
			if (array_out_tri_pointer_[t]->children[0] != NULL) continue; //如果节点三角形已经存在子节点则跳过
			else{
				for (int i = 0; i < 3; i++)
					local_vert[i] = array_stt_vert_[array_out_tri_pointer_[t]->tri->ids[i]];
				//查询当前节点三角形的各边是否有未连接的顶点并记录
				new_count = 0;
				for (int i = 0; i < 3; i++){
					local_vert[i+3].posic = 0.5*(local_vert[i].posic + local_vert[(i+1)%3].posic);//计算新顶点坐标
					ivm_ = map_str_vertex_.find(GetStringIndex(local_vert[i+3]));
					//若为新的顶点则将其增加到两个映射和一个链表中
					if(ivm_!=map_str_vertex_.end()){
						local_vert[i+3].id = ivm_->second.id;
						new_count++;
					}
					else local_vert[i+3].id = -1;
				}
				//如果存在三个未连接的顶点则新建四个三角形
				if (new_count == 3){
					for (int i = 0; i < 4; i++){
						array_out_tri_pointer_[t]->children[i] = new QuadTreeNode;
						array_out_tri_pointer_[t]->children[i]->id = 10*(array_out_tri_pointer_[t]->id) + 1 + i;
						array_out_tri_pointer_[t]->children[i]->depth = array_out_tri_pointer_[t]->depth + 1;
						array_out_tri_pointer_[t]->children[i]->tri->ids[0] = local_vert[newids[i][0]].id;
						array_out_tri_pointer_[t]->children[i]->tri->ids[1] = local_vert[newids[i][1]].id;
						array_out_tri_pointer_[t]->children[i]->tri->ids[2] = local_vert[newids[i][2]].id;
					}
				}
				else if (new_count == 2){
					for (int i = 0; i < 3; i++){
						if (local_vert[i+3].id == -1){
							array_out_tri_pointer_[t]->children[0] = new QuadTreeNode;
							array_out_tri_pointer_[t]->children[0]->id = 10*(array_out_tri_pointer_[t]->id) + 1;
							array_out_tri_pointer_[t]->children[0]->depth = array_out_tri_pointer_[t]->depth + 1;
							array_out_tri_pointer_[t]->children[0]->tri->ids[0] = local_vert[i].id;
							array_out_tri_pointer_[t]->children[0]->tri->ids[1] = local_vert[(i+1)%3].id;
							array_out_tri_pointer_[t]->children[0]->tri->ids[2] = local_vert[(i+1)%3+3].id;

							array_out_tri_pointer_[t]->children[1] = new QuadTreeNode;
							array_out_tri_pointer_[t]->children[1]->id = 10*(array_out_tri_pointer_[t]->id) + 2;
							array_out_tri_pointer_[t]->children[1]->depth = array_out_tri_pointer_[t]->depth + 1;
							array_out_tri_pointer_[t]->children[1]->tri->ids[0] = local_vert[i].id;
							array_out_tri_pointer_[t]->children[1]->tri->ids[1] = local_vert[(i+1)%3+3].id;
							array_out_tri_pointer_[t]->children[1]->tri->ids[2] = local_vert[(i+2)%3+3].id;

							array_out_tri_pointer_[t]->children[2] = new QuadTreeNode;
							array_out_tri_pointer_[t]->children[2]->id = 10*(array_out_tri_pointer_[t]->id) + 3;
							array_out_tri_pointer_[t]->children[2]->depth = array_out_tri_pointer_[t]->depth + 1;
							array_out_tri_pointer_[t]->children[2]->tri->ids[0] = local_vert[(i+2)%3].id;
							array_out_tri_pointer_[t]->children[2]->tri->ids[1] = local_vert[(i+2)%3+3].id;
							array_out_tri_pointer_[t]->children[2]->tri->ids[2] = local_vert[(i+1)%3+3].id;
							break;
						}
					}
				}
				else if (new_count == 1){
					for (int i = 0; i < 3; i++){
						if (local_vert[i+3].id != -1){
							array_out_tri_pointer_[t]->children[0] = new QuadTreeNode;
							array_out_tri_pointer_[t]->children[0]->id = 10*(array_out_tri_pointer_[t]->id) + 1;
							array_out_tri_pointer_[t]->children[0]->depth = array_out_tri_pointer_[t]->depth + 1;
							array_out_tri_pointer_[t]->children[0]->tri->ids[0] = local_vert[i].id;
							array_out_tri_pointer_[t]->children[0]->tri->ids[1] = local_vert[i+3].id;
							array_out_tri_pointer_[t]->children[0]->tri->ids[2] = local_vert[(i+2)%3].id;

							array_out_tri_pointer_[t]->children[1] = new QuadTreeNode;
							array_out_tri_pointer_[t]->children[1]->id = 10*(array_out_tri_pointer_[t]->id) + 2;
							array_out_tri_pointer_[t]->children[1]->depth = array_out_tri_pointer_[t]->depth + 1;
							array_out_tri_pointer_[t]->children[1]->tri->ids[0] = local_vert[(i+2)%3].id;
							array_out_tri_pointer_[t]->children[1]->tri->ids[1] = local_vert[i+3].id;
							array_out_tri_pointer_[t]->children[1]->tri->ids[2] = local_vert[(i+1)%3].id;
							break;
						}
					}
				}
			}
		}
	}
	return;
}