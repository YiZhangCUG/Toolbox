#include "stt_class.h"

int SttGenerator::set_pole_equator_radius(char* input_parameter){
	double ratio;
	// if input_parameter is NULL or empty, set pole_radius_ and equator_radius_ to DefaultR
	if (!strcmp(input_parameter,"NULL") || !strcmp(input_parameter,"")){
		pole_radius_ = equator_radius_ = DefaultR;
	}
	// use predefined values
	else if (!strcmp(input_parameter,"WGS84")){
		pole_radius_ = WGS84_r; equator_radius_ = WGS84_R;
	}
	else if (!strcmp(input_parameter,"Earth")){
		pole_radius_ = equator_radius_ = Earth_r;
	}
	else if (!strcmp(input_parameter,"Moon")){
		pole_radius_ = equator_radius_ = Moon_r;
	}
	// first try to read the input parameter as equator_radius_/pole_radius_
	// note that equator_radius_ and pole_radius_ must be bigger than zero
	else if (2 != sscanf(input_parameter,"%lf/%lf",&equator_radius_,&pole_radius_) || pole_radius_ <= 0.0 || equator_radius_ <= 0.0){
		// then try to read it as equator_radius_,ratio in which ratio = pole_radius_/equator_radius_
		// note the ratio must be bigger than zero but not necessarily smaller than or equal to one
		// However, for reality account, we set the limit of the ratio as 2.0
		if (2 == sscanf(input_parameter,"%lf,%lf",&equator_radius_,&ratio) && equator_radius_ > 0.0 && ratio > 0.0 && ratio <= 2.0){
			pole_radius_ = ratio * equator_radius_;
		}
		// all attempts fail, return -1
		else{
			cerr << BOLDRED << "Error ==> " << RESET << "fail to initialize the coordinate reference system: " << input_parameter << endl;
			return -1;
		}
	}
	// test output
	//clog << equator_radius_ << " " << pole_radius_ << endl;
	return 0;
}