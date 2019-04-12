#include "gm3d.h"

int GM3D::ForwardGy(char* noise_level){
	int i,j;
	double x1,x2,y1,y2,z1,z2;
	double R222,R122,R212,R112,R221,R121,R211,R111;
	double G222,G122,G212,G112,G221,G121,G211,G111;

	double noise_mean, noise_dev, temp_noise;
	if (2 != sscanf(noise_level,"%lf/%lf",&noise_mean,&noise_dev)){
		noise_mean = noise_dev = 0.0;
	}

	//添加高斯噪声值
	default_random_engine generator;
	normal_distribution<double> dist(noise_mean, noise_dev);

	_1dArray temp_obs(model_num_);

	ProgressBar *bar = new ProgressBar(obs_num_,"Forward modeling");
	for (i = 0; i < obs_num_; i++){
		bar->Progressed(i);
#pragma omp parallel for private(j,R222,R122,R212,R112,R221,R121,R211,R111,G222,G122,G212,G112,G221,G121,G211,G111,x1,x2,y1,y2,z1,z2) shared(i) schedule(guided)
		for (j = 0; j < model_num_; j++){
			if (fabs(forward_model_[j]) > ZERO){
				x1 = model_cube_[j].cen.x - 0.5*model_cube_[j].dx; x2 = model_cube_[j].cen.x + 0.5*model_cube_[j].dx;
				y1 = model_cube_[j].cen.y - 0.5*model_cube_[j].dy; y2 = model_cube_[j].cen.y + 0.5*model_cube_[j].dy;
				z1 = model_cube_[j].cen.z - 0.5*model_cube_[j].dz; z2 = model_cube_[j].cen.z + 0.5*model_cube_[j].dz;

				R222=sqrt((x2-grav_obs_[i].x)*(x2-grav_obs_[i].x)+(y2-grav_obs_[i].y)*(y2-grav_obs_[i].y)+(z2-grav_obs_[i].z)*(z2-grav_obs_[i].z));
				R122=sqrt((x1-grav_obs_[i].x)*(x1-grav_obs_[i].x)+(y2-grav_obs_[i].y)*(y2-grav_obs_[i].y)+(z2-grav_obs_[i].z)*(z2-grav_obs_[i].z));
				R212=sqrt((x2-grav_obs_[i].x)*(x2-grav_obs_[i].x)+(y1-grav_obs_[i].y)*(y1-grav_obs_[i].y)+(z2-grav_obs_[i].z)*(z2-grav_obs_[i].z));
				R112=sqrt((x1-grav_obs_[i].x)*(x1-grav_obs_[i].x)+(y1-grav_obs_[i].y)*(y1-grav_obs_[i].y)+(z2-grav_obs_[i].z)*(z2-grav_obs_[i].z));
				R221=sqrt((x2-grav_obs_[i].x)*(x2-grav_obs_[i].x)+(y2-grav_obs_[i].y)*(y2-grav_obs_[i].y)+(z1-grav_obs_[i].z)*(z1-grav_obs_[i].z));
				R121=sqrt((x1-grav_obs_[i].x)*(x1-grav_obs_[i].x)+(y2-grav_obs_[i].y)*(y2-grav_obs_[i].y)+(z1-grav_obs_[i].z)*(z1-grav_obs_[i].z));
				R211=sqrt((x2-grav_obs_[i].x)*(x2-grav_obs_[i].x)+(y1-grav_obs_[i].y)*(y1-grav_obs_[i].y)+(z1-grav_obs_[i].z)*(z1-grav_obs_[i].z));
				R111=sqrt((x1-grav_obs_[i].x)*(x1-grav_obs_[i].x)+(y1-grav_obs_[i].y)*(y1-grav_obs_[i].y)+(z1-grav_obs_[i].z)*(z1-grav_obs_[i].z));

				G222=log((x2-grav_obs_[i].x)+R222);
				G122=log((x1-grav_obs_[i].x)+R122);
				G212=log((x2-grav_obs_[i].x)+R212);
				G112=log((x1-grav_obs_[i].x)+R112);
				G221=log((x2-grav_obs_[i].x)+R221);
				G121=log((x1-grav_obs_[i].x)+R121);
				G211=log((x2-grav_obs_[i].x)+R211);
				G111=log((x1-grav_obs_[i].x)+R111);

				temp_obs[j] = G0*(G222-G122-G212+G112-G221+G121+G211-G111)*forward_model_[j];
			}
		}

		for (int n = 0; n < model_num_; n++){
			grav_obs_[i].val += temp_obs[n];
		}
	}

	for (int i = 0; i < obs_num_; i++){
		temp_noise = dist(generator);
		grav_obs_[i].val += temp_noise;
		grav_obs_[i].dev = noise_dev;
	}
	return 0;
}