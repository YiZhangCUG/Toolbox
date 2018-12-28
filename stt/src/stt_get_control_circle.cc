#include "stt_class.h"

int SttGenerator::GetControlCircle(char* filename)
{
	double node_eleva;
	stringstream temp_ss;
	string temp_str;
	ControlCircle temp_circle;
	ifstream infile;

	if (!strcmp(filename,"NULL")){
		return 0;
	}

	if (OpenInfile(infile,filename)) return -1;
	else{
		while (getline(infile,temp_str)){
			if (*(temp_str.begin()) == '#' || temp_str == "") continue;
			else{
				temp_ss = Str2Ss(temp_str);
				if (temp_ss >> temp_circle.circle_center.posis.lon >> temp_circle.circle_center.posis.lat >> temp_circle.circle_cap_degree
				 >> temp_circle.max_depth >> temp_circle.minimal_resolution >> temp_circle.physic_group)
				{
					if (temp_circle.max_depth < 0) temp_circle.max_depth = 1e+3; //这里直接给一个很大的深度值 节点深度一定小于这个值
					if (temp_circle.minimal_resolution < 0) temp_circle.minimal_resolution = -1.0; //这里直接给成-1
					temp_circle.circle_center.posis.rad = DefaultR;
					temp_circle.circle_center.id = array_control_circle_.size();
					temp_circle.circle_center.posic = Sphere2Cartesian(temp_circle.circle_center.posis);
					array_control_circle_.push_back(temp_circle);
				}
			}
		}
		infile.close();
	}
	return 0;
}