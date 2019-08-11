#include "gm3d.h"
#include "disp_help.h"

void display_help_info(char* program_name){
	string ex_name = program_name;
	string ex_usage = program_name;
	ex_usage += " {-o<output-msh-file> -m<model-list-file>  \
	[-d<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<zs>/<dz>/<ze>] [-r]} \
	| {-i<input-msh-file> -p<observation-file>|<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<elevation> \
	-f<output-data-file> -tVz|Vzx|Vzy|Vzz|DT|DTx|DTy|DTz|Hax|Hay|Za \
	[-v<I0>/<D0>/<I>/<D>] [-n<noise-mean>/<noise-dev>]} -e<element-data-name> [-h]";

	DispHelp dh;
	dh.changeLayerOut(0,10);
	dh.addHeadInfo(ex_name,"1.0","3D forward modeling of gravity and magnetic data under the Cartesian coordinates.","Dr. Yi Zhang (zhangyiss@icloud.com). School of Earth Sciences, Zhejiang University");
	dh.addUsage(ex_usage);
	dh.addOption("Filename of the input Gmsh(.msh) model file for forward calculation.","-i");
	dh.addOption("Filename of the output Gmsh(.msh) model file built with given parameters.","-o");
	dh.addOption("Filename of the output observation file of gravity or magnetic data.","-f");
	dh.addOption("3D dimensions of the model space. the suffix 's' means the starting coordinate and 'e' represents the ending coordinate in axial directions. \
	 'dx', 'dy' and 'dz' are step lengths. The default value is 10/20/990/10/20/990/10/20/490. \
	 The axial orientation adopted by the program is a right-hand Cartesian system with the z-axis point vertical downward.","-d");
	dh.addOption("Model file that contains different types of model parameter. See instructions for formats of different model types. Note that any line starts with '#' will be skipped. \
	Four different types of models are supported by the program. Keyword parameters for different model types are shown as:","-m");
	dh.addOptionSec("1. regular_block add|replace|erase <physical_value> <xmin>/<xmax>/<ymin>/<ymax>/<zmin>/<zmax>");
	dh.addOptionSec("2. tilted_block add|replace|erase <physical_value> <xmin_z>/<xmax_z>/<ymin_z>/<ymax_z>/<zmin>/<xmin_Z>/<xmax_Z>/<ymin_Z>/<ymax_Z>/<zmax>");
	dh.addOptionSec("3. sphere add|replace|erase <physical_value> <xcen>/<ycen>/<zcen>/<xradius>/<yradius>/<zradius>");
	dh.addOptionSec("4. interface add|replace|erase/top|bot <physical_value> <filename>");
	dh.addOption("Element data name of the input/output Gmsh(.msh) file. Note that the name must be around by \"\".","-e");
	dh.addOption("Observation locations. You can either initialize the observation points from parameters or a file. \
	 Each line of the file contain coordinates y(easting), x(northing) and z(elevation) of an observation point.","-p");
	dh.addOption("Forward component Vz, Vzx, Vzy or Vzz for gravitational data and \
		DeltaT, DeltaTx, DeltaTy, DeltaTz, Hax, Hay and Za for magnetic data.","-t");
	dh.addOption("Inclination and declination of the geomagnetic field and magnetization.","-v");
	dh.addOption("Add noise to the forward calculated data","-n");
	dh.addOption("Remove model elements with no data in the output Gmsh(.msh) file.","-r");
	dh.addOption("Display help information.","-h");
	dh.show();
	return;
}

