#include "stt_class.h"

int SttGenerator::InTriangleLine(QuadTreeNode* node){
	// If no constraint line, return false
	if (array_control_line_.empty()){
		return 0;
	}
	else{
		int count, node_depth;
		double node_resolution;
		Cpoint tri_nor;
		Cpoint lineface_nor, edgeface_nor;
		Cpoint intersect[2];
		Cpoint angle_mid;
		Cpoint edge_mid;
		Cpoint cross_point;

		Triangle temp_tri;
		for (int i = 0; i < 3; i++){
			temp_tri.ids[i] = node->tri->ids[i];
		}

		node_depth = node->depth;

		node_resolution = 0;
		for (int i = 0; i < 3; i++){
			node_resolution += acos(DotProduct(array_stt_vert_[temp_tri.ids[i]].posic, array_stt_vert_[temp_tri.ids[(i+1)%3]].posic)
				/(ModuleLength(array_stt_vert_[temp_tri.ids[i]].posic) * ModuleLength(array_stt_vert_[temp_tri.ids[(i+1)%3]].posic)));
		}
		node_resolution = node_resolution*60/Pi;

		tri_nor = CrossProduct(array_stt_vert_[temp_tri.ids[1]].posic - array_stt_vert_[temp_tri.ids[0]].posic,
		 array_stt_vert_[temp_tri.ids[2]].posic - array_stt_vert_[temp_tri.ids[0]].posic);

		for (int i = 0; i < array_control_line_.size(); i++){
			for (int j = 0; j < array_control_line_[i].vert.size(); j++){
				if (DotProduct(tri_nor,array_control_line_[i].vert[j].posic) > 0){
					count = 0;
					for (int k = 0; k < 3; k++){
						cross_point = LineCrossPlane(array_stt_vert_[temp_tri.ids[k]].posic, tri_nor, array_control_line_[i].vert[j].posic);
						if (DotProduct(tri_nor,
							CrossProduct(array_stt_vert_[temp_tri.ids[(k+1)%3]].posic - array_stt_vert_[temp_tri.ids[k]].posic,
							 cross_point - array_stt_vert_[temp_tri.ids[k]].posic)) > 0){
							count++;
						}
					}

					if (count == 3 && array_control_line_[i].max_depth >= node_depth && node_resolution >= array_control_line_[i].minimal_resolution){
						node->tri->physic_group = array_control_line_[i].physic_group;
						return 1;
					}
				}
			}
		}

		for (int i = 0; i < array_control_line_.size(); i++){
			for (int j = 0; j < array_control_line_[i].vert.size()-1; j++){
				lineface_nor = CrossProduct(array_control_line_[i].vert[j].posic, array_control_line_[i].vert[j+1].posic);
				angle_mid = 0.5*(array_control_line_[i].vert[j].posic + array_control_line_[i].vert[j+1].posic);
				for (int n = 0; n < 3; n++){
					edgeface_nor = CrossProduct(array_stt_vert_[temp_tri.ids[n]].posic, array_stt_vert_[temp_tri.ids[(n+1)%3]].posic);
					edge_mid = 0.5*(array_stt_vert_[temp_tri.ids[n]].posic + array_stt_vert_[temp_tri.ids[(n+1)%3]].posic);

					if (fabs(DotProduct(lineface_nor,edgeface_nor))/(ModuleLength(lineface_nor) * ModuleLength(edgeface_nor)) != 1.0){
						intersect[0] = CrossProduct(lineface_nor,edgeface_nor);
						intersect[1] = CrossProduct(edgeface_nor,lineface_nor);
						for (int k = 0; k < 2; k++){
							if (DotProduct(angle_mid,tri_nor) > 0
							 && DotProduct(CrossProduct(intersect[k],array_stt_vert_[temp_tri.ids[n]].posic),CrossProduct(intersect[k],array_stt_vert_[temp_tri.ids[(n+1)%3]].posic)) < 0
							 && DotProduct(CrossProduct(intersect[k],array_control_line_[i].vert[j].posic),CrossProduct(intersect[k],array_control_line_[i].vert[j+1].posic)) < 0
							 && DotProduct(intersect[k],angle_mid) > 0
							 && DotProduct(intersect[k],edge_mid) > 0
							 && array_control_line_[i].max_depth >= node_depth
							 && node_resolution >= array_control_line_[i].minimal_resolution){
								node->tri->physic_group = array_control_line_[i].physic_group;
								return 1;
							}
						}
					}
				}
			}
		}
		return 0;
	}
}