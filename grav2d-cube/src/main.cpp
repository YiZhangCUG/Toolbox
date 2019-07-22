#include "grav2d_cube.h"
#include "dispHelp.h"

void disp_help(char* proname)
{
	string exName = proname;
	string exUsage = proname;
	exUsage += " -r<y-start>/<y-step>/<y-end>/<x-start>/<x-step>/<x-end>/<elevation>|<filename> \
	-c<y-cen>/<x-cen>/<z-cen>/<dy>/<dx>/<dz>/<density>|<filename> -tgravity|gx|gy|gz > out-file";
	dispHelp dh;
	dh.changeLayerOut(0,10);
	dh.addHeadInfo(exName,"0.1","Forward calculation of gravitational data of cubes.","Yi Zhang (zhangyi.cugwuhan@gmail.com)");
	dh.addUsage(exUsage);
	dh.addOption("Range of calculation, which could get from parameters or a file contains x y z locations. Defaults are 0/10/1000/0/10/1000/0","-r");
	dh.addOption("Cube parameters, which could get from parameters (single) or a file (multiple). Defaults are 500/500/200/100/100/100/1.0","-c");
	dh.addOption("Calculation type equals gravity (default), gx, gy or gz.","-t");
	dh.show();
	return;
}

int main(int argc, char* argv[])
{
	grav2d_cube gc;
	char rangeChar[1024] = "0/10/1000/0/10/1000/0";
	char cubeChar[1024] = "500/500/200/100/100/100/1.0";
	char typeChar[1024] = "gravity";

	opterr = 0; //内置参数 若不为0则会在发生遭遇错误时输出一条信息到屏幕

	int curr;
	/*循环拾取参数 最后一个参数为-1 需要变量的参数后跟一个冒号 可有可无参数跟两个冒号*/
	while((curr = getopt(argc,argv,"hr:c:t:")) != -1)
	{
		/*匹配命令*/
		switch (curr)
		{
			case 'h': //显示帮助信息
				disp_help(argv[0]);
				return 0;
			case 'r':
				if (1!=sscanf(optarg,"%s",rangeChar))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'c':
				if (1!=sscanf(optarg,"%s",cubeChar))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 't':
				if (1!=sscanf(optarg,"%s",typeChar))
				{
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case '?': //处理未定义或错误参数
				if (optopt == 'r' || optopt == 'c' || optopt == 't')
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

	gc.routine(typeChar,rangeChar,cubeChar);
	return 0;
}