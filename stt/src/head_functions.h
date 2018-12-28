#ifndef _HEAD_FUNCTIONS_H
#define _HEAD_FUNCTIONS_H
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

using namespace std;

// terminal controls
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
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

// define some mathematic constants
#define DBL_MAX 1.0e+30
#define DBL_MIN -1.0e+30
#define ZERO 1.0e-20
#define DefaultR 1e+5

// define some physical constants
// semi-radius (pole and equator) of the WGS84 reference system
#define WGS84_r 6356752.314
#define WGS84_R 6378137
// mean radius of the Earth and Moon
#define Earth_r 6371000
#define Moon_r 1738000
// Pi and the golden ratio
#define Pi (4.0*atan(1.0))
#define GoldenMean ((sqrt(5.0)+1)/2)
// Universal gravitational constant
#define G0 6.67408e-11

// Macro functions
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define SetToBox(low,high,input) (MAX(low,MIN(high,input)))

// Define global functions
// degree version of sin() and cos()
inline double sind(double);
inline double cosd(double);
// calculate the semi-radius at a given latitude given the pole and equator radius of the reference system
double EllipsoidRadius(double,double,double);
// Bilinear interpolation on the spherical sphere
double SphBilinearInterpolation(double,double,double,double,double,double,double,double,double,double);
// Convert a string to stringstream
stringstream Str2Ss(string);
// Check the existence of a input file and return the running status
int OpenInfile(ifstream&,char*);
// Check the existence of a output file and return the running status
int OpenOutfile(ofstream&,char*);
#endif