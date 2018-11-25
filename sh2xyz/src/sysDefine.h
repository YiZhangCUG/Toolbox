#ifndef _SYSDEFINE_H
#define _SYSDEFINE_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "iomanip"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "cmath"
#include "vector"
#include "omp.h"

#define pi (4.0*atan(1.0))
#define MAX_BDL 1e+30
#define WGS84_PoleRadius 6356752.314//WGS84椭球极半径
#define WGS84_EquatorRadius 6378137//WGS84椭球长半径
#define EarthRadius 6371000
#define MoonRadius 1738000

#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDBLUE "\033[1m\033[34m"
#define UNDERLINE "\033[1m\033[4m"
#define RESET "\033[0m"
#define MOVEUP(x) printf("\033[%dA", (x))
#define MOVEDOWN(x) printf("\033[%dB", (x))
#define MOVELEFT(x) printf("\033[%dD", (x))
#define MOVERIGHT(x) printf("\033[%dC", (x))
#define MOVETO(y,x) printf("\033[%d;%dH", (y), (x))
#define CLEARLINE "\033[K"
#define CLEARALL "\033[2J"

using namespace std;

typedef vector<double> _1dArray;
typedef vector<vector<double> > _2dArray;

struct spoint
{
	double lon,lat,ref,alti,rad,val;
	spoint()
	{
		lon = lat = ref = alti = rad = val = MAX_BDL;
	}
	void info()
	{
		cout << setprecision(16) << lon << " " << lat << " "  << ref << " " << alti << " " << val << endl;
	}
};
typedef vector<spoint> sphArray;

//计算一个参考椭球或者参考球在纬度位置的半径
double refRadius(double lati,double refr,double refR)
{
	return refr*refR/sqrt(pow(refr,2)*pow(cos((double) lati*pi/180.0),2)+pow(refR,2)*pow(sin((double) lati*pi/180.0),2));
}
//测试打开输入文件 如果成功则返回0并输出信息 否则返回1
int open_infile(ifstream &infile,char* filename)
{
	infile.open(filename);
	if (!infile)
	{
		cout << BOLDRED << "error ==> " << RESET << "file not found: " << filename << endl;
		return -1;
	}
	return 0;
}
//测试打开输出文件 如果成功则返回0并输出信息 否则返回1
int open_outfile(ofstream &outfile,char* filename)
{
	outfile.open(filename);
	if (!outfile)
	{
		cout << BOLDRED << "error ==> " << RESET << "fail to create the file: " << filename << endl;
		return -1;
	}
	return 0;
}

double cosd(double deg)
{
	return (cos(deg*pi/180.0));
}
/*
lat
|
|
h21----h22
|       |
|       |
h11----h12----> lon
*/
// 球面双线性插值函数 以度为单位的版本 注意纬度为余纬度
double SphBiInterp_deg(double CoLat1,double CoLat2,double Lon1,double Lon2,double CoLat0,double Lon0,double h11,double h12,double h21,double h22)
{
	double Delta=(Lon2-Lon1)*(cosd(CoLat2)-cosd(CoLat1));
	double A=(Lon1*(h12-h22)+Lon2*(h21-h11))/Delta;
	double B=(cosd(CoLat1)*(h21-h22)+cosd(CoLat2)*(h12-h11))/Delta;
	double C=(h11+h22-h21-h12)/Delta;
	double D=(Lon2*cosd(CoLat2)*h11-Lon2*cosd(CoLat1)*h21+Lon1*cosd(CoLat1)*h22-Lon1*cosd(CoLat2)*h12)/Delta;
	double h0=A*cosd(CoLat0)+B*Lon0+C*Lon0*cosd(CoLat0)+D;
	return h0;
}
#endif