#include "gm3d.h"

int GM3D::AddSphereBlock(modelist para_list){
	double xc,yc,zc,rad_x,rad_y,rad_z;
	double dist,rad_limit;
	double rel_x,rel_y,rel_z,theta,phi;
	bool model_added = false;

	if (6 == sscanf(para_list.mod_para,"%lf/%lf/%lf/%lf/%lf/%lf",&xc,&yc,&zc,&rad_x,&rad_y,&rad_z)){
		if (!strcmp(para_list.val_type,"replace")){
			for (int i = 0; i < model_num_; i++){
				rel_x = model_cube_[i].cen.x - xc;
				rel_y = model_cube_[i].cen.y - yc;
				rel_z = model_cube_[i].cen.z - zc;
				dist = sqrt(rel_x*rel_x + rel_y*rel_y + rel_z*rel_z);

				theta = acos(rel_z/dist);
				phi = atan2(rel_y,rel_x);

				rad_limit = rad_x*rad_y*rad_z/sqrt(pow(rad_y*rad_z*sin(theta)*cos(phi),2) + pow(rad_x*rad_z*sin(theta)*sin(phi),2) + pow(rad_x*rad_y*cos(theta),2));

				if (dist <= rad_limit){
					model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"add")){
			for (int i = 0; i < model_num_; i++){
				rel_x = model_cube_[i].cen.x - xc;
				rel_y = model_cube_[i].cen.y - yc;
				rel_z = model_cube_[i].cen.z - zc;
				dist = sqrt(rel_x*rel_x + rel_y*rel_y + rel_z*rel_z);

				theta = acos(rel_z/dist);
				phi = atan2(rel_y,rel_x);

				rad_limit = rad_x*rad_y*rad_z/sqrt(pow(rad_y*rad_z*sin(theta)*cos(phi),2) + pow(rad_x*rad_z*sin(theta)*sin(phi),2) + pow(rad_x*rad_y*cos(theta),2));

				if (dist <= rad_limit){
					if (model_block_val_[i] == BDL_MAX)
						model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					else
						model_block_val_[i] += para_list.mod_value;
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"erase")){
			for (int i = 0; i < model_num_; i++){
				rel_x = model_cube_[i].cen.x - xc;
				rel_y = model_cube_[i].cen.y - yc;
				rel_z = model_cube_[i].cen.z - zc;
				dist = sqrt(rel_x*rel_x + rel_y*rel_y + rel_z*rel_z);

				theta = acos(rel_z/dist);
				phi = atan2(rel_y,rel_x);

				rad_limit = rad_x*rad_y*rad_z/sqrt(pow(rad_y*rad_z*sin(theta)*cos(phi),2) + pow(rad_x*rad_z*sin(theta)*sin(phi),2) + pow(rad_x*rad_y*cos(theta),2));

				if (dist <= rad_limit){
					model_block_val_[i] = BDL_MAX; //注意重复赋值的块体会覆盖
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