#include "stt_class.h"

void disp_help(char* proname){
	string exe_name = proname;
	exe_name = " " + exe_name + 
" -d<minimal-depth>/<maximal-depth> \
[-r'WGS84'|'Earth'|'Moon'|<equator-radius>/<pole-radius>|<equator_radius>,<flat-rate>] \
[-o<orient-longitude>/<orient-latitude>] \
[-m<output-msh-filename>] \
[-v<output-vert-loc-filename>] \
[-t<output-tri-cen-filename>] \
[-n<output-tri-neg-filename>] \
[-p<control-point-filename>] \
[-l<control-line-filename>] \
[-g<control-poly-filename>] \
[-c<control-circle-filename>] \
[-s<outline-shape-filename>] \
[-k<hole-shape-filename>] \
[-h]";
	clog << proname << " - v1.0 Spherical Triangular Tessellation(STT) generator." << endl;
	clog << "Usage: " << exe_name << endl;
	clog << "Options:" << endl;
	clog << "\t-d\tMinimal and maximal quad-tree depths of the output STT." << endl;
	clog << "\t-r\tCoordinate reference system of the output STT, the default is 1e+5/1e+5." << endl;
	clog << "\t-o\tOrientation of the top vertex of the base icosahedron, the default is 0/90." << endl;
	clog << "\t-m\tOutput Gmsh(.msh) filename." << endl;
	clog << "\t-v\tOutput vertex location(.txt) filename." << endl;
	clog << "\t-t\tOutput triangle center location(.txt) filename." << endl;
	clog << "\t-n\tOutput triangle neighbor(.txt) filename." << endl;
	clog << "\t-p\tInput control point location(.txt) filename." << endl;
	clog << "\t-l\tInput control line location(.txt) filename." << endl;
	clog << "\t-g\tInput control polygon location(.txt) filename." << endl;
	clog << "\t-c\tInput control circle location(.txt) filename." << endl;
	clog << "\t-s\tInput outline polygon location(.txt) filename." << endl;
	clog << "\t-k\tInput hole polygon location(.txt) filename." << endl;
	clog << "\t-h\tShow help information." << endl;
}

int main(int argc, char* argv[]){
	/*map of input options
	0 -> tree depths
	1 -> coordinate reference system
	2 -> orientation of the base icosahedron
	3 -> output filename for the constructed model (Gmsh .msh file)
	4 -> output filename for vertex locations
	5 -> output filename for triangle's center locations
	6 -> output filename for triangle's neighboring index
	7 -> input filename for point constraints
	8 -> input filename for line constraints
	9 -> input filename for polygon constraints
	10-> input filename for circle constraints
	11-> input filename for outline shape constraints
	12-> input filename for hole shape constraints*/
	char input_options[13][1024];
	for (int i = 0; i < 13; i++){
		strcpy(input_options[i],"NULL");
	}

	// show help information is no options is read
	if (argc == 1){
		disp_help(argv[0]);
		return 0;
	}

	int curr, option_number;
	while((curr = getopt(argc,argv,"hd:r:o:m:v:t:n:p:l:g:c:s:k:")) != -1){
		// get option number
		switch (curr){
			case 'h': // show help information
				disp_help(argv[0]); return 0;
			case 'd':
				option_number = 0; break;
			case 'r':
				option_number = 1; break;
			case 'o':
				option_number = 2; break;
			case 'm':
				option_number = 3; break;
			case 'v':
				option_number = 4; break;
			case 't':
				option_number = 5; break;
			case 'n':
				option_number = 6; break;
			case 'p':
				option_number = 7; break;
			case 'l':
				option_number = 8; break;
			case 'g':
				option_number = 9; break;
			case 'c':
				option_number =10; break;
			case 's':
				option_number =11; break;
			case 'k':
				option_number =12; break;
			case '?': //处理未定义或错误参数
				if (optopt == 'd' || optopt == 'r' || optopt == 'o' || optopt == 'm' || optopt == 'n'
					|| optopt == 'v' || optopt == 't' || optopt == 'p' || optopt == 'l'
					|| optopt == 'g' || optopt == 'c' || optopt == 's' || optopt == 'k'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
					return 0;
				}
				else if (isprint(optopt)){
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
					return 0;
				}
				else{
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
					return 0;
				} break;
			default:
				abort();
		}

		if (1!=sscanf(optarg,"%s",input_options[option_number])){
			cerr<<BOLDRED<<"Error ==> "<<RESET<<"bad syntax: "<<optarg<<endl;
			return 0;
		}
	}

	SttGenerator instance;
	// record commands
	instance.set_command_record(argc,argv);
	// generate stt model
	instance.Routine(input_options);
	return 0;
}