#ifndef _FUNC_H
#define _FUNC_H
#include "sysDefine.h"
#include "NALF-SFCM.h"
#include "progressBar_imp.h"
// 命令规则 n为阶数 m为次数
class sh2xyz
{
public:
	sh2xyz(){}
	~sh2xyz(){}
	int readSHC(char*,char*,char*); //读入球谐系数
	int initMatrix(char*,char*,char*,char*); //初始化相关的矩阵大小
	int initObs(char*,char*,char*); //初始化观测点 如只有范围参数则只初始化经纬度位置
	int relocateAltitude(char*); //根据输入文件重新确定计算高程
	int outObs(char*); //输出计算结果 如果有文件指定的位置则插值
	int calSolution(); //计算球谐结果 同一高程观测值
	int calSolution2(char*); //计算不同高程的观测值
private:
	_2dArray Anm;
	_2dArray Bnm;
	_2dArray Pnm; //伴随勒让德函数系数 这个函数只和观测位置的纬度/余纬度相关 同一纬度只需要计算一次
	_2dArray mCos; //不同次数cos函数值 这个值只和观测位置的经度相关 行数为不同经度位置 列数为不同次数 矩阵维度即为经度个数*阶次 一般估算在1000*1000级别
	_2dArray mSin; //不同次数sin函数值 其他与上同
	_2dArray coff_S; //球谐系数sin参数
	_2dArray coff_C; //球谐系数cos参数
	_2dArray multi_array; //乘子矩阵
	sphArray obsPoint; //计算地形是的观测位置 即计算半径值
	sphArray outPoint; //输出计算值

	double norSum;
	double GM,R; //球谐系数中重力常数与质量的乘积 单位为SI标准 g 与 m
	double multi_factor; // 乘子系数
	int NN_size; //系数矩阵大小
	int lon_size,lat_size;
	double refr,refR,altitude;
	double lonmin,lonmax,dlon;
	double latmin,latmax,dlat;
};
//读取球谐参数文件 文件名 起止阶次 列序列
int sh2xyz::readSHC(char* filename,char* para,char* orders)
{
	ifstream infile;
	if (open_infile(infile,filename)) return -1;

	int n_start,m_start,n_end,m_end;
	if (4 != sscanf(para,"%d/%d/%d/%d",&n_start,&m_start,&n_end,&m_end))
	{
		cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << para << endl;
		return -1;
	}
	//识别列次序
	int order[4];
	if (4 != sscanf(orders,"%d,%d,%d,%d",&order[0],&order[1],&order[2],&order[3]))
	{
		cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << orders << endl;
		return -1;
	}

	//按照最大阶数初始化下半三角矩阵
	NN_size = n_end + 1;
	//对于二维vector来说 对行初始化的时候需要使用resize 而对于列的初始化而言使用reserve效率更高
	coff_C.resize(NN_size);
	coff_S.resize(NN_size);
	for (int i = 0; i < NN_size; i++)
	{
		coff_C[i].resize(i+1,0.0);
		coff_S[i].resize(i+1,0.0);
	}

	int n,m; //行列号
	double temp_d,temp_c,temp_s;
	_1dArray temp_row; temp_row.reserve(100); //出现初始化100个double的空间 这样读文件更快
	string temp_str;
	stringstream temp_ss;
	while (getline(infile,temp_str))
	{
		if (*(temp_str.begin()) == '#') continue;
		if (!temp_row.empty()) temp_row.clear();
		temp_ss.str("");
		temp_ss.clear();
		temp_ss << temp_str;
		while (temp_ss >> temp_d)
			temp_row.push_back(temp_d);

		n = int(temp_row[order[0]]);
		m = int(temp_row[order[1]]);
		temp_c = temp_row[order[2]];
		temp_s = temp_row[order[3]];

		if (n >= n_start && n <= n_end && m >= m_start && m <= m_end)
		{
			coff_C[n][m] = temp_c;
			coff_S[n][m] = temp_s;
		}
	}
	infile.close();
	return 0;
}

