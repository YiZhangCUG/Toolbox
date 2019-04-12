#include "gm3d.h"

int GM3D::BuildRegularGrid(char* space_para){
	cpoint temp_cp;
	cube temp_cu;
	string temp_id_str;
	double x,xs,dx,xe,xmin,xmax;
	double y,ys,dy,ye,ymin,ymax;
	double z,zs,dz,ze,zmin,zmax;
	double sign[8][3] = {{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{0.5,0.5,-0.5},{-0.5,0.5,-0.5},
						{-0.5,-0.5,0.5},{0.5,-0.5,0.5},{0.5,0.5,0.5},{-0.5,0.5,0.5}};

	_str2pointMap map_str_point;
	_str2pointMap::iterator imsp;

	if (9 == sscanf(space_para,"%lf/%lf/%lf/%lf/%lf/%lf/%lf/%lf/%lf",
					&xs,&dx,&xe,&ys,&dy,&ye,&zs,&dz,&ze)){
		xmin = MIN(xs,xe); xmax = MAX(xs,xe);
		ymin = MIN(ys,ye); ymax = MAX(ys,ye);
		zmin = MIN(zs,ze); zmax = MAX(zs,ze);

		temp_cu.dx = fabs(dx); temp_cu.dy = fabs(dy); temp_cu.dz = fabs(dz);
		y = ys;
		while(y >= ymin && y <= ymax){
			x = xs;
			while(x >= xmin && x <= xmax){
				z = zs;
				while(z >= zmin && z <= zmax){
					//添加denMod
					temp_cu.cen.id = model_cube_.size();
					temp_cu.cen.x = x; temp_cu.cen.y = y; temp_cu.cen.z = z;
					//添加mshVert
					for (int i = 0; i < 8; i++){
						temp_cp.id = model_vert_.size(); //添加msh的顶点索引为mshVert的大小
						temp_cp.x = temp_cu.cen.x - sign[i][0]*temp_cu.dx; //左下底角
						temp_cp.y = temp_cu.cen.y - sign[i][1]*temp_cu.dy;
						temp_cp.z = temp_cu.cen.z - sign[i][2]*temp_cu.dz;
						temp_id_str = cpoint_id(temp_cp);
						imsp = map_str_point.find(temp_id_str);
						//利用map_vert查到当前顶点是否存在,这里需要注意，如果顶点已经存在则只需要将顶点索引置为已存在顶点的索引，不增加顶点计数
						if(imsp!=map_str_point.end()){
							temp_cu.ids[i] = imsp->second.id;
						}
						//若为新的顶点则将其增加到两个映射和一个链表中
						else{
							temp_cu.ids[i] = temp_cp.id;//新的顶点索引等于顶点集的数量
							model_vert_.push_back(temp_cp);//将新产生的顶点保存到顶点链表中
							map_str_point[temp_id_str] = temp_cp;//将新产生的顶点保存到顶点位置映射中
						}
					}
					model_cube_.push_back(temp_cu);
					z += dz;
				}
				x += dx;
			}
			y += dy;
		}
	}

	if (model_cube_.empty()){
		cerr << BOLDRED << "error ==> " << RESET << "fail to initial model space with the parameter: " << space_para << endl;
		return -1;
	}
	else{
		vert_num_ = model_vert_.size();
		model_num_ = model_cube_.size();
		model_block_val_.resize(model_num_,BDL_MAX); //初始化模型块体值为BDL_MAX
	}
	return 0;
}