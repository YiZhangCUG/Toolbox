#include "gm3d.h"
//我们读入一个界面数据 插值计算每个块体中心位置的值 然后按情况赋值
int GM3D::AddInterfaceBlock(modelist para_list){
	int m, n, xnum, ynum;
	double xs,xe,xmin,xmax,dx;
	double ys,ye,ymin,ymax,dy;
	bool model_added = false;

	string temp_str;
	stringstream temp_ss;

	cpoint temp_topo;
	cpointArray face_topo;

	ifstream infile;
	char filename[1024];
	if (1 == sscanf(para_list.mod_para,"%s",filename)){
		if (open_infile(infile,filename)) return -1;

		while (getline(infile,temp_str)){
			//#range必须出现在数据之前
			if (*(temp_str.begin()) == '#'){
				if (6 == sscanf(temp_str.c_str(),"# range=%lf/%lf/%lf/%lf/%lf/%lf",&xs,&dx,&xe,&ys,&dy,&ye)){
					xmin = MIN(xs,xe); xmax = MAX(xs,xe);
					ymin = MIN(ys,ye); ymax = MAX(ys,ye);
					dx = fabs(dx); dy = fabs(dy);
					xnum = round((xmax - xmin)/dx) + 1;
					ynum = round((ymax - ymin)/dy) + 1;
				}
				else continue;
			}
			else{
				temp_ss = str2ss(temp_str);
				temp_ss >> temp_topo.x >> temp_topo.y >> temp_topo.z;
				face_topo.push_back(temp_topo);
			}
		}
		infile.close();

		if (!strcmp(para_list.val_type,"replace/top")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z >= temp_topo.z){
					model_block_val_[i] = para_list.mod_value;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"replace/bot")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z <= temp_topo.z){
					model_block_val_[i] = para_list.mod_value;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"add/top")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z >= temp_topo.z){
					if (model_block_val_[i] == BDL_MAX)
						model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					else
						model_block_val_[i] += para_list.mod_value;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"add/bot")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z <= temp_topo.z){
					if (model_block_val_[i] == BDL_MAX)
						model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					else
						model_block_val_[i] += para_list.mod_value;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"erase/top")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z >= temp_topo.z){
					model_block_val_[i] = BDL_MAX;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"erase/bot")){
			for (int i = 0; i < model_num_; i++){
				m = floor((model_cube_[i].cen.x - xmin)/dx);
				n = floor((model_cube_[i].cen.y - ymin)/dy);

				temp_topo.z = grid_interpolate(xmin+dx*m,ymin+dy*n,dx,dy,model_cube_[i].cen.x,model_cube_[i].cen.y,
											face_topo[xnum*n+m].z,face_topo[xnum*n+m+1].z,face_topo[xnum*(n+1)+m].z,face_topo[xnum*(n+1)+m+1].z);

				if (model_cube_[i].cen.z <= temp_topo.z){
					model_block_val_[i] = BDL_MAX;
					model_added = true;
				}
			}
		}
		else{
			cerr << BOLDRED << "error ==> " << RESET << "wrong value type: " << para_list.val_type << " of the model type: " << para_list.mod_type << endl;
			return -1;
		}
	}
	else{
		cerr << BOLDRED << "error ==> " << RESET << "fail to add blocks with the parameter: " << para_list.mod_para << endl;
		return -1;
	}

	if (!model_added){
		cerr << BOLDYELLOW << "warning ==> " << RESET << "no block changed with the parameter: " << para_list.mod_para << endl;
		return -1;
	}
	return 0;
}