int sh2xyz::initObs(char* r_para,char* i_para,char* refsys)
{
	//解析参考球
	if (!strcmp(refsys,"NULL"))
	{
		refr = refR = 0.0;
	}
	else if (!strcmp(refsys,"WGS84"))
	{
		refr = WGS84_PoleRadius;
		refR = WGS84_EquatorRadius;
	}
	else if (!strcmp(refsys,"EarthRadius"))
	{
		refr = EarthRadius;
		refR = EarthRadius;
	}
	else if (!strcmp(refsys,"MoonRadius"))
	{
		refr = MoonRadius;
		refR = MoonRadius;
	}
	else if (2 != sscanf(refsys,"%lf/%lf",&refr,&refR))
	{
		cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << refsys << endl;
		return -1;
	}

	//解析经纬度范围 按规则网络初始化观测点位置
	if (5 != sscanf(r_para,"%lf/%lf/%lf/%lf/%lf",&lonmin,&lonmax,&latmin,&latmax,&altitude))
	{
		if (4 != sscanf(r_para,"%lf/%lf/%lf/%lf",&lonmin,&lonmax,&latmin,&latmax))
		{
			cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << r_para << endl;
			return -1;
		}
		else altitude = 0.0;
	}

	//解析间隔
	if (2 != sscanf(i_para,"%lf/%lf",&dlon,&dlat))
	{
		cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << i_para << endl;
		return -1;
	}

	spoint temp_spoint;
	double lon,lat;
	lon_size = floor((lonmax-lonmin)/dlon) + 1;
	lat_size = floor((latmax-latmin)/dlat) + 1;
	obsPoint.reserve(lon_size*lat_size);
	for (int i = 0; i < lat_size; i++)
	{
		for (int j = 0; j < lon_size; j++)
		{
			lat = latmin + i*dlat; lon = lonmin + j*dlon;
			temp_spoint.lon = lon; temp_spoint.lat = lat;
			temp_spoint.ref = refRadius(temp_spoint.lat,refr,refR);
			temp_spoint.alti = altitude;
			temp_spoint.rad = temp_spoint.ref + temp_spoint.alti;
			temp_spoint.val = 0.0;
			obsPoint.push_back(temp_spoint);
		}
	}
	return 0;
}

int sh2xyz::relocateAltitude(char* filepara)
{
	char filename[1024];
	int orders[3] = {0,1,2}; //默认的读入的数据列为前三列

	if(!strcmp(filepara,"NULL")) return 0;

	//解析文件名中是否含有+d标示 如果有则将+d以前解释为filename 之后为需要读入的数据列 默认为逗号分隔
	//否则将filepara赋值为filename
	if (4 != sscanf(filepara,"%[^+]+d%d,%d,%d",filename,&orders[0],&orders[1],&orders[2]))
		strcpy(filename,filepara);

	ifstream infile;
	if (open_infile(infile,filename)) return -1;

	int numM,numN,tempM,tempN;
	string temp_str;
	stringstream temp_ss;
	double temp_d,temp_lon,temp_lat,temp_alti;
	_1dArray temp_row;

	numM = floor((latmax-latmin)/dlat)+1;
	numN = floor((lonmax-lonmin)/dlon)+1;
	while(getline(infile,temp_str))
	{
		if (*(temp_str.begin()) == '#') continue;

		temp_ss.str(""); temp_ss.clear(); temp_ss << temp_str;
		if(!temp_row.empty()) temp_row.clear();
		while(temp_ss >> temp_d)
			temp_row.push_back(temp_d);

		temp_lon = temp_row[orders[0]];
		temp_lat = temp_row[orders[1]];
		temp_alti = temp_row[orders[2]];

		tempM = round((temp_lat-latmin)/dlat);
		tempN = round((temp_lon-lonmin)/dlon);
		obsPoint[tempM*numN+tempN].alti = temp_alti;
		obsPoint[tempM*numN+tempN].rad = obsPoint[tempM*numN+tempN].ref + temp_alti;
	}
	infile.close();
	return 0;
}

