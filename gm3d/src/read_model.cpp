#include "gm3d.h"

int GM3D::ReadModel(char* filename,char* forward_model_name){
	int temp_int,ele_type,attri_num,temp_attri,temp_id;
	double temp_val;
	_1dArray temp_model;
	cpoint temp_vert;
	cube temp_cu;
	string temp_str;
	stringstream temp_ss;

	ifstream mshin;
	if (open_infile(mshin,filename)) return -1; //检查并打开模型文件
	while(getline(mshin,temp_str)){
		//读入模型空间顶点集 msh文件版本为2.2
		if (temp_str == "$Nodes"){
			getline(mshin,temp_str);
			temp_ss = str2ss(temp_str);
			temp_ss >> vert_num_; //第一个数为顶点的个数
			model_vert_.resize(vert_num_); //开辟空间
			for (int i = 0; i < vert_num_; i++){
				getline(mshin,temp_str);
				temp_ss = str2ss(temp_str);
				temp_ss >> temp_vert.id >> temp_vert.x >> temp_vert.y >> temp_vert.z;
				model_vert_[i] = temp_vert;
			}
		}
		//读入模型空间单元体
		else if (temp_str == "$Elements"){
			getline(mshin,temp_str);
			temp_ss = str2ss(temp_str);
			temp_ss >> model_num_; //第一个数为总元素的个数 包含了所有类型的元素 比如三角形 四边形 块体等
			model_cube_.resize(model_num_);
			for (int i = 0; i < model_num_; i++){
				getline(mshin,temp_str);
				temp_ss = str2ss(temp_str);
				temp_ss >> temp_cu.cen.id >> ele_type;
				//只读入块体
				if (ele_type == 5){
					temp_ss >> attri_num; //跳过模型单元的几何组与物理组等信息 以后可能会有用
					for (int a = 0; a < attri_num; a++)
						temp_ss >> temp_attri;
					for (int a = 0; a < 8; a++)
						temp_ss >> temp_cu.ids[a];
					model_cube_[i] = temp_cu;
				}
			}
		}
		else continue; //不能识别的单元都被忽略了
	}
	mshin.close();

	//第二次读入模型文件 初始化模型单元属性
	if (open_infile(mshin,filename)) return -1; //检查并打开模型文件
	while(getline(mshin,temp_str)){
		//读入模型单元属性 注意因为msh文件中$ElementData并未注明所属元素类型
		//所以可能会将其他元素类型的属性值也读入 但因为其在pyIdMap中并未注册 所以属性值会全为0 在后续使用时我们需要通过名称辨别
		if (temp_str == "$ElementData"){
			temp_model.resize(model_num_,0.0); //初始化temp_model 为读入模型单元属性做准备
			for (int i = 0; i < 2; i++) //先读入元素块的名称 添加到数组
				getline(mshin,temp_str);
			input_model_names_.push_back(temp_str);
			for (int i = 0; i < 6; i++) //跳过元素属性前面的值 最后一次为当前元素块的个数
				getline(mshin,temp_str);
			temp_ss = str2ss(temp_str);
			temp_ss >> temp_int;
			for (int i = 0; i < temp_int; i++){
				getline(mshin,temp_str);
				temp_ss = str2ss(temp_str);
				temp_ss >> temp_id >> temp_val; //读入单元体索引与属性值
				temp_model[temp_id] = temp_val;
			}
			input_models_.push_back(temp_model);
			temp_model.clear();
		}
		else continue; //不能识别的单元都被忽略了
	}
	mshin.close();

	//清理映射
	temp_model.clear();
	vector <double>().swap(temp_model);

	//初始化数组
	forward_model_.resize(model_num_,0.0);
	//拷贝数组
	for (int i = 0; i < input_model_names_.size(); i++){
		if (!strcmp(input_model_names_[i].c_str(),forward_model_name)){
			forward_model_ = input_models_[i];
			clog << "forward model initialized from file." << endl;
		}
	}

	//计算块体的中心位置和尺寸
	cpoint corner[8];
	for (int i = 0; i < model_num_; i++){
		for (int j = 0; j < 8; j++){
			corner[j] = model_vert_[model_cube_[i].ids[j]];
		}

		model_cube_[i].cen.x = 0.5*(corner[0].x + corner[6].x);
		model_cube_[i].cen.y = 0.5*(corner[0].y + corner[6].y);
		model_cube_[i].cen.z = 0.5*(corner[0].z + corner[6].z);
		model_cube_[i].dx = fabs(corner[6].x - corner[0].x);
		model_cube_[i].dy = fabs(corner[6].y - corner[0].y);
		model_cube_[i].dz = fabs(corner[6].z - corner[0].z);
	}

	for (int i = 0; i < input_models_.size(); i++){
		input_models_[i].clear();
		vector <double>().swap(input_models_[i]);
	}
	input_models_.clear();
	vector < vector<double> >().swap(input_models_);

	input_model_names_.clear();
	vector <string>().swap(input_model_names_);
	return 0;
}