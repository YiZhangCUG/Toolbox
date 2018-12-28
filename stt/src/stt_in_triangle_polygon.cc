#include "stt_class.h"

int SttGenerator::InTrianglePolygon(QuadTreeNode* node){
	// If no polygon, return false
	if (array_control_polygon_.empty()){
		return 0;
	}
	else{
		int count, point_num, node_depth;
		double node_resolution;
		Cpoint tri_nor;
		Cpoint lineface_nor, edgeface_nor;
		Cpoint intersect[2];
		Cpoint angle_mid;
		Cpoint polygon_mid;
		Cpoint cross_point;

		// copy node triangle index to a local variable triangle
		Triangle node_tri;
		for (int j = 0; j < 3; j++){
			node_tri.ids[j] = node->tri->ids[j];
		}
		// copy node depth to a local variable node_depth
		node_depth = node->depth;
		// calculate the spatial resolution of current node's triangle, which equals to the averaged geocentric angle of the triangle's edges.
		node_resolution = 0;
		for (int i = 0; i < 3; i++){
			node_resolution += acos(DotProduct(array_stt_vert_[node_tri.ids[i]].posic,array_stt_vert_[node_tri.ids[(i+1)%3]].posic)
				/(ModuleLength(array_stt_vert_[node_tri.ids[i]].posic)*ModuleLength(array_stt_vert_[node_tri.ids[(i+1)%3]].posic)));
		}
		node_resolution = node_resolution*60/Pi;
		// calculate the outside normal vector of current node's triangle. The vector does not need to be normalized.
		tri_nor = CrossProduct(array_stt_vert_[node_tri.ids[1]].posic-array_stt_vert_[node_tri.ids[0]].posic,
		 array_stt_vert_[node_tri.ids[2]].posic-array_stt_vert_[node_tri.ids[0]].posic);
		// To begin with, we deal with triangles that might be located on the edge of a polygon.
		for (int i = 0; i < array_control_polygon_.size(); i++){
			point_num = array_control_polygon_[i].vert.size();
			for (int j = 0; j < point_num; j++){
				// Firstly, we determine if any vertex of a polygon locates inside of the node's triangle.
				// The vertex must face the side as the same as the node's triangle.
				if (DotProduct(tri_nor,array_control_polygon_[i].vert[j].posic) > 0){
					count = 0;
					for (int k = 0; k < 3; k++){
						cross_point = LineCrossPlane(array_stt_vert_[node_tri.ids[k]].posic,tri_nor,array_control_polygon_[i].vert[j].posic);
						// See if the intersection point is on the left side of the triangle's edges
						if (DotProduct(tri_nor,
							CrossProduct(array_stt_vert_[node_tri.ids[(k+1)%3]].posic-array_stt_vert_[node_tri.ids[k]].posic,
							 cross_point-array_stt_vert_[node_tri.ids[k]].posic)) > 0){
							count++;
						}
					}
					// if the intersection point is on the left side of all three edges of the triangle, then the point is located inside the triangle
					// Meanwhile, the node depth must be small than or equal to the maximal constraint depth of the polygon,
					// or the node triangle's resolution must be bigger than or equal to the minimal constraint resolution of the polygon.
					if (count == 3 && array_control_polygon_[i].max_depth >= node_depth && node_resolution >= array_control_polygon_[i].minimal_resolution){
						// assign the polygon's physical group to the node's triangle as it is a part of the refined STT
						node->tri->physic_group = array_control_polygon_[i].physic_group;
						return 1;
					}
				}
				// Secondly, we see if and edges of a polygon is intersected with the node's triangle
				// calculate the outside normal vector of a triangle composed by two endpoints of a polygon's edge and the origin.
				lineface_nor = CrossProduct(array_control_polygon_[i].vert[j].posic,array_control_polygon_[i].vert[(j+1)%point_num].posic);
				// calculate the middle vector of two endpoints of a polygon's edge.
				angle_mid = 0.5*(array_control_polygon_[i].vert[j].posic + array_control_polygon_[i].vert[(j+1)%point_num].posic);
				// for edges of the node's triangle
				for (int n = 0; n < 3; n++){
					// calculate the outside normal vector of a triangle composed by two endpoints of a triangle's edge and the origin.
					edgeface_nor = CrossProduct(array_stt_vert_[node_tri.ids[n]].posic,array_stt_vert_[node_tri.ids[(n+1)%3]].posic);
					// exclude the situation that the two edges are parallel to each other.
					if (fabs(DotProduct(lineface_nor,edgeface_nor))/(ModuleLength(lineface_nor)*ModuleLength(edgeface_nor)) != 1.0){
						// two intersection points of the edges are found using the cross product.
						intersect[0] = CrossProduct(lineface_nor,edgeface_nor);
						intersect[1] = CrossProduct(edgeface_nor,lineface_nor);
						for (int k = 0; k < 2; k++){
							// The two edges are intersected with each other if one intersection point is located between the polygon's edge and the triangle's edge.
							// Note that the two edges should be facing the same hemisphere.
							if (DotProduct(CrossProduct(intersect[k],array_stt_vert_[node_tri.ids[n]].posic),CrossProduct(intersect[k],array_stt_vert_[node_tri.ids[(n+1)%3]].posic)) < 0
							 && DotProduct(CrossProduct(intersect[k],array_control_polygon_[i].vert[j].posic),CrossProduct(intersect[k],array_control_polygon_[i].vert[(j+1)%point_num].posic)) < 0
							 && DotProduct(angle_mid,tri_nor) > 0
							 && array_control_polygon_[i].max_depth >= node_depth
							 && node_resolution >= array_control_polygon_[i].minimal_resolution)
							{
								node->tri->physic_group = array_control_polygon_[i].physic_group;
								return 1;
							}
						}
					}
				}
			}
		}

		// Now we found triangles that are inside the polygon.
		for (int i = 0; i < array_control_polygon_.size(); i++){
			point_num = array_control_polygon_[i].vert.size();
			// find the center direction of the polygon
			polygon_mid = CloudCenter(array_control_polygon_[i].vert);
			// calculate number of intersection points for each edge of the triangle against the polygon.
			// If any edge of the triangle is inside the polygon, then the triangle is inside the polygon.
			for (int k = 0; k < 3; k++){
				count = 0;
				edgeface_nor = CrossProduct(array_stt_vert_[node_tri.ids[k]].posic,array_stt_vert_[node_tri.ids[(k+1)%3]].posic);
				for (int j = 0; j < point_num; j++){
					lineface_nor = CrossProduct(array_control_polygon_[i].vert[j].posic,array_control_polygon_[i].vert[(j+1)%point_num].posic);
					angle_mid = 0.5*(array_control_polygon_[i].vert[j].posic + array_control_polygon_[i].vert[(j+1)%point_num].posic);
					// exclude the situation that the two edges are parallel to each other.
					if (fabs(DotProduct(lineface_nor,edgeface_nor))/(ModuleLength(lineface_nor)*ModuleLength(edgeface_nor)) != 1.0){
						intersect[0] = CrossProduct(lineface_nor,edgeface_nor);
						intersect[1] = CrossProduct(edgeface_nor,lineface_nor);
						for (int n = 0; n < 2; n++){
							// The two edges are intersected with each other if one intersection point is located between the polygon's edge and the triangle's edge.
							// The two edges should be facing the same hemisphere.
							// The intersection point is located between an edge of the polygon.
							// Or the beginning point of an polygon's edge is the intersection point (you can choose to use the ending point too, but you should only use one of them).
							// The intersection should be facing the same hemisphere as the polygon's edge.
							// Only one side of the extended trajectory of the triangle's edge should be used.
							if (DotProduct(polygon_mid,tri_nor) > 0
								&& (DotProduct(CrossProduct(intersect[k],array_control_polygon_[i].vert[j].posic),CrossProduct(intersect[k],array_control_polygon_[i].vert[(j+1)%point_num].posic)) < 0
								|| array_control_polygon_[i].vert[j].posic == intersect[n])
								&& DotProduct(angle_mid,intersect[n]) > 0
								&& DotProduct(edgeface_nor,CrossProduct(array_stt_vert_[node_tri.ids[k]].posic,intersect[n])) > 0){
								count++;
							}
						}
					}
				}
				// If the number of intersection is odd. The triangle is inside the polygon.
				if (pow(-1,count) < 0 && array_control_polygon_[i].max_depth >= node_depth && node_resolution >= array_control_polygon_[i].minimal_resolution){
					node->tri->physic_group = array_control_polygon_[i].physic_group;
					return 1;
				}
			}
		}
		// All fails, the triangle is outside the polygon.
		return 0;
	}
}