//初始化矩阵
int sh2xyz::initMatrix(char* type,char* para,char* norType,char* zfile)
{
	//初始化GM与R
	if (strcmp(para,"NULL")) //如果para不为NULL则识别参数 否则将GM与R初始化为MAX_BDL
	{
		if (2 != sscanf(para,"%lf/%lf",&GM,&R))
		{
			cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << para << endl;
			return -1;
		}
	}
	else GM = R = MAX_BDL;
	//初始化归一化类型
	if (!strcmp(norType,"g")) norSum = 4.0*pi;
	else if (!strcmp(norType,"m")) norSum = 1.0;
	else
	{
		cerr << BOLDRED << "error ==> " << RESET << "wrong parameter: " << norType << endl;
		return -1;
	}
	//初始化伴随勒让德函数矩阵
	Pnm.resize(NN_size);
	for (int i = 0; i < NN_size; i++)
		Pnm.at(i).resize(i+1,0.0);
	//初始化sin和cos矩阵
	mCos.resize(lon_size);
	mSin.resize(lon_size);
	for (int i = 0; i < lon_size; i++)
	{
		mCos[i].reserve(NN_size);
		mSin[i].reserve(NN_size);
	}
	//计算mCos和mSin的值
	int i,j;
	double lon;
#pragma omp parallel for private(i,j,lon) schedule(guided)
	for (i = 0; i < lon_size; i++)
	{
		lon = lonmin + i*dlon;
		for (j = 0; j < NN_size; j++)
		{
			mCos[i].push_back(cos(j*lon*pi/180.0));
			mSin[i].push_back(sin(j*lon*pi/180.0));
		}
	}
	//计算勒让德函数系数
	Anm = get_a_nm_array(NN_size);
	Bnm = get_b_nm_array(NN_size);
	//计算乘子参数
	if(!strcmp(type,"t")) //topography
		multi_factor = 1.0;
	else if (!strcmp(type,"d") || !strcmp(type,"g")) //gravity disturbance
		multi_factor = 1e+5*GM/(R*R);
	else if (!strcmp(type,"r")) //gravity disturbance
		multi_factor = 1e+9*GM/(R*R);
	else if (!strcmp(type,"p"))
		multi_factor = 1e+5*GM/R;
	else
	{
		cerr << BOLDRED << "error ==> " << RESET << "unknown calculation type of " << type << endl;
		return -1;
	}
	//初始化乘子矩阵
	multi_array.resize(lat_size);
	for (i = 0; i < lat_size; i++)
	{
		multi_array[i].resize(NN_size,1.0); //初始化乘子矩阵为1 适用于地形等直接计算的类型
	}
	//如果计算高程不在同一高程 则不能使用multi_array 同时应该使用calSolution2()函数
	if (strcmp(zfile,"NULL"))
		return 0;
	//如果计算类型不是地形等直接计算类型则需要检验-g选项是否已经设置
	if (strcmp(type,"t"))
	{
		if (GM == MAX_BDL || R == MAX_BDL)
		{
			cerr << BOLDRED << "error ==> " << RESET << "-g option must be set for gravitational calculation" << endl;
			return -1;
		}
	}
	//根据不同类型计算乘子参数和乘子矩阵
	if (!strcmp(type,"d")) //gravity disturbance
	{
#pragma omp parallel for private(i,j) shared(R,lon_size) schedule(guided)
		for (i = 0; i < lat_size; i++)
		{
			for (j = 0; j < NN_size; j++)
			{
				multi_array[i][j] = pow(R/obsPoint[i*lon_size].rad,j+2)*(j+1);
			}
		}
	}
	else if (!strcmp(type,"r")) //gravity gradient
	{
#pragma omp parallel for private(i,j) shared(R,lon_size) schedule(guided)
		for (i = 0; i < lat_size; i++)
		{
			for (j = 0; j < NN_size; j++)
			{
				multi_array[i][j] = pow(R/obsPoint[i*lon_size].rad,j+2)*(j+1)*(j+2)/obsPoint[i*lon_size].rad;
			}
		}
	}
	else if (!strcmp(type,"g")) //gravity anomaly
	{
#pragma omp parallel for private(i,j) shared(R,lon_size) schedule(guided)
		for (i = 0; i < lat_size; i++)
		{
			for (j = 0; j < NN_size; j++)
			{
				multi_array[i][j] = pow(R/obsPoint[i*lon_size].rad,j+2)*(j-1);
			}
		}
	}
	else if (!strcmp(type,"p")) //geo-potential
	{
#pragma omp parallel for private(i,j) shared(R,lon_size) schedule(guided)
		for (i = 0; i < lat_size; i++)
		{
			for (j = 0; j < NN_size; j++)
			{
				multi_array[i][j] = pow(R/obsPoint[i*lon_size].rad,j+1);
			}
		}
	}
	return 0;
}

