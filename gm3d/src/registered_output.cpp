#include "gm3d.h"

int GM3D::RegisteredOuput(bool remove_empty_element){
	int count;
	//统计输出模型单元块体和顶点 以及输出的块体数据列表
	if (remove_empty_element){
		count = 0;
		//遍历所有块体数据 注册有值的块体
		for (int i = 0; i < model_num_; i++){
			if (model_block_val_[i] != BDL_MAX){
				out_ele_data_ids_.push_back(i);
				out_ele_ids_.push_back(i);
				ele_data_out_map_[count] = count;
				count++;
			}
		}

		//遍历所有注册的块体 添加顶点
		for (int i = 0; i < out_ele_ids_.size(); i++){
			for (int j = 0; j < 8; j++){
				out_vert_ids_.push_back(model_cube_[out_ele_ids_[i]].ids[j]);
			}
		}

		//去除输出顶点中的重复部分
		vector<int>::iterator pos; //整型向量的迭代器
		sort(out_vert_ids_.begin(),out_vert_ids_.end()); //对顶点序列由小到大排序
		pos = unique(out_vert_ids_.begin(),out_vert_ids_.end()); //获取重复序列开始的位置
		out_vert_ids_.erase(pos,out_vert_ids_.end()); //删除重复点

		//将需要输出的模型顶点序号与它们的排序做一个对应 保证在输出文件中顶点索引号始终是从0开始的连续的序列
		for (int i = 0; i < out_vert_ids_.size(); i++){
			vert_out_map_[out_vert_ids_[i]] = i;
		}
	}
	else{
		//输出的模型块体为所有
		out_ele_ids_.resize(model_num_);
		for (int i = 0; i < model_num_; i++){
			out_ele_ids_[i] = i;
		}

		//输出的模型顶点为所有
		out_vert_ids_.resize(vert_num_);
		for (int i = 0; i < vert_num_; i++){
			vert_out_map_[i] = out_vert_ids_[i] = i;
		}

		//注册所有有值的块体数据
		count = 0;
		for (int i = 0; i < model_num_; i++){
			if (model_block_val_[i] != BDL_MAX){
				ele_data_out_map_[count] = i;
				out_ele_data_ids_.push_back(i);
				count++;
			}
		}
	}
	//输出所有模型单元块体和顶点 只统计输出的块体数据列表
	return 0;
}