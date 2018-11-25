#include "sh2xyz.h"

void disp_help()
{
	cout << "sh2xyz - v0.1 calculated spherical harmonic coefficients to table data" << endl
		<< "Author: zhangyi.cugwuhan@gmail.com" << endl << endl
		<< "usage: sh2xyz -t<table> -r<lonmin>/<lonmax>/<latmin>/<latmax>[/<altitude>] -i<dlon>/<dlat> -d<type> -f<ln>/<lm>/<hn>/<hm> [-p<loc-file>] [-s<refr>/<refR>] [-g<GM>/<R>] [-n<type>] [-c<col1>,<col2>,<col3>,<col4>] [-z<alti-file>[+d<col1>,<col2>,<col3>]] [-h] > out-file" << endl
		<< "-t\tinput spherical harmonic coefficients, four columns are needed at least" << endl
		<< "-r\tcalculation range" << endl
		<< "-i\tcalculation intervals" << endl
		<< "-d\tcalculation types, \n\tt for topography (default)\n\td for gravity disturbance\n\tg for gravity anomaly and\n\tp for geo-potential\n\tr for radial gravity gradient" << endl
		<< "-s\treference system" << endl
		<< "-p\tinterpolating locations indicated by a input file, every line of the file has a longitudinal and a latitudinal value" << endl
		<< "-g\tGM and R parameters for gravitational data's calculation" << endl
		<< "-n\tnormalized sum of associated Legendre functions\n\tm for mathematic normalization which equal 1\n\tg for geodetic normalization which equal 4*pi (default)" << endl
		<< "-f\tstart and end orders and degrees used for calculation" << endl
		<< "-c\tselect data columns of the input table data. the default is 0,1,2,3" << endl
		<< "-z\tprovide a input file for observations' altitudes. use +d to select input data columns, the default is 0,1,2" << endl
		<< "-h\tshow this info" << endl;
}

int main(int argc, char* argv[])
{
	char infilename[1024] = "NULL";
	char interfilename[1024] = "NULL";
	char range[1024] = "NULL";
	char interval[1024] = "NULL";
	char type[1024] = "t";
	char referSystem[1024] = "NULL";
	char coffPara[1024] = "NULL";
	char columns[1024] = "0,1,2,3";
	char gravPara[1024] = "NULL";
	char norType[1024] = "g";
	char altiFile[1024] = "NULL";

	opterr = 0; //内置参数 若不为0则会在发生遭遇错误时输出一条信息到屏幕

	int curr;
	/*循环拾取参数 最后一个参数为-1 需要变量的参数后跟一个冒号 可有可无参数跟两个冒号*/
	while((curr = getopt(argc,argv,"ht:r:i:s:f:c:d:g:n:p:z:")) != -1)
	{
		/*匹配命令*/
		switch (curr)
		{
			case 'h': //显示帮助信息
				disp_help();
				break;
			case 't':
				if (1!=sscanf(optarg,"%s",infilename))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'r':
				if (1!=sscanf(optarg,"%s",range))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'i':
				if (1!=sscanf(optarg,"%s",interval))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'd':
				if (1!=sscanf(optarg,"%s",type))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'p':
				if (1!=sscanf(optarg,"%s",interfilename))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 's':
				if (1!=sscanf(optarg,"%s",referSystem))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'g':
				if (1!=sscanf(optarg,"%s",gravPara))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'n':
				if (1!=sscanf(optarg,"%s",norType))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'f':
				if (1!=sscanf(optarg,"%s",coffPara))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'c':
				if (1!=sscanf(optarg,"%s",columns))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'z':
				if (1!=sscanf(optarg,"%s",altiFile))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case '?': //处理未定义或错误参数
				if (optopt == 't' || optopt == 'r' || optopt == 'i' || optopt == 's' || optopt == 'f' || optopt == 'c' || optopt == 'p' || optopt == 'z')
				{
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
					return -1;
				}
				else if (isprint(optopt))
				{
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
					return -1;
				}
				else
				{
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
					return -1;
				}
				break;
			default:
				abort();
		}
	}

	sh2xyz sx;
	//读入球谐系数文件
	if(sx.readSHC(infilename,coffPara,columns)) return 0;
	//初始化观测点位置
	if(sx.initObs(range,interval,referSystem)) return 0;
	//重定位观测点高程
	if(sx.relocateAltitude(altiFile)) return 0;
	//初始化矩阵
	sx.initMatrix(type,gravPara,norType,altiFile);
	//计算
	if (!strcmp(altiFile,"NULL"))
	{
		sx.calSolution();
	}
	else sx.calSolution2(type);
	//输出
	sx.outObs(interfilename);
	return 0;
}