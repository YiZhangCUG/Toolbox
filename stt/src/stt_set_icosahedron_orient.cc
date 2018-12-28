#include "stt_class.h"

int SttGenerator::set_icosahedron_orient(char* input_parameter){
	// if input_parameter is NULL or empty, set icosahedron_orient_ to the positive direction of z-axis
	if (!strcmp(input_parameter,"NULL") || !strcmp(input_parameter,"")){
		icosahedron_orient_.posis.lon = 0.0; icosahedron_orient_.posis.lat = 90.0;
	}
	// try to read the input parameter as icosahedron_orient_.posis.lon/icosahedron_orient_.posis.lat along with some boundary checks
	else if (2!=sscanf(input_parameter,"%lf/%lf",&icosahedron_orient_.posis.lon,&icosahedron_orient_.posis.lat) 
		|| icosahedron_orient_.posis.lon < -180.0 || icosahedron_orient_.posis.lon > 180.0
		|| icosahedron_orient_.posis.lat < -90.0 || icosahedron_orient_.posis.lat > 90.0){
		cerr << BOLDRED << "Error ==> " << RESET << "fail to initialize the orient of the base icosahedron: " << input_parameter << endl;
		return -1;
	}

	icosahedron_orient_.posis.rad = DefaultR;
	icosahedron_orient_.posic = Sphere2Cartesian(icosahedron_orient_.posis);
	return 0;
}