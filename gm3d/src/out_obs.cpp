#include "gm3d.h"

int GM3D::OutObs(char* filename){
	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;
	outfile << "# x(m) y(m) ele(m) obs-val(mGal|Eo) obs-dev(mGal|Eo)" << endl;
	for (int i = 0; i < obs_num_; i++){
		outfile << obs_p_[i].y << " " << obs_p_[i].x << " " << -1.0*obs_p_[i].z << " "
		 << setprecision(16) << obs_p_[i].val << " "  << obs_p_[i].dev << endl;
	}
	outfile.close();
	return 0;
}