#include "stt_class.h"
#include "progress_bar.h"

void SttGenerator::SortNeighbor(QuadTreeNodePointerArray input_pointers)
{
	int local_id1,local_id2;
	IntArray1D vert_index; //当前层的顶点索引列表
	vector<int>::iterator pos; //整型向量的迭代器
	IntArray2D vert_neighbor; //对应于当前层顶点索引的相邻三角形索引列表
	IntArray2D neighbor_index; //三角形的相邻索引
	Int2IntMap map_vert_id;

	//确定当前层的所有顶点索引列表
	if (!vert_index.empty()) vert_index.clear();
	for (int i = 0; i < input_pointers.size(); i++){
		for (int t = 0; t < 3; t++){
			vert_index.push_back(input_pointers[i]->tri->ids[t]);
		}
	}
	//去除所有重复点
	sort(vert_index.begin(),vert_index.end()); //对顶点序列由小到大排序
	pos = unique(vert_index.begin(),vert_index.end()); //获取重复序列开始的位置
	vert_index.erase(pos,vert_index.end()); //删除重复点
	//初始化map_vert_id
	if (!map_vert_id.empty()) map_vert_id.clear();
	for (int i = 0; i < vert_index.size(); i++){
		map_vert_id[vert_index[i]] = i;
	}
	//确定与顶点相连的三角形索引列表
	if (!vert_neighbor.empty()){
		for (int i = 0; i < vert_neighbor.size(); i++){
			if (!vert_neighbor[i].empty()) vert_neighbor[i].clear();
		}
	}
	vert_neighbor.resize(vert_index.size());
	for (int i = 0; i < input_pointers.size(); i++){
		for (int j = 0; j < 3; j++){
			vert_neighbor[map_vert_id[input_pointers[i]->tri->ids[j]]].push_back(i);
		}
	}

	if (!neighbor_index.empty()){
		for (int i = 0; i < neighbor_index.size(); i++){
			if (!neighbor_index[i].empty()) neighbor_index[i].clear();
		}
	}
	neighbor_index.resize(input_pointers.size());
	for (int i = 0; i < neighbor_index.size(); i++)
		neighbor_index[i].reserve(3);

	//确定相邻的三角形
	for (int i = 0; i < vert_neighbor.size(); i++){
		//顺序取其中两个三角形 size为1时循环直接跳过
		for (int m = 0; m < vert_neighbor[i].size()-1; m++){
			local_id1 = LocalIndex(vert_index[i],*(input_pointers[vert_neighbor[i][m]]->tri));
			for (int n = m+1; n < vert_neighbor[i].size(); n++){
				local_id2 = LocalIndex(vert_index[i],*(input_pointers[vert_neighbor[i][n]]->tri));
				//使用单边相邻进行判定 避免重复添加
				if (input_pointers[vert_neighbor[i][m]]->tri->ids[(local_id1+1)%3] == input_pointers[vert_neighbor[i][n]]->tri->ids[(local_id2+2)%3]){
					//相互添加
					neighbor_index[vert_neighbor[i][m]].push_back(vert_neighbor[i][n]);
					neighbor_index[vert_neighbor[i][n]].push_back(vert_neighbor[i][m]);
				}
			}
		}
	}


	//最后为对应的节点添加neighbor
	for (int i = 0; i < neighbor_index.size(); i++)
	{
		for (int j = 0; j < neighbor_index[i].size(); j++)
		{
			input_pointers[i]->neighbor[j] = input_pointers[neighbor_index[i][j]];
		}
	}
	return;
}