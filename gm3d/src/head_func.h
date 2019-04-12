#ifndef _HEAD_FUNC_H
#define _HEAD_FUNC_H
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
#define PRECISION 16
#define ZERO 1e-16

//物理常量
#define Pi (4.0*atan(1.0))
//注意这里本来应该是e-11，考虑到单位转换，取维度单位为m，密度单位为g/cm^3，乘以G0则重力单位即为mGal
#define G0 6.67408e-3
//宏函数
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
//如果in在a和b之间返回in 否则返回边界值
#define SetToBox(a,b,in) (MAX(a,MIN(b,in)))
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
typedef map<int,int> _int2intMap;
//模型块体参数
struct modelist{
	char mod_type[1024];
	char val_type[1024];
	char mod_para[1024];
	double mod_value;
};
typedef vector<modelist> modelistArray;
//直角坐标系点
struct cpoint{
	int id = -1;
	double x = BDL_MAX; double y = BDL_MAX; double z = BDL_MAX;
};
typedef vector<cpoint> cpointArray;
typedef map<string,cpoint> _str2pointMap;
//观测点
struct obspoint : public cpoint{
	double val = BDL_MAX; double dev = BDL_MAX;
};
typedef vector<obspoint> obspointArray;
//块体
struct cube{
	cpoint cen;
	int ids[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	double dx = BDL_MAX; double dy = BDL_MAX; double dz = BDL_MAX;
};
typedef vector<cube> cubeArray;
/*************************数据结构函数********************************/
cpoint operator -(cpoint,cpoint); //矢量减法
double modCpoint(cpoint); //矢量模
/*************************全局函数********************************/
double arctg(double); //正负分离的atan函数 正数返回atan 负数返回atan+pi
stringstream str2ss(string); //将string转换为stringstream
string cpoint_id(cpoint); //返回一个cpoint的位置id
int open_infile(ifstream&,char*); //测试打开输入文件 如果成功则返回0并输出信息 否则返回1
int open_outfile(ofstream&,char*); //测试打开输出文件 如果成功则返回0并输出信息 否则返回1
double grid_interpolate(double,double,double,double,double,double,double,double,double,double); //规则网络插值
#endif