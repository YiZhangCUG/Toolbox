#include "stt_class.h"

int SttGenerator::OutputTriangleCenterLocation(char* filename,double pole_radius,double equator_radius)
{
	time_t now = time(0);
	char* dt = ctime(&now);

	if (!strcmp(filename,"NULL") || !strcmp(filename,""))
		return -1;

	ofstream outfile;
	if(OpenOutfile(outfile,filename)) return -1;

	Vertex temp_vert;
	outfile << "# This file is created by stt-generator.ex on " << dt;
	outfile << "# Commands: " << command_record_ << endl;
	outfile << "# Vertex number: "<< array_out_tri_pointer_.size() << endl;
	outfile << "# x y z longitude latitude radius (meter)" << endl;
	for (int i = 0; i < array_out_tri_pointer_.size(); i++)
	{
		temp_vert.posic = 1.0/3.0*(array_stt_vert_[array_out_tri_pointer_[i]->tri->ids[0]].posic 
			+ array_stt_vert_[array_out_tri_pointer_[i]->tri->ids[1]].posic 
			+ array_stt_vert_[array_out_tri_pointer_[i]->tri->ids[2]].posic);
		temp_vert.posis = Cartesian2Sphere(temp_vert.posic);
		temp_vert.posis.rad = EllipsoidRadius(temp_vert.posis.lat,pole_radius,equator_radius);
		temp_vert.posic = Sphere2Cartesian(temp_vert.posis);
		outfile << setprecision(16) << temp_vert.posic.x << " " << temp_vert.posic.y << " " << temp_vert.posic.z 
		<< " " << temp_vert.posis.lon << " " << temp_vert.posis.lat << " " << temp_vert.posis.rad << endl;
	}
	outfile.close();
	return 0;
}