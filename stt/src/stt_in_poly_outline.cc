#include "stt_class.h"

int SttGenerator::InPolyOutline(QuadTreeNode* node)
{
	//没有范围多边形 直接返回否
	if (array_hole_polygon_.empty()){
		return 0;
	}
	else{
		int count, pnum;
		Cpoint tri_nor;
		Cpoint lineface_nor, edgeface_nor;
		Cpoint intersect[2];
		Cpoint angle_mid;
		Cpoint polygon_mid;
		Cpoint cross_point;
		Triangle temp_tri;
		for (int j = 0; j < 3; j++){
			temp_tri.ids[j] = node->tri->ids[j];
		}

		//计算三角面元外法线矢量
		tri_nor = CrossProduct(array_stt_vert_[temp_tri.ids[1]].posic-array_stt_vert_[temp_tri.ids[0]].posic,
		 array_stt_vert_[temp_tri.ids[2]].posic-array_stt_vert_[temp_tri.ids[0]].posic);
		//首先判断多边形的顶点是否在当前节点三角形内 或者多边形的边是否与当前节点三角形相交 这些条件可以判断多边形边上的三角形
		for (int i = 0; i < array_hole_polygon_.size(); i++){
			pnum = array_hole_polygon_[i].vert.size();
			for (int j = 0; j < array_hole_polygon_[i].vert.size(); j++){
				//排除球形背面的点
				if (DotProduct(tri_nor,array_hole_polygon_[i].vert[j].posic) > 0){
					//多边形节点在当前节点三角形内
					count = 0;
					for (int k = 0; k < 3; k++){
						cross_point = LineCrossPlane(array_stt_vert_[temp_tri.ids[k%3]].posic,tri_nor,array_hole_polygon_[i].vert[j].posic);
						//依次判断前后两条边与待检测点的外法线是否同向 注意排除从球体背面穿射的情况 全为真则返回真
						if (DotProduct(tri_nor,
							CrossProduct(array_stt_vert_[temp_tri.ids[(k+1)%3]].posic-array_stt_vert_[temp_tri.ids[k%3]].posic,
							 cross_point-array_stt_vert_[temp_tri.ids[k%3]].posic)) > 0)
						{
							count++;
						}
					}
					//全部在左侧 多边形顶点至少有一个在节点三角形内 即节点三角形至少有一个顶点在多边形内 返回真
					if (count == 3) return 1;
				}
				//多边形边与当前节点三角形相交
				lineface_nor = CrossProduct(array_hole_polygon_[i].vert[j%pnum].posic,array_hole_polygon_[i].vert[(j+1)%pnum].posic);
				angle_mid = 0.5*(array_hole_polygon_[i].vert[j%pnum].posic + array_hole_polygon_[i].vert[(j+1)%pnum].posic);
				for (int n = 0; n < 3; n++){
					edgeface_nor = CrossProduct(array_stt_vert_[temp_tri.ids[n%3]].posic,array_stt_vert_[temp_tri.ids[(n+1)%3]].posic);
					//排除两个扇面在同一个平面的情况
					if (fabs(DotProduct(lineface_nor,edgeface_nor))/(ModuleLength(lineface_nor)*ModuleLength(edgeface_nor)) != 1.0){
						//两个扇面可能的交点矢量垂直于两个扇面的外法线矢量 正反两个矢量
						intersect[0] = CrossProduct(lineface_nor,edgeface_nor);
						intersect[1] = CrossProduct(edgeface_nor,lineface_nor);
						for (int k = 0; k < 2; k++){
							//交点矢量在两个线段端点矢量之间 注意端点先后顺序决定了大圆弧在球面上的范围 注意这里同样有从背面穿透的可能 因为我们不确定intersect中哪一个是我们想要的
							//注意计算叉乘的时候 我们总是会走一个角度小于180的方向
							//排除与angle_mid相反的半球上所有的三角形
							if (DotProduct(CrossProduct(intersect[k],array_stt_vert_[temp_tri.ids[n%3]].posic),CrossProduct(intersect[k],array_stt_vert_[temp_tri.ids[(n+1)%3]].posic)) < 0
							 && DotProduct(CrossProduct(intersect[k],array_hole_polygon_[i].vert[j%pnum].posic),CrossProduct(intersect[k],array_hole_polygon_[i].vert[(j+1)%pnum].posic)) < 0
							 && DotProduct(angle_mid,tri_nor) > 0)
							{
								//多边形边与节点三角形相交 即节点三角形至少有一个顶点在多边形内 返回真
								return 1;
							}
						}
					}
				}
			}
		}
		//多边形的顶点和边与当前节点三角形不相交或者包含 判断三角形是否在多边形内
		for (int i = 0; i < array_hole_polygon_.size(); i++){
			pnum = array_hole_polygon_[i].vert.size();
			polygon_mid = CloudCenter(array_hole_polygon_[i].vert);
			//依次判断节点三角形的三条边与多边形边的交点个数
			for (int k = 0; k < 3; k++){
				count = 0;
				//计算三角形边与球心的平面的法线矢量 只要任意一条边在多边形内 则三角形在多边形内
				edgeface_nor = CrossProduct(array_stt_vert_[temp_tri.ids[(k)%3]].posic,array_stt_vert_[temp_tri.ids[(k+1)%3]].posic);
				for (int j = 0; j < array_hole_polygon_[i].vert.size(); j++){
					//多边形边与当前节点三角形相交
					lineface_nor = CrossProduct(array_hole_polygon_[i].vert[j%pnum].posic,array_hole_polygon_[i].vert[(j+1)%pnum].posic);
					angle_mid = 0.5*(array_hole_polygon_[i].vert[j%pnum].posic + array_hole_polygon_[i].vert[(j+1)%pnum].posic);
					//排除两个扇面在同一个平面的情况
					if (fabs(DotProduct(lineface_nor,edgeface_nor))/(ModuleLength(lineface_nor)*ModuleLength(edgeface_nor)) != 1.0){
						//两个扇面可能的交点矢量垂直于两个扇面的外法线矢量 正反两个矢量
						intersect[0] = CrossProduct(lineface_nor,edgeface_nor);
						intersect[1] = CrossProduct(edgeface_nor,lineface_nor);
						for (int n = 0; n < 2; n++){
							/*注意 这里我们只判断交点是否在线段之间 或者一个点上 这里选择第一个点也可以选择第二点 但只能包含一个 不判断是不是在边之间
							注意我们只统计从当前三角形顶点向左或者向右旋转180度中遇到的交点*/
							//交点矢量在两个线段端点矢量之间 注意端点先后顺序决定了大圆弧在球面上的范围 注意这里同样有从背面穿透的可能 因为我们不确定intersect中哪一个是我们想要的
							//注意计算叉乘的时候 我们总是会走一个角度小于180的方向
							//排除与angle_mid相反的半球上所有的三角形
							if (DotProduct(polygon_mid,tri_nor) > 0 //排除位于球背面的三角形
								&& (DotProduct(CrossProduct(intersect[k],array_hole_polygon_[i].vert[j%pnum].posic),CrossProduct(intersect[k],array_hole_polygon_[i].vert[(j+1)%pnum].posic)) < 0
								|| array_hole_polygon_[i].vert[j].posic == intersect[n]) //排除与多边形的边不相交的三角形边的延长线 这里包含了一个等于条件 即交点刚好在多边形的顶点上
								&& DotProduct(angle_mid,intersect[n]) > 0 //排除位于球背面的多边形边与三角形边延长线的交点
								&& DotProduct(edgeface_nor,CrossProduct(array_stt_vert_[temp_tri.ids[k%3]].posic,intersect[n])) > 0) //只取三角形边其中一则的延长线
							{
								count++;
							}
						}
					}
				}
				//交点个数为奇数 边在多边形内 返回真
				if (pow(-1,count) < 0) return 1;
			}
		}
		//全不为真 返回假
		return 0;
	}
}