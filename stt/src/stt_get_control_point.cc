#include "stt_class.h"

int SttGenerator::GetControlPoint(char* filename)
{
	stringstream temp_ss;
	string temp_str;
	ControlPoint one_point;
	ifstream infile;

	if (!strcmp(filename,"NULL")) return 0;

	if (OpenInfile(infile,filename)) return -1;
	else
	{
		while (getline(infile,temp_str))
		{
			if (*(temp_str.begin()) == '#' || temp_str == "") continue;
			else
			{
				temp_ss = Str2Ss(temp_str);
				if (temp_ss >> one_point.vert.posis.lon 
					>> one_point.vert.posis.lat 
					>> one_point.max_depth 
					>> one_point.minimal_resolution 
					>> one_point.physic_group)
				{
					if (one_point.max_depth < 0) one_point.max_depth = 1e+3; //这里直接给一个很大的深度值 节点深度一定小于这个值
					if (one_point.minimal_resolution < 0) one_point.minimal_resolution = -1.0; //这里直接给成-1
					one_point.vert.posis.rad = DefaultR;
					one_point.vert.id = array_control_point_.size();
					one_point.vert.posic = Sphere2Cartesian(one_point.vert.posis);
					array_control_point_.push_back(one_point);
				}
			}
		}
		infile.close();
	}
	return 0;
}