int sh2xyz::outObs(char* filename)
{
	if (!strcmp(filename,"NULL")) //没有输入文件 直接输出规则网计算结果
	{
		cout << "# NaN value = 1e+30" << endl;
		cout << "# lon(deg) lat(deg) reference-radius(m) altitude(m) topography(m)|gravitational-field(mGal)" << endl;
		for (int i = 0; i < obsPoint.size(); i++)
		{
			obsPoint[i].info();
		}
	}
	else
	{
		ifstream infile;
		if(open_infile(infile,filename)) return -1;

		spoint temp_sp;
		string temp_str;
		stringstream temp_ss;
		while (getline(infile,temp_str))
		{
			if(*(temp_str.begin()) == '#') continue;

			temp_ss.str("");
			temp_ss.clear();
			temp_ss << temp_str;
			temp_ss >> temp_sp.lon >> temp_sp.lat;
			temp_sp.ref = refRadius(temp_sp.lat,refr,refR);
			temp_sp.alti = altitude;
			outPoint.push_back(temp_sp);
		}
		infile.close();

		int numM,numN,tempM,tempN;
		double lon1,lon2,lat1,lat2;

		numM = floor((latmax-latmin)/dlat)+1;
		numN = floor((lonmax-lonmin)/dlon)+1;
		for (int i = 0; i < outPoint.size(); i++)
		{
			tempM = floor((outPoint[i].lat-latmin)/dlat);
			tempN = floor((outPoint[i].lon-lonmin)/dlon);

			if (tempM == (numM-1))
				tempM -= 1;
			if (tempN == (numN-1))
				tempN -= 1;

			if (tempM >= 0 && tempN >= 0 && tempM <= numM-2 && tempN <= numN-2)
			{
				lon1 = lonmin+tempN*dlon;
				lon2 = lonmin+(tempN+1)*dlon;
				lat1 = latmin+tempM*dlat;
				lat2 = latmin+(tempM+1)*dlat;
				outPoint[i].val = SphBiInterp_deg(lat1,lat2,lon1,lon2,
											outPoint[i].lat,outPoint[i].lon,
											obsPoint[tempM*numN+tempN].val,
											obsPoint[tempM*numN+tempN+1].val,
											obsPoint[(tempM+1)*numN+tempN].val,
											obsPoint[(tempM+1)*numN+tempN+1].val);
			}
		}

		cout << "# NaN value = 1e+30" << endl;
		cout << "# lon(deg) lat(deg) reference-radius(m) altitude(m) topography(m)|gravitational-field(mGal)" << endl;
		for (int i = 0; i < outPoint.size(); i++)
		{
			outPoint[i].info();
		}
	}
	return 0;
}

int sh2xyz::calSolution()
{
	//计算
	int i,j,n,m;
	double temp_d,lat;

	ProgressBar *bar = new ProgressBar(lat_size,"Process");
	for (i = 0; i < lat_size; i++)
	{
		bar->Progressed(i);
		lat = latmin + dlat*i;
		//计算伴随勒让德函数 对于同一个纬度只需要计算一次
		NALF_SFCM3(Pnm,Anm,Bnm,NN_size,90.0-lat,norSum);
		//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
		//一种并行方案更快一些
#pragma omp parallel for private(j,n,m,temp_d) shared(i,multi_factor) schedule(guided)
		for (j = 0; j < lon_size; j++)
		{
			temp_d = 0;
			for (n = 0; n < NN_size; n++)
			{
				for (m = 0; m < n+1; m++)
				{
					temp_d += multi_array[i][n]*Pnm[n][m]*(coff_C[n][m]*mCos[j][m]+coff_S[n][m]*mSin[j][m]);
				}
			}
			obsPoint[i*lon_size+j].val = multi_factor*temp_d;
		}
	}
	return 0;
}