int main(int argc, char* argv[]){
	GM3D gm3d_instance;
	char dimension[1024] = "10/20/990/10/20/990/10/20/490";
	char modelname[1024] = "NULL";
	char in_mshname[1024] = "NULL";
	char out_mshname[1024] = "Untitled.msh";
	char elename[1024] = "Untitled";
	char obsername[1024] = "NULL";
	char out_obserfile[1024] = "NULL";
	char noise_para[1024] = "NULL";
	char forward_type[1024] = "Vz";
	char mag_field_para[1024] = "90/0/90/0";
	bool build_model = true;
	bool remove_null = false;

	opterr = 0; //内置参数 若不为0则会在发生遭遇错误时输出一条信息到屏幕

	if (argc == 1){
		display_help_info(argv[0]);
		return 0;
	}

	int curr;
	/*循环拾取参数 最后一个参数为-1 需要变量的参数后跟一个冒号 可有可无参数跟两个冒号*/
	while((curr = getopt(argc,argv,"hrd:m:i:o:e:p:f:n:t:v:")) != -1){
		/*匹配命令*/
		switch (curr){
			case 'h': //显示帮助信息
				display_help_info(argv[0]);
				return 0;
			case 'r':
				remove_null = true;
				break;
			case 'd':
				if (1!=sscanf(optarg,"%s",dimension)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'm':
				if (1!=sscanf(optarg,"%s",modelname)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'i':
				if (1!=sscanf(optarg,"%s",in_mshname)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				else build_model = false;
				break;
			case 'o':
				if (1!=sscanf(optarg,"%s",out_mshname)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'f':
				if (1!=sscanf(optarg,"%s",out_obserfile)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'n':
				if (1!=sscanf(optarg,"%s",noise_para)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'e':
				if (1!=sscanf(optarg,"%s",elename)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'v':
				if (1!=sscanf(optarg,"%s",mag_field_para)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 'p':
				if (1!=sscanf(optarg,"%s",obsername)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case 't':
				if (1!=sscanf(optarg,"%s",forward_type)){
					cout << "error ==> wrong format of " << optarg << endl;
				}
				break;
			case '?': //处理未定义或错误参数
				if (optopt == 'd' || optopt == 'm' || optopt == 'o' 
					|| optopt == 'e' || optopt == 'i' || optopt == 'p' 
					|| optopt == 'n' || optopt == 't' || optopt == 'v'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
					return -1;
				}
				else if (isprint(optopt)){
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
					return -1;
				}
				else{
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
					return -1;
				}
				break;
			default:
				abort();
		}
	}

	if (build_model){
		if (gm3d_instance.BuildRegularGrid(dimension)) return 0;
		if (gm3d_instance.AddModels(modelname)) return 0;
		gm3d_instance.RegisteredOuput(remove_null);
		if (gm3d_instance.OutMshFile(out_mshname,elename)) return 0;
	}
	else{
		if (gm3d_instance.ReadModel(in_mshname,elename)) return 0;
		if (gm3d_instance.InitObs(obsername)) return 0;
		if (!strcmp(forward_type,"Vz")){
			gm3d_instance.ForwardVz(noise_para);
		}
		else if (!strcmp(forward_type,"Vzx")){
			gm3d_instance.ForwardVzx(noise_para);
		}
		else if (!strcmp(forward_type,"Vzy")){
			gm3d_instance.ForwardVzy(noise_para);
		}
		else if (!strcmp(forward_type,"Vzz")){
			gm3d_instance.ForwardVzz(noise_para);
		}
		else if (!strcmp(forward_type,"DeltaT")){
			gm3d_instance.ForwardDeltaT(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"DeltaTx")){
			gm3d_instance.ForwardDeltaTx(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"DeltaTy")){
			gm3d_instance.ForwardDeltaTy(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"DeltaTz")){
			gm3d_instance.ForwardDeltaTz(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"Hax")){
			gm3d_instance.ForwardHax(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"Hay")){
			gm3d_instance.ForwardHay(noise_para,mag_field_para);
		}
		else if (!strcmp(forward_type,"Za")){
			gm3d_instance.ForwardZa(noise_para,mag_field_para);
		}
		else{
			cerr << BOLDYELLOW << "error ==> " << RESET << "wrong forward component type: " << forward_type << endl;
			return 0;
		}
		if (gm3d_instance.OutObs(out_obserfile)) return 0;
	}
	return 0;
}
