#include "gm3d.h"

int GM3D::OutMshFile(char* filename,string data_name){
	if (!strcmp(filename,"NULL")) return 0;

	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;

	//好啦 我们这里输出的模型类型应该是块体
	outfile<<"$MeshFormat"<<endl<<"2.2 0 8"<<endl<<"$EndMeshFormat"<<endl<<"$Nodes"<<endl<< out_vert_ids_.size() <<endl;
	//输出顶点 最后一个不输出
	for (int i = 0; i < out_vert_ids_.size(); i++){
		outfile << i << " " 
		<< setprecision(16) << model_vert_[out_vert_ids_[i]].x << " " 
		<< model_vert_[out_vert_ids_[i]].y << " " 
		<< model_vert_[out_vert_ids_[i]].z << endl;
	}
	outfile<<"$EndNodes"<<endl;
	outfile<<"$Elements"<<endl<< out_ele_ids_.size() <<endl;
	for (int i = 0; i < out_ele_ids_.size(); i++){
		outfile << i <<" 5 1 0";
		for (int j = 0; j < 8; j++) outfile << " " << vert_out_map_[model_cube_[out_ele_ids_[i]].ids[j]];
		outfile << endl;
	}
	outfile << "$EndElements"<< endl;

	if (out_ele_data_ids_.size() > 0){
		outfile<<"$ElementData"<<endl;
		outfile<<1<<endl<<"\""<<data_name<<"\""<<endl<<1<<endl<<0.0<<endl<<3<<endl<<0<<endl<<1<<endl<< out_ele_data_ids_.size() <<endl;
		for (int i = 0; i < out_ele_data_ids_.size(); i++){
			outfile << ele_data_out_map_[i] << " " << setprecision(16) << model_block_val_[out_ele_data_ids_[i]] << endl;
		}
		outfile<<"$EndElementData"<< endl;
	}
	outfile.close();
	return 0;
}