int sh2xyz::calSolution2(char* type)
{
	//计算
	int i,j,n,m;
	double temp_d,lat;

	ProgressBar *bar = new ProgressBar(lat_size,"Process");
	if (!strcmp(type,"d"))
	{
		for (i = 0; i < lat_size; i++)
		{
			bar->Progressed(i);
			lat = latmin + dlat*i;
			//计算伴随勒让德函数 对于同一个纬度只需要计算一次
			NALF_SFCM3(Pnm,Anm,Bnm,NN_size,90.0-lat,norSum);
			//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
			//一种并行方案更快一些
#pragma omp parallel for private(j,n,m,temp_d) shared(i,multi_factor,lon_size) schedule(guided)
			for (j = 0; j < lon_size; j++)
			{
				temp_d = 0;
				for (n = 0; n < NN_size; n++)
				{
					for (m = 0; m < n+1; m++)
					{
						//pow(R/obsPoint[i*lon_size+j].rad,n+2)*(n+1)
						temp_d += pow(R/obsPoint[i*lon_size+j].rad,n+2)*(n+1)*Pnm[n][m]*(coff_C[n][m]*mCos[j][m]+coff_S[n][m]*mSin[j][m]);
					}
				}
				obsPoint[i*lon_size+j].val = multi_factor*temp_d;
			}
		}
	}
	else if (!strcmp(type,"g"))
	{
		for (i = 0; i < lat_size; i++)
		{
			bar->Progressed(i);
			lat = latmin + dlat*i;
			//计算伴随勒让德函数 对于同一个纬度只需要计算一次
			NALF_SFCM3(Pnm,Anm,Bnm,NN_size,90.0-lat,norSum);
			//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
			//一种并行方案更快一些
#pragma omp parallel for private(j,n,m,temp_d) shared(i,multi_factor,lon_size) schedule(guided)
			for (j = 0; j < lon_size; j++)
			{
				temp_d = 0;
				for (n = 0; n < NN_size; n++)
				{
					for (m = 0; m < n+1; m++)
					{
						//pow(R/obsPoint[i*lon_size+j].rad,n+2)*(n-1)
						temp_d += pow(R/obsPoint[i*lon_size+j].rad,n+2)*(n-1)*Pnm[n][m]*(coff_C[n][m]*mCos[j][m]+coff_S[n][m]*mSin[j][m]);
					}
				}
				obsPoint[i*lon_size+j].val = multi_factor*temp_d;
			}
		}
	}
	else if (!strcmp(type,"p"))
	{
		for (i = 0; i < lat_size; i++)
		{
			bar->Progressed(i);
			lat = latmin + dlat*i;
			//计算伴随勒让德函数 对于同一个纬度只需要计算一次
			NALF_SFCM3(Pnm,Anm,Bnm,NN_size,90.0-lat,norSum);
			//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
			//一种并行方案更快一些
#pragma omp parallel for private(j,n,m,temp_d) shared(i,multi_factor,lon_size) schedule(guided)
			for (j = 0; j < lon_size; j++)
			{
				temp_d = 0;
				for (n = 0; n < NN_size; n++)
				{
					for (m = 0; m < n+1; m++)
					{
						//pow(R/obsPoint[i*lon_size+j].rad,n+1)
						temp_d += pow(R/obsPoint[i*lon_size+j].rad,n+1)*Pnm[n][m]*(coff_C[n][m]*mCos[j][m]+coff_S[n][m]*mSin[j][m]);
					}
				}
				obsPoint[i*lon_size+j].val = multi_factor*temp_d;
			}
		}
	}
	return 0;
}

#endif