#include "stt_class.h"

int SttGenerator::InTriangleCircle(QuadTreeNode* node){
	// If no constraint circle, return false
	if (array_control_circle_.empty()){
		return 0;
	}
	else{
		int node_depth;
		double node_resolution, center_degree;

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

		for (int i = 0; i < array_control_circle_.size(); i++){
			for (int j = 0; j < 3; j++){
				// calculate the geocentric angle between a vertex of the triangle and the center of the circle
				center_degree = acos(DotProduct(array_control_circle_[i].circle_center.posic,array_stt_vert_[temp_tri.ids[j]].posic)
					/(ModuleLength(array_control_circle_[i].circle_center.posic)*ModuleLength(array_stt_vert_[temp_tri.ids[j]].posic)))*180.0/Pi;

				if (center_degree <= array_control_circle_[i].circle_cap_degree
					&& array_control_circle_[i].max_depth >= node_depth && node_resolution >= array_control_circle_[i].minimal_resolution){
					node->tri->physic_group = array_control_circle_[i].physic_group;
					return 1;
				}
			}
		}
		return 0;
	}
}