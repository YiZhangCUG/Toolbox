#include "stt_class.h"

int SttGenerator::GetControlLine(char* filename,ControlLineArray& return_line_array)
{
	int count;
	stringstream temp_ss;
	string temp_str;
	Vertex temp_vert;
	ControlLine temp_line;
	ifstream infile;

	if (!strcmp(filename,"NULL")){
		return 0;
	}

	//clear return array
	if (!return_line_array.empty()){
		for (int i = 0; i < return_line_array.size(); i++){
			if (!return_line_array[i].vert.empty()) return_line_array[i].vert.clear();
		}
		return_line_array.clear();
	}

	if (OpenInfile(infile,filename)) return -1;
	else{
		while (getline(infile,temp_str)){
			if (*(temp_str.begin()) == '#' || temp_str == "") continue;
			else{
				if (!temp_line.vert.empty()) temp_line.vert.clear();
				temp_ss = Str2Ss(temp_str);
				temp_ss >> count >> temp_line.max_depth >> temp_line.minimal_resolution >> temp_line.physic_group;
				if (temp_line.max_depth <= 0) temp_line.max_depth = 1e+3; //这里直接给一个很大的深度值 节点深度一定小于这个值
				if (temp_line.minimal_resolution <= 0) temp_line.minimal_resolution = -1.0; //这里直接给成-1
				for (int i = 0; i < count; i++){
					getline(infile,temp_str);
					temp_ss = Str2Ss(temp_str);
					if (temp_ss >> temp_vert.posis.lon >> temp_vert.posis.lat){
						temp_vert.posis.rad = DefaultR;
						temp_vert.id = temp_line.vert.size();
						temp_vert.posic = Sphere2Cartesian(temp_vert.posis);
						temp_line.vert.push_back(temp_vert);
					}
				}
				temp_line.id = return_line_array.size();
				return_line_array.push_back(temp_line);
			}
		}
		infile.close();
	}
	return 0;
}