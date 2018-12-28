#include "stt_class.h"

int SttGenerator::InTrianglePoint(QuadTreeNode* node){
	//没有插入的点位 直接返回否
	if (array_control_point_.empty()){
		return 0;
	}
	else{
		int count, node_depth;
		double node_resolution;
		Cpoint tri_nor;
		Cpoint cross_point;

		Triangle temp_tri;
		for (int j = 0; j < 3; j++){
			temp_tri.ids[j] = node->tri->ids[j];
		}

		node_depth = node->depth;

		node_resolution = 0;
		for (int i = 0; i < 3; i++){
			node_resolution += acos(DotProduct(array_stt_vert_[temp_tri.ids[i]].posic,array_stt_vert_[temp_tri.ids[(i+1)%3]].posic)
				/(ModuleLength(array_stt_vert_[temp_tri.ids[i]].posic)*ModuleLength(array_stt_vert_[temp_tri.ids[(i+1)%3]].posic)));
		}
		node_resolution = node_resolution*60/Pi;

		tri_nor = CrossProduct(array_stt_vert_[temp_tri.ids[1]].posic - array_stt_vert_[temp_tri.ids[0]].posic,
		 array_stt_vert_[temp_tri.ids[2]].posic - array_stt_vert_[temp_tri.ids[0]].posic);
		for (int i = 0; i < array_control_point_.size(); i++){
			if (DotProduct(tri_nor, array_control_point_[i].vert.posic) > 0){
				count = 0;
				for (int j = 0; j < 3; j++){
					cross_point = LineCrossPlane(array_stt_vert_[temp_tri.ids[j]].posic, tri_nor, array_control_point_[i].vert.posic);
					if (DotProduct(tri_nor,
						CrossProduct(array_stt_vert_[temp_tri.ids[(j+1)%3]].posic - array_stt_vert_[temp_tri.ids[j]].posic,
						 cross_point - array_stt_vert_[temp_tri.ids[j]].posic)) > 0){
						count++;
					}
				}

				if (count == 3 && array_control_point_[i].max_depth >= node_depth && node_resolution >= array_control_point_[i].minimal_resolution){
					node->tri->physic_group = array_control_point_[i].physic_group;
					return 1;
				}
			}
		}
		return 0;
	}
}