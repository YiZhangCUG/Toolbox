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
#define BDL_MAX 1e+30 ///< 定义变量的最大值
#define BDL_MIN -1e+30 ///< 定义变量的最小值
#define PRECISION 16 ///< 定义小数点后需要使用的位数
#define ZERO 1e-16 ///< 定义零值

//物理常量
#define Pi (4.0*atan(1.0)) ///< 圆周率
#define G0 6.67408e-3 ///< 万有引力常数。注意这里的量级本来应该是e-11，考虑到单位转换，取维度单位为m，密度单位为g/cm^3，乘以G0则重力单位即为mGal
#define T0 5.0e+4 ///< 地磁场平均强度
//宏函数
#define MAX(a,b) (a>b?a:b) ///< 返回a与b的最大值
#define MIN(a,b) (a<b?a:b) ///< 返回a与b的最小值
#define SetToBox(a,b,in) (MAX(a,MIN(b,in))) ///< 返回a与b之间的一个值，若in在a与b之间则直接返回，否则返回较近的边界值
//终端显示控制符
#define BOLDRED "\033[1m\033[31m" ///< 设置后续字符字体为红色加粗
#define BOLDGREEN "\033[1m\033[32m" ///< 设置后续字符字体为绿色加粗
#define BOLDYELLOW "\033[1m\033[33m" ///< 设置后续字符字体为黄色加粗
#define BOLDBLUE "\033[1m\033[34m" ///< 设置后续字符字体为蓝色加粗
#define UNDERLINE "\033[1m\033[4m" ///< 设置后续字符为添加下划线
#define RESET "\033[0m" ///< 重置字符设置
#define MOVEUP(x) printf("\033[%dA", (x)) ///< 将光标向上挪x行
#define MOVEDOWN(x) printf("\033[%dB", (x)) ///< 将光标向下娜x行
#define MOVELEFT(x) printf("\033[%dD", (x)) ///< 将光标向左娜x字符
#define MOVERIGHT(x) printf("\033[%dC", (x)) ///< 将光标向右娜x字符
#define MOVETO(y,x) printf("\033[%d;%dH", (y), (x)) ///< 将光标向右娜动y字符,向上挪动x字符
#define CLEARLINE "\033[K" ///< 清除本行
#define CLEARALL "\033[2J" ///< 清除终端满屏
//数据结构
typedef vector<int> _1iArray; ///< 整形一维向量
typedef vector<double> _1dArray; ///< 双精度浮点一维向量
typedef vector<string> _1sArray; ///< 字符串一维向量
typedef vector<vector<int> > _2iArray; ///< 整形浮点二维向量
typedef vector<vector<double> > _2dArray; ///< 双精度浮点二维向量
typedef map<int,int> _int2intMap; ///< 整型到整形的映射
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