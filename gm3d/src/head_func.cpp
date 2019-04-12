#include "head_func.h"
/*************************数据结构函数********************************/
//cpoint减法
cpoint operator -(cpoint a, cpoint b){
	cpoint m;
	m.x=a.x-b.x;
	m.y=a.y-b.y;
	m.z=a.z-b.z;
	return m;
}
//cpoint模长
double modCpoint(cpoint v){
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}
/*************************全局函数********************************/
//正负分离的atan函数 正数返回atan 负数返回atan+pi
double arctg(double v){
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
//返回一个cpoint的位置id字符串
string cpoint_id(cpoint c){
	string vert_id, mid_id;
	stringstream sstemp;
	sstemp.str(""); sstemp.clear(); sstemp<<setprecision(PRECISION)<<c.x;
	sstemp>>vert_id;
	sstemp.str(""); sstemp.clear(); sstemp<<setprecision(PRECISION)<<c.y;
	sstemp>>mid_id;
	vert_id = vert_id + " " + mid_id;
	sstemp.str(""); sstemp.clear(); sstemp<<setprecision(PRECISION)<<c.z;
	sstemp>>mid_id;
	vert_id = vert_id + " " + mid_id;
	return vert_id;
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

//规则网络插值 长方形内数据插值 距离平方反比
/*长方体示意图*/
// y
// |
// |
// 3------------2
// |            |
// |            |
// |            |
// 0------------1--->x
// 左下角坐标x0 y0
// 块体尺寸dx dy
// 插值点坐标x y
// 四个角点值
double grid_interpolate(double x0,double y0,double dx,double dy,double x,double y,
					double d0,double d1,double d2,double d3)
{
	double res = 0;
	double total_dist = 0;
	double dist[4] = {0,0,0,0};
	double val[4];
	val[0] = d0; val[1] = d1; val[2] = d2; val[3] = d3;
	dist[0] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-y0)*(y-y0));
	dist[1] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-y0)*(y-y0));
	dist[2] = 1.0/(1e-30+(x-dx-x0)*(x-dx-x0)+(y-dy-y0)*(y-dy-y0));
	dist[3] = 1.0/(1e-30+(x-x0)*(x-x0)+(y-dy-y0)*(y-dy-y0));
	for (int i = 0; i < 4; i++){
		total_dist += dist[i];
	}
	for (int i = 0; i < 4; i++){
		res += val[i]*dist[i]/total_dist;
	}
	return res;
}