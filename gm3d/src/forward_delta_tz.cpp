#include "gm3d.h"

int GM3D::ForwardDeltaTz(char* noise_level,char* mag_para){
	int i,j;
	double I0,A0,I,A;
	double k1,k2,k3,k4,k5,k6;
	double x1,x2,y1,y2,z1,z2;
	double w111,w112,w121,w122,w211,w212,w221,w222;
	double v111,v112,v121,v122,v211,v212,v221,v222;
	double t111,t112,t121,t122,t211,t212,t221,t222;
	double R222,R122,R212,R112,R221,R121,R211,R111;
	double G222,G122,G212,G112,G221,G121,G211,G111;

	//确定噪声水平
	double noise_mean, noise_dev, temp_noise;
	if (2 != sscanf(noise_level,"%lf/%lf",&noise_mean,&noise_dev)){
		noise_mean = noise_dev = 0.0;
	}

	//确定磁化参数
	if (4 != sscanf(noise_level,"%lf/%lf/%lf/%lf",&I0,&A0,&I,&A)){
		I0 = I = 0; A0 = A = 90;
	}
	else{
		I0=I0*Pi/180;
		A0=A0*Pi/180;
		I=I*Pi/180;
		A=A*Pi/180;

		k1=cos(I0)*sin(A0)*sin(I)+sin(I0)*cos(I)*sin(A);
		k2=cos(I0)*cos(A0)*sin(I)+sin(I0)*cos(I)*cos(A);
		k3=cos(I0)*cos(A0)*cos(I)*sin(A)+cos(I0)*sin(A0)*cos(I)*cos(A);
		k4=cos(I0)*cos(A0)*cos(I)*cos(A);
		k5=cos(I0)*sin(A0)*cos(I)*sin(A);
		k6=-sin(I0)*sin(I);
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
		t111,t112,t121,t122,t211,t212,t221,t222,\
		w111,w112,w121,w122,w211,w212,w221,w222,\
		v111,v112,v121,v122,v211,v212,v221,v222,\
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

				w222=((x2-obs_p_[i].x)*(y2-obs_p_[i].y)*(pow(z2-obs_p_[i].z,2)+pow(R222,2)))/(R222*(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))*(pow(x2-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				w122=((x1-obs_p_[i].x)*(y2-obs_p_[i].y)*(pow(z2-obs_p_[i].z,2)+pow(R122,2)))/(R122*(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))*(pow(x1-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				w212=((x2-obs_p_[i].x)*(y1-obs_p_[i].y)*(pow(z2-obs_p_[i].z,2)+pow(R212,2)))/(R212*(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))*(pow(x2-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				w112=((x1-obs_p_[i].x)*(y1-obs_p_[i].y)*(pow(z2-obs_p_[i].z,2)+pow(R112,2)))/(R112*(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2))*(pow(x1-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				w221=((x2-obs_p_[i].x)*(y2-obs_p_[i].y)*(pow(z1-obs_p_[i].z,2)+pow(R221,2)))/(R221*(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))*(pow(x2-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				w121=((x1-obs_p_[i].x)*(y2-obs_p_[i].y)*(pow(z1-obs_p_[i].z,2)+pow(R121,2)))/(R121*(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))*(pow(x1-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				w211=((x2-obs_p_[i].x)*(y1-obs_p_[i].y)*(pow(z1-obs_p_[i].z,2)+pow(R211,2)))/(R211*(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))*(pow(x2-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				w111=((x1-obs_p_[i].x)*(y1-obs_p_[i].y)*(pow(z1-obs_p_[i].z,2)+pow(R111,2)))/(R111*(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2))*(pow(x1-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));

				v222=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R222*(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2)));
				v122=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R122*(pow(y2-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2)));
				v212=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R212*(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2)));
				v112=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R112*(pow(y1-obs_p_[i].y,2)+pow(z2-obs_p_[i].z,2)));
				v221=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R221*(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2)));
				v121=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R121*(pow(y2-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2)));
				v211=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R211*(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2)));
				v111=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R111*(pow(y1-obs_p_[i].y,2)+pow(z1-obs_p_[i].z,2)));

				t222=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R222*(pow(x2-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				t122=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R122*(pow(x1-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				t212=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R212*(pow(x2-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				t112=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R112*(pow(x1-obs_p_[i].x,2)+pow(z2-obs_p_[i].z,2)));
				t221=((x2-obs_p_[i].x)*(y2-obs_p_[i].y))/(R221*(pow(x2-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				t121=((x1-obs_p_[i].x)*(y2-obs_p_[i].y))/(R121*(pow(x1-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				t211=((x2-obs_p_[i].x)*(y1-obs_p_[i].y))/(R211*(pow(x2-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				t111=((x1-obs_p_[i].x)*(y1-obs_p_[i].y))/(R111*(pow(x1-obs_p_[i].x,2)+pow(z1-obs_p_[i].z,2)));
				
				G222=-k1*(z2-obs_p_[i].z)/(R222*(R222+x2-obs_p_[i].x))-k2*(z2-obs_p_[i].z)/(R222*(R222+y2-obs_p_[i].y))-k3/R222+k4*t222+k5*v222+k6*w222;
				G122=-k1*(z2-obs_p_[i].z)/(R122*(R122+x1-obs_p_[i].x))-k2*(z2-obs_p_[i].z)/(R122*(R122+y2-obs_p_[i].y))-k3/R122+k4*t122+k5*v122+k6*w122;
				G212=-k1*(z2-obs_p_[i].z)/(R212*(R212+x2-obs_p_[i].x))-k2*(z2-obs_p_[i].z)/(R212*(R212+y1-obs_p_[i].y))-k3/R212+k4*t212+k5*v212+k6*w212;
				G112=-k1*(z2-obs_p_[i].z)/(R112*(R112+x1-obs_p_[i].x))-k2*(z2-obs_p_[i].z)/(R112*(R112+y1-obs_p_[i].y))-k3/R112+k4*t112+k5*v112+k6*w112;
				G221=-k1*(z1-obs_p_[i].z)/(R221*(R221+x2-obs_p_[i].x))-k2*(z1-obs_p_[i].z)/(R221*(R221+y2-obs_p_[i].y))-k3/R221+k4*t221+k5*v221+k6*w221;
				G121=-k1*(z1-obs_p_[i].z)/(R121*(R121+x1-obs_p_[i].x))-k2*(z1-obs_p_[i].z)/(R121*(R121+y2-obs_p_[i].y))-k3/R121+k4*t121+k5*v121+k6*w121;
				G211=-k1*(z1-obs_p_[i].z)/(R211*(R211+x2-obs_p_[i].x))-k2*(z1-obs_p_[i].z)/(R211*(R211+y1-obs_p_[i].y))-k3/R211+k4*t211+k5*v211+k6*w211;
				G111=-k1*(z1-obs_p_[i].z)/(R111*(R111+x1-obs_p_[i].x))-k2*(z1-obs_p_[i].z)/(R111*(R111+y1-obs_p_[i].y))-k3/R111+k4*t111+k5*v111+k6*w111;

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