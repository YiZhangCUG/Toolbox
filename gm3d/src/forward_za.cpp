#include "gm3d.h"

int GM3D::ForwardZa(char* noise_level,char* mag_para){
	int i,j;
	double I0,A0,I,A;
	double Alpha,Beta,Gamma;
	double x1,x2,y1,y2,z1,z2;
	double w111,w112,w121,w122,w211,w212,w221,w222;
	double R222,R122,R212,R112,R221,R121,R211,R111;
	double G222,G122,G212,G112,G221,G121,G211,G111;

	//确定噪声水平
	double noise_mean, noise_dev, temp_noise;
	if (2 != sscanf(noise_level,"%lf/%lf",&noise_mean,&noise_dev)){
		noise_mean = noise_dev = 0.0;
	}

	//确定磁化参数
	if (4 != sscanf(mag_para,"%lf/%lf/%lf/%lf",&I0,&A0,&I,&A)){
		I = 0; A = 90;
	}
	else{
		I=I*Pi/180;
		A=A*Pi/180;

		Alpha=cos(I)*cos(A);
		Beta=cos(I)*sin(A);
		Gamma=sin(I);
	}

	//添加高斯噪声值
	default_random_engine generator;
	normal_distribution<double> dist(noise_mean, noise_dev);

	_1dArray temp_obs(model_num_);

	ProgressBar *bar = new ProgressBar(obs_num_,"Forward modeling");
	for (i = 0; i < obs_num_; i++){
		bar->Progressed(i);
#pragma omp parallel for private(j,R222,R122,R212,R112,R221,R121,R211,R111,\
		G222,G122,G212,G112,G221,G121,G211,G111,\
		w111,w112,w121,w122,w211,w212,w221,w222,\
		x1,x2,y1,y2,z1,z2) shared(i) schedule(guided)
		for (j = 0; j < model_num_; j++){
			if (fabs(forward_model_[j]) > ZERO){
				x1 = model_cube_[j].cen.x - 0.5*model_cube_[j].dx; x2 = model_cube_[j].cen.x + 0.5*model_cube_[j].dx;
				y1 = model_cube_[j].cen.y - 0.5*model_cube_[j].dy; y2 = model_cube_[j].cen.y + 0.5*model_cube_[j].dy;
				z1 = model_cube_[j].cen.z - 0.5*model_cube_[j].dz; z2 = model_cube_[j].cen.z + 0.5*model_cube_[j].dz;

				R222=sqrt((x2-obs_p_[i].x)*(x2-obs_p_[i].x)+(y2-obs_p_[i].y)*(y2-obs_p_[i].y)+(z2-obs_p_[i].z)*(z2-obs_p_[i].z));
				R122=sqrt((x1-obs_p_[i].x)*(x1-obs_p_[i].x)+(y2-obs_p_[i].y)*(y2-obs_p_[i].y)+(z2-obs_p_[i].z)*(z2-obs_p_[i].z));
				R212=sqrt((x2-obs_p_[i].x)*(x2-obs_p_[i].x)+(y1-obs_p_[i].y)*(y1-obs_p_[i].y)+(z2-obs_p_[i].z)*(z2-obs_p_[i].z));
				R112=sqrt((x1-obs_p_[i].x)*(x1-obs_p_[i].x)+(y1-obs_p_[i].y)*(y1-obs_p_[i].y)+(z2-obs_p_[i].z)*(z2-obs_p_[i].z));
				R221=sqrt((x2-obs_p_[i].x)*(x2-obs_p_[i].x)+(y2-obs_p_[i].y)*(y2-obs_p_[i].y)+(z1-obs_p_[i].z)*(z1-obs_p_[i].z));
				R121=sqrt((x1-obs_p_[i].x)*(x1-obs_p_[i].x)+(y2-obs_p_[i].y)*(y2-obs_p_[i].y)+(z1-obs_p_[i].z)*(z1-obs_p_[i].z));
				R211=sqrt((x2-obs_p_[i].x)*(x2-obs_p_[i].x)+(y1-obs_p_[i].y)*(y1-obs_p_[i].y)+(z1-obs_p_[i].z)*(z1-obs_p_[i].z));
				R111=sqrt((x1-obs_p_[i].x)*(x1-obs_p_[i].x)+(y1-obs_p_[i].y)*(y1-obs_p_[i].y)+(z1-obs_p_[i].z)*(z1-obs_p_[i].z));

				w222=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R222*(z2-obs_p_[i].z));
				w122=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R122*(z2-obs_p_[i].z));
				w212=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R212*(z2-obs_p_[i].z));
				w112=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R112*(z2-obs_p_[i].z));
				w221=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R221*(z1-obs_p_[i].z));
				w121=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R121*(z1-obs_p_[i].z));
				w211=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R211*(z1-obs_p_[i].z));
				w111=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R111*(z1-obs_p_[i].z));
				
				G222=-Gamma*arctg(w222)+Alpha*log(R222+y2-obs_p_[i].y)+Beta*log(R222+x2-obs_p_[i].x);
				G122=-Gamma*arctg(w122)+Alpha*log(R122+y2-obs_p_[i].y)+Beta*log(R122+x1-obs_p_[i].x);
				G212=-Gamma*arctg(w212)+Alpha*log(R212+y1-obs_p_[i].y)+Beta*log(R212+x2-obs_p_[i].x);
				G112=-Gamma*arctg(w112)+Alpha*log(R112+y1-obs_p_[i].y)+Beta*log(R112+x1-obs_p_[i].x);
				G221=-Gamma*arctg(w221)+Alpha*log(R221+y2-obs_p_[i].y)+Beta*log(R221+x2-obs_p_[i].x);
				G121=-Gamma*arctg(w121)+Alpha*log(R121+y2-obs_p_[i].y)+Beta*log(R121+x1-obs_p_[i].x);
				G211=-Gamma*arctg(w211)+Alpha*log(R211+y1-obs_p_[i].y)+Beta*log(R211+x2-obs_p_[i].x);
				G111=-Gamma*arctg(w111)+Alpha*log(R111+y1-obs_p_[i].y)+Beta*log(R111+x1-obs_p_[i].x);

				temp_obs[j] = T0/(4*Pi)*(G222-G122-G212+G112-G221+G121+G211-G111)*forward_model_[j];
			}
		}

		for (int n = 0; n < model_num_; n++){
			obs_p_[i].val += temp_obs[n];
		}
	}

	for (int i = 0; i < obs_num_; i++){
		temp_noise = dist(generator);
		obs_p_[i].val += temp_noise;
		obs_p_[i].dev = noise_dev;
	}
	return 0;
}