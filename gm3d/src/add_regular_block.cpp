#include "gm3d.h"

int GM3D::AddRegularBlock(modelist para_list){
	double xs,xe,xmin,xmax;
	double ys,ye,ymin,ymax;
	double zs,ze,zmin,zmax;
	bool model_added = false;

	if (6 == sscanf(para_list.mod_para,"%lf/%lf/%lf/%lf/%lf/%lf",&xs,&xe,&ys,&ye,&zs,&ze)){
		xmin = MIN(xs,xe); xmax = MAX(xs,xe);
		ymin = MIN(ys,ye); ymax = MAX(ys,ye);
		zmin = MIN(zs,ze); zmax = MAX(zs,ze);

		if (!strcmp(para_list.val_type,"replace")){
			for (int i = 0; i < model_num_; i++){
				if (model_cube_[i].cen.x >= xmin && model_cube_[i].cen.x <= xmax &&
					model_cube_[i].cen.y >= ymin && model_cube_[i].cen.y <= ymax &&
					model_cube_[i].cen.z >= zmin && model_cube_[i].cen.z <= zmax){
					model_block_val_[i] = para_list.mod_value; //注意重复赋值的块体会覆盖
					model_added = true;
				}
			}
		}
		else if (!strcmp(para_list.val_type,"add")){
			for (int i = 0; i < model_num_; i++){
				if (model_cube_[i].cen.x >= xmin && model_cube_[i].cen.x <= xmax &&
					model_cube_[i].cen.y >= ymin && model_cube_[i].cen.y <= ymax &&
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
				if (model_cube_[i].cen.x >= xmin && model_cube_[i].cen.x <= xmax &&
					model_cube_[i].cen.y >= ymin && model_cube_[i].cen.y <= ymax &&
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