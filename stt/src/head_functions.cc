#include "head_functions.h"

// degree version of sin() and cos()
inline double sind(double degree){
	return sin(degree*Pi/180.0);
}

inline double cosd(double degree){
	return cos(degree*Pi/180.0);
}
// calculate the semi-radius at a given latitude given the pole and equator radius of the reference system
double EllipsoidRadius(double latitude,double pole_radius,double equator_radius){
	return pole_radius*equator_radius/sqrt(pow(pole_radius,2)*pow(cosd(latitude),2)+pow(equator_radius,2)*pow(sind(latitude),2));
}
// Bilinear interpolation on the sphere
/*
 latitude
   |
   |
value21-------value22
   |             |
   |             |
   |      0      |
   |             |
   |             |
value11-------value12----> longitude
*/
double SphBilinearInterpolation(
	double latitude1,double latitude2,double longitude1,double longitude2,
	double latitude0,double longitude0,
	double value11,double value12,double value21,double value22){
	double colatitude1 = 90.0 - latitude1;
	double colatitude2 = 90.0 - latitude2;
	double colatitude0 = 90.0 - latitude0;
	double delta=(longitude2-longitude1)*(cosd(colatitude2)-cosd(colatitude1));
	double A=(longitude1*(value12-value22)+longitude2*(value21-value11))/delta;
	double B=(cosd(colatitude1)*(value21-value22)+cosd(colatitude2)*(value12-value11))/delta;
	double C=(value11+value22-value21-value12)/delta;
	double D=(longitude2*cosd(colatitude2)*value11-longitude2*cosd(colatitude1)*value21+longitude1*cosd(colatitude1)*value22-longitude1*cosd(colatitude2)*value12)/delta;
	return A*cosd(colatitude0)+B*longitude0+C*longitude0*cosd(colatitude0)+D;
}
// Convert a string to stringstream
stringstream Str2Ss(string input_string){
	stringstream sstr;
	sstr.str(""); sstr.clear(); sstr.str(input_string);
	return sstr;
}
// Check the existence of a input file and return the running status
int OpenInfile(ifstream &input_file,char* filename){
	input_file.open(filename);
	if (!input_file){
		cerr << BOLDRED << "Error ==> " << RESET << "file not found: " << filename << endl;
		return -1;
	}
	return 0;
}
// Check the existence of a output file and return the running status
int OpenOutfile(ofstream &output_file,char* filename){
	output_file.open(filename);
	if (!output_file){
		cerr << BOLDRED << "Error ==> " << RESET << "fail to create the file: " << filename << endl;
		return -1;
	}
	return 0;
}