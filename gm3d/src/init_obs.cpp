#include "gm3d.h"

int GM3D::InitObs(char* obs_para){
	obspoint temp_obs;
	string temp_str;
	stringstream temp_ss;
	double x,y;
	double xmin,xmax,ymin,ymax;
	double xs,xe,ys,ye,eleva,dx,dy;

	//按格式解析参数 初始化观测位置 用于正演计算
	if (7 == sscanf(obs_para,"%lf/%lf/%lf/%lf/%lf/%lf/%lf",&xs,&dx,&xe,&ys,&dy,&ye,&eleva)){
		xmin = MIN(xs,xe); xmax = MAX(xs,xe);
		ymin = MIN(ys,ye); ymax = MAX(ys,ye);

		y = ys;
		while(y >= ymin && y <= ymax){
			x = xs;
			while(x >= xmin && x <= xmax){
				temp_obs.id = obs_p_.size();
				temp_obs.x = x; temp_obs.y = y; temp_obs.z = -1.0*eleva;
				temp_obs.val = temp_obs.dev = 0.0;
				obs_p_.push_back(temp_obs);
				x += dx;
			}
			y += dy;
		}
	}
	//解析失败 按文件读入 用于反演使用或者正演计算
	else{
		ifstream infile;
		if (open_infile(infile,obs_para)) return -1;

		while(getline(infile,temp_str)){
			if (*(temp_str.begin()) == '#') continue;
			else{
				//按每行3个数据解析 初始化为用于正演的观测点
				if (3 == sscanf(temp_str.c_str(),"%lf %lf %lf",&temp_obs.x,&temp_obs.y,&temp_obs.z)){
					temp_obs.z *= -1.0;
					temp_obs.id = obs_p_.size();
					temp_obs.val = temp_obs.dev = 0.0;
					obs_p_.push_back(temp_obs);
				}
				else{
					cerr << BOLDYELLOW << "ignored ==> " << RESET << "wrong input: " << temp_str << endl;
					continue;
				}
			}
		}
		infile.close();
	}

	if (obs_p_.empty()){
		cerr << BOLDRED << "error ==> " << RESET << "fail to initial observations with the parameter: " << obs_para << endl;
		return -1;
	}
	else{
		obs_num_ = obs_p_.size();
	}
	return 0;
}