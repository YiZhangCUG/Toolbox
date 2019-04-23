#ifndef _GM3D_H
#define _GM3D_H
#include "head_func.h"
#include "progress_bar.h"

class GM3D{
public:
	GM3D(){}
	~GM3D(){}
	int BuildRegularGrid(char*); //初始化反演模型空间
	int AddModels(char*); //读取模型块体参数文件
	int AddRegularBlock(modelist); //添加普通模型块体
	int AddTiltedBlock(modelist); //添加倾斜模型块体
	int AddSphereBlock(modelist); //添加球体椭球体块体
	int AddInterfaceBlock(modelist); //添加密度界面
	//模型操作
	int ReadModel(char*,char*);
	//输出模型
	int RegisteredOuput(bool); //注册输出的块体模型
	int OutMshFile(char*,string); //输出模型文件
	int OutNeighborFile(char*,char*); //输出模型块体或顶点的相邻关系 暂缓
	//观测数据
	int InitObs(char*);
	int OutObs(char*);
	//正演函数
	int ForwardVz(char*);
	int ForwardVzx(char*);
	int ForwardVzy(char*);
	int ForwardVzz(char*);
	int ForwardDeltaT(char*,char*);
	int ForwardDeltaTx(char*,char*);
	int ForwardDeltaTy(char*,char*);
	int ForwardDeltaTz(char*,char*);
	int ForwardHax(char*,char*);
	int ForwardHay(char*,char*);
	int ForwardZa(char*,char*);
private:
	int obs_num_, model_num_, vert_num_;
	//正演数组
	obspointArray obs_p_;
	_2dArray input_models_;
	_1sArray input_model_names_;
	_1dArray forward_model_;
	//模型数据
	cubeArray model_cube_;
	cpointArray model_vert_;
	_1dArray model_block_val_;
	modelistArray model_list_;
	_1iArray out_ele_ids_;
	_1iArray out_ele_data_ids_;
	_1iArray out_vert_ids_;
	_int2intMap vert_out_map_;
	_int2intMap ele_data_out_map_;

	_2iArray model_vert_neighbor_; //暂缓
	_2iArray model_cube_neighbor_; //暂缓
};
#endif