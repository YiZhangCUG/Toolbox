#include "gm3d.h"

int GM3D::OutObs(char* filename){
	ofstream outfile;
	if (open_outfile(outfile,filename)) return -1;
	outfile << "# x(m) y(m) ele(m) obs-val(mGal|Eo) obs-dev(mGal|Eo)" << endl;
	for (int i = 0; i < obs_num_; i++){
		outfile << grav_obs_[i].x << " " << grav_obs_[i].y << " " << -1.0*grav_obs_[i].z << " "
		 << setprecision(16) << grav_obs_[i].val << " "  << grav_obs_[i].dev << endl;
	}
	outfile.close();
	return 0;
}