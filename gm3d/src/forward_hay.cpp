#include "gm3d.h"

int GM3D::ForwardHay(char* noise_level,char* mag_para){
	int i,j;
	double I0,A0,I,A;
	double Alpha,Beta,Gamma;
	double x1,x2,y1,y2,z1,z2;
	double w111_1,w112_1,w121_1,w122_1,w211_1,w212_1,w221_1,w222_1;
	double w111_2,w112_2,w121_2,w122_2,w211_2,w212_2,w221_2,w222_2;
	double R222,R122,R212,R112,R221,R121,R211,R111;
	double G222,G122,G212,G112,G221,G121,G211,G111;

	//确定噪声水平
	double noise_mean, noise_dev, temp_noise;
	if (2 != sscanf(noise_level,"%lf/%lf",&noise_mean,&noise_dev)){
		noise_mean = noise_dev = 0.0;
	}

	//确定磁化参数
	if (4 != sscanf(mag_para,"%lf/%lf/%lf/%lf",&I0,&A0,&I,&A)){
		I = 90; A = 0;
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
		w111_1,w112_1,w121_1,w122_1,w211_1,w212_1,w221_1,w222_1,\
		w111_2,w112_2,w121_2,w122_2,w211_2,w212_2,w221_2,w222_2,\
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

				w222_1=(x2-obs_p_[i].x)*(y2-obs_p_[i].y);
				w122_1=(x1-obs_p_[i].x)*(y2-obs_p_[i].y);
				w212_1=(x2-obs_p_[i].x)*(y1-obs_p_[i].y);
				w112_1=(x1-obs_p_[i].x)*(y1-obs_p_[i].y);
				w221_1=(x2-obs_p_[i].x)*(y2-obs_p_[i].y);
				w121_1=(x1-obs_p_[i].x)*(y2-obs_p_[i].y);
				w211_1=(x2-obs_p_[i].x)*(y1-obs_p_[i].y);
				w111_1=(x1-obs_p_[i].x)*(y1-obs_p_[i].y);

				w222_2=(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))+R222*(z2-obs_p_[i].z);
				w122_2=(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))+R122*(z2-obs_p_[i].z);
				w212_2=(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))+R212*(z2-obs_p_[i].z);
				w112_2=(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))+R112*(z2-obs_p_[i].z);
				w221_2=(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))+R221*(z1-obs_p_[i].z);
				w121_2=(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))+R121*(z1-obs_p_[i].z);
				w211_2=(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))+R211*(z1-obs_p_[i].z);
				w111_2=(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))+R111*(z1-obs_p_[i].z);
				
				G222=Beta*atan2(w222_1,w222_2)+Alpha*log(R222+z2-obs_p_[i].z)+Gamma*log(R222+x2-obs_p_[i].x);
				G122=Beta*atan2(w122_1,w122_2)+Alpha*log(R122+z2-obs_p_[i].z)+Gamma*log(R122+x1-obs_p_[i].x);
				G212=Beta*atan2(w212_1,w212_2)+Alpha*log(R212+z2-obs_p_[i].z)+Gamma*log(R212+x2-obs_p_[i].x);
				G112=Beta*atan2(w112_1,w112_2)+Alpha*log(R112+z2-obs_p_[i].z)+Gamma*log(R112+x1-obs_p_[i].x);
				G221=Beta*atan2(w221_1,w221_2)+Alpha*log(R221+z1-obs_p_[i].z)+Gamma*log(R221+x2-obs_p_[i].x);
				G121=Beta*atan2(w121_1,w121_2)+Alpha*log(R121+z1-obs_p_[i].z)+Gamma*log(R121+x1-obs_p_[i].x);
				G211=Beta*atan2(w211_1,w211_2)+Alpha*log(R211+z1-obs_p_[i].z)+Gamma*log(R211+x2-obs_p_[i].x);
				G111=Beta*atan2(w111_1,w111_2)+Alpha*log(R111+z1-obs_p_[i].z)+Gamma*log(R111+x1-obs_p_[i].x);

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