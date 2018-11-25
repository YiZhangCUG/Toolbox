#ifndef _INCLUDES_H
#define _INCLUDES_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "string.h"
#include "cmath"
#include "iomanip"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "vector"
#include "map"
#include "algorithm"
#include "ctime"
#include "omp.h"
#include "random"

using namespace std;

//数学常量
#define BDL_MAX 1e+30
#define BDL_MIN -1e+30
#define ZERO 1e-20
//物理常量
#define Pi (4.0*atan(1.0))
#define G0 6.67408e-3 //注意这里本来应该是e-11，考虑到单位转换，取维度单位为m，密度单位为g/cm^3，乘以G0则重力单位即为mGal
//宏函数
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define SetToBox(a,b,in) (MAX(a,MIN(b,in))) //如果in在a和b之间返回in 否则返回边界值
//终端显示控制符
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDYELLOW "\033[1m\033[33m"
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
//数据结构
typedef vector<int> _1iArray;
typedef vector<double> _1dArray;
typedef vector<string> _1sArray;
typedef vector<vector<int> > _2iArray;
typedef vector<vector<double> > _2dArray;
typedef map<int,int> _i2iMap;

struct cpoint
{
	int id = -1;
	double x = BDL_MAX; double y = BDL_MAX; double z = BDL_MAX;
};
typedef vector<cpoint> cpointArray;

struct obspoint : public cpoint
{
	double val = BDL_MAX; double dev = BDL_MAX;
};
typedef vector<obspoint> obspointArray;

struct cube
{
	cpoint cen;
	double rho = 0.0;
	double dx = BDL_MAX; double dy = BDL_MAX; double dz = BDL_MAX;
};
typedef vector<cube> cubeArray;

/*************************全局函数********************************/
//正负分离的atan函数 正数返回atan 负数返回atan+pi
double arctg(double v)
{
	double ang;
	if(v>=0) ang=atan(v);
	else if(v<0) ang=atan(v)+Pi;
	return ang;
}
//将string转换为stringstream
stringstream str2ss(string s){
	stringstream sstr;
	sstr.str(""); sstr.clear(); sstr.str(s);
	return sstr;
}
//测试打开输入文件 如果成功则返回0并输出信息 否则返回1
int open_infile(ifstream &infile,char* filename){
	infile.open(filename);
	if (!infile){
		cerr << BOLDRED << "error ==> " << RESET << "file not found: " << filename << endl;
		return -1;
	}
	return 0;
}
//测试打开输出文件 如果成功则返回0并输出信息 否则返回1
int open_outfile(ofstream &outfile,char* filename){
	outfile.open(filename);
	if (!outfile){
		cerr << BOLDRED << "error ==> " << RESET << "fail to create the file: " << filename << endl;
		return -1;
	}
	return 0;
}
#endif