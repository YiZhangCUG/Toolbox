#include "gm3d.h"

int GM3D::AddTiltedBlock(modelist para_list){
	double xs_1,xe_1,xmin_1,xmax_1;
	double xs_2,xe_2,xmin_2,xmax_2;
	double ys_1,ye_1,ymin_1,ymax_1;
	double ys_2,ye_2,ymin_2,ymax_2;
	double layer_xmin,layer_xmax,layer_ymin,layer_ymax;
	double zs,ze,zmin,zmax;
	bool model_added = false;

	if (10 == sscanf(para_list.mod_para,"%lf/%lf/%lf/%lf/%lf/%lf/%lf/%lf/%lf/%lf",
		&xs_1,&xe_1,&ys_1,&ye_1,&zs,&xs_2,&xe_2,&ys_2,&ye_2,&ze)){
		xmin_1 = MIN(xs_1,xe_1); xmax_1 = MAX(xs_1,xe_1);
		ymin_1 = MIN(ys_1,ye_1); ymax_1 = MAX(ys_1,ye_1);
		xmin_2 = MIN(xs_2,xe_2); xmax_2 = MAX(xs_2,xe_2);
		ymin_2 = MIN(ys_2,ye_2); ymax_2 = MAX(ys_2,ye_2);
		zmin = MIN(zs,ze); zmax = MAX(zs,ze);

		if (!strcmp(para_list.val_type,"replace")){
			for (int i = 0; i < model_num_; i++){
				//计算当前层的x与y范围
				layer_xmin = (model_cube_[i].cen.z - zmin)*(xmin_2 - xmin_1)/(zmax - zmin) + xmin_1;
				layer_xmax = (model_cube_[i].cen.z - zmin)*(xmax_2 - xmax_1)/(zmax - zmin) + xmax_1;
				layer_ymin = (model_cube_[i].cen.z - zmin)*(ymin_2 - ymin_1)/(zmax - zmin) + ymin_1;
				layer_ymax = (model_cube_[i].cen.z - zmin)*(ymax_2 - ymax_1)/(zmax - zmin) + ymax_1;

				if (model_cube_[i].cen.x >= layer_xmin && model_cube_[i].cen.x <= layer_xmax &&
					model_cube_[i].cen.y >= layer_ymin && model_cube_[i].cen.y <= layer_ymax &&
					model_cube_[i].cen.z >= zmin && model_cube_[i].cen.z <= zmax){
					model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"add")){
			for (int i = 0; i < model_num_; i++){
				//计算当前层的x与y范围
				layer_xmin = (model_cube_[i].cen.z - zmin)*(xmin_2 - xmin_1)/(zmax - zmin) + xmin_1;
				layer_xmax = (model_cube_[i].cen.z - zmin)*(xmax_2 - xmax_1)/(zmax - zmin) + xmax_1;
				layer_ymin = (model_cube_[i].cen.z - zmin)*(ymin_2 - ymin_1)/(zmax - zmin) + ymin_1;
				layer_ymax = (model_cube_[i].cen.z - zmin)*(ymax_2 - ymax_1)/(zmax - zmin) + ymax_1;

				if (model_cube_[i].cen.x >= layer_xmin && model_cube_[i].cen.x <= layer_xmax &&
					model_cube_[i].cen.y >= layer_ymin && model_cube_[i].cen.y <= layer_ymax &&
					model_cube_[i].cen.z >= zmin && model_cube_[i].cen.z <= zmax){
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
				//计算当前层的x与y范围
				layer_xmin = (model_cube_[i].cen.z - zmin)*(xmin_2 - xmin_1)/(zmax - zmin) + xmin_1;
				layer_xmax = (model_cube_[i].cen.z - zmin)*(xmax_2 - xmax_1)/(zmax - zmin) + xmax_1;
				layer_ymin = (model_cube_[i].cen.z - zmin)*(ymin_2 - ymin_1)/(zmax - zmin) + ymin_1;
				layer_ymax = (model_cube_[i].cen.z - zmin)*(ymax_2 - ymax_1)/(zmax - zmin) + ymax_1;

				if (model_cube_[i].cen.x >= layer_xmin && model_cube_[i].cen.x <= layer_xmax &&
					model_cube_[i].cen.y >= layer_ymin && model_cube_[i].cen.y <= layer_ymax &&
					model_cube_[i].cen.z >= zmin && model_cube_[i].cen.z <= zmax){
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