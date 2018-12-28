#include "stt_class.h"

int SttGenerator::OutputVertexLocation(char* filename,double pole_radius,double equator_radius)
{
	time_t now = time(0);
	char* dt = ctime(&now);
	IntArray1D array_vert_id;

	if (!strcmp(filename,"NULL") || !strcmp(filename,""))
		return -1;

	ofstream outfile;
	if(OpenOutfile(outfile,filename)) return -1;

	vector<int>::iterator pos;
	for (int i = 0; i < array_out_tri_pointer_.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			array_vert_id.push_back(array_out_tri_pointer_[i]->tri->ids[j]);
		}
	}
	sort(array_vert_id.begin(), array_vert_id.end()); //对顶点序列由小到大排序
	pos = unique(array_vert_id.begin(), array_vert_id.end()); //获取重复序列开始的位置
	array_vert_id.erase(pos, array_vert_id.end()); //删除重复顶点序列

	Vertex temp_vert;
	outfile << "# This file is created by stt-generator.ex on " << dt;
	outfile << "# Commands: " << command_record_ << endl;
	outfile << "# Vertex number: "<< array_vert_id.size() << endl;
	outfile << "# x y z longitude latitude radius (meter)" << endl;
	for (int i = 0; i < array_vert_id.size(); i++)
	{
		temp_vert = array_stt_vert_[array_vert_id[i]];
		temp_vert.posis.rad = EllipsoidRadius(temp_vert.posis.lat,pole_radius,equator_radius);
		temp_vert.posic = Sphere2Cartesian(temp_vert.posis);
		outfile << setprecision(16) << temp_vert.posic.x << " " << temp_vert.posic.y << " " << temp_vert.posic.z 
		<< " " << temp_vert.posis.lon << " " << temp_vert.posis.lat << " " << temp_vert.posis.rad << endl;
	}
	outfile.close();
	return 0;
}