#include "gm3d.h"

int GM3D::OutNeighborFile(char* v_name,char* b_name){
	/*
	if (strcmp(v_name,"NULL") || strcmp(b_name,"NULL")){
		//整理块体间的相邻关系 先初始化顶点相邻数组
		model_vert_neighbor_.resize(vert_num_);
		for (int i = 0; i < vert_num_; i++)
			model_vert_neighbor_[i].resize(8,-1);

		//遍历所有块体整理顶点相邻关系
		for (int i = 0; i < model_num_; i++){
			for (int j = 0; j < 8; j++){
				model_vert_neighbor_[model_cube_[i].ids[j]][j] = model_cube_[i].cen.id;
			}
		}

		//遍历所有顶点相邻关系 所有共点的块体都被认定为相邻块体
		model_cube_neighbor_.resize(model_num_);

		//循环顶点相邻列表 都不为-1则相互添加
		for (int i = 0; i < vert_num_; i++){
			for (int n = 0; n < 8; n++){
				if (model_vert_neighbor_[i][n] != -1){
					for (int k = 1; k < 8; k++){
						if (model_vert_neighbor_[i][(n+k)%8] != -1){
							model_cube_neighbor_[model_vert_neighbor_[i][n]].push_back(model_vert_neighbor_[i][(n+k)%8]);
							model_cube_neighbor_[model_vert_neighbor_[i][(n+k)%8]].push_back(model_vert_neighbor_[i][n]);
						}
					}
				}
			}
		}

		vector<int>::iterator pos; //整型向量的迭代器
		for (int i = 0; i < model_num_; i++){
			sort(model_cube_neighbor_[i].begin(),model_cube_neighbor_[i].end()); //对顶点序列由小到大排序
			pos = unique(model_cube_neighbor_[i].begin(),model_cube_neighbor_[i].end()); //获取重复序列开始的位置
			model_cube_neighbor_[i].erase(pos,model_cube_neighbor_[i].end()); //删除重复点
		}

		//清理数组
		for (int i = 0; i < vert_num_; i++){
			model_vert_neighbor_[i].clear();
			vector <int>().swap(model_vert_neighbor_[i]);
		}
		model_vert_neighbor_.clear();
		vector < vector<int> >().swap(model_vert_neighbor_);
	}
	*/
	return 0;
}