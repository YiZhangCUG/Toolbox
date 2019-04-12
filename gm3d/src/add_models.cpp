#include "gm3d.h"

int GM3D::AddModels(char* filename){
	string temp_str;
	stringstream temp_ss;
	modelist temp_list;

	ifstream infile;
	if (open_infile(infile,filename)) return -1;
	while(getline(infile,temp_str)){
		if (*(temp_str.begin()) == '#') continue;
		else{
			//按每行5个数据解析 初始化为含观测值与不确定度的观测点
			if (4 == sscanf(temp_str.c_str(),"%s %s %lf %s",
				temp_list.mod_type,temp_list.val_type,&temp_list.mod_value,temp_list.mod_para)){
				model_list_.push_back(temp_list);
			}
			else{
				cerr << BOLDYELLOW << "ignored ==> " << RESET << "wrong input: " << temp_str << endl;
				continue;
			}
		}
	}
	infile.close();

	for (int i = 0; i < model_list_.size(); i++){
		if (!strcmp(model_list_[i].mod_type,"regular_block")){
			AddRegularBlock(model_list_[i]);
		}
		else if (!strcmp(model_list_[i].mod_type,"tilted_block")){
			AddTiltedBlock(model_list_[i]);
		}
		else if (!strcmp(model_list_[i].mod_type,"sphere")){
			AddSphereBlock(model_list_[i]);
		}
		else if (!strcmp(model_list_[i].mod_type,"interface")){
			AddInterfaceBlock(model_list_[i]);
		}
		else{
			cerr << BOLDYELLOW << "ignored ==> " << RESET << "unknown model type: " << model_list_[i].mod_type << endl;
			continue;
		}
	}
	return 0;
}