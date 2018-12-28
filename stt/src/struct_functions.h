#ifndef _STRUCT_FUNCTIONS_H
#define _STRUCT_FUNCTIONS_H
#include "head_functions.h"

// array structures
typedef vector<int> IntArray1D; //1D int array
typedef vector<vector<int> > IntArray2D; // 2D int array
typedef vector<double> DoubleArray1D; // 1D double array
// map structures
typedef map<int,int> Int2IntMap; // int to int map
// point structures in the Cartesian and spherical coordinates
struct Cpoint{
	double x = DBL_MAX, y = DBL_MAX, z = DBL_MAX;
};
typedef vector<Cpoint> CpointArray;
// lon for longitude, lat for latitude and rad for radius
struct Spoint{
	double lon = DBL_MAX, lat = DBL_MAX, rad = DBL_MAX;
};
typedef vector<Spoint> SpointArray;
// vertex structure
struct Vertex{
	int id = -1;
	Cpoint posic; //position under the Cartesian coordinates
	Spoint posis; //position under the sphere coordinates
};
typedef vector<Vertex> VertexArray;
typedef map<int,Vertex> Int2VertexMap;
typedef map<string,Vertex> String2VertexMap;
// triangle structure
struct Triangle{
	int ids[3] = {-1,-1,-1}; // index s
	int physic_group = 0;
};
typedef vector<Triangle> TriangleArray;
// icosahedron structure
struct Icosahedron{
	Vertex vert[12]; // vert for vertex
	Triangle tri[20]; // tir for triangle
};
// Quad-tree node structure
struct QuadTreeNode{
	int id = -1, depth = -1;
	bool out_ok = true;
	Triangle* tri = new Triangle; // tri for triangle
	QuadTreeNode* children[4] = {NULL,NULL,NULL,NULL};
	QuadTreeNode* neighbor[3] = {NULL,NULL,NULL};
};
typedef vector<QuadTreeNode*> QuadTreeNodePointerArray;
// quad-tree structure
struct QuadTree{
	QuadTreeNode* root;
};
// control points, lines(polygons) and circles
struct ControlPoint{
	int id = -1, max_depth = -1, physic_group = 0;
	double minimal_resolution = DBL_MAX;
	Vertex vert; // vert for vertex
};
typedef vector<ControlPoint> ControlPointArray;

struct ControlLine{
	int id = -1, max_depth = -1, physic_group = 0;
	double minimal_resolution = -1.0;
	VertexArray vert; // verts for vertices
};
typedef vector<ControlLine> ControlLineArray;

struct ControlCircle{
	int id = -1, max_depth = -1, physic_group = 0;
	double minimal_resolution = -1.0, circle_cap_degree = -1.0;
	Vertex circle_center;
};
typedef vector<ControlCircle> ControlCircleArray;
// mathematic functions regarding declared structures
bool operator ==(Cpoint,Cpoint);
bool operator !=(Cpoint,Cpoint);
Cpoint operator +(Cpoint,Cpoint);
Cpoint operator -(Cpoint,Cpoint);
Cpoint operator *(double,Cpoint);
double DotProduct(Cpoint,Cpoint);
Cpoint CrossProduct(Cpoint,Cpoint);
Cpoint CloudCenter(VertexArray);
double ModuleLength(Cpoint);
double ProjectAngle(Cpoint,Cpoint);
Cpoint Sphere2Cartesian(Spoint);
Spoint Cartesian2Sphere(Cpoint);
Vertex RotateVertex(Vertex,Vertex,Vertex);
Cpoint LineCrossPlane(Cpoint,Cpoint,Cpoint);
string GetStringIndex(Vertex);
int LocalIndex(int,Triangle);
#endif