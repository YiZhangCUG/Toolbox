#ifndef _STT_CLASS_H
#define _STT_CLASS_H
#include "head_functions.h"
#include "struct_functions.h"

class SttGenerator
{
public:
	SttGenerator(){}
	~SttGenerator(){}
	int set_command_record(int,char**);
	int set_tree_depth(char*);
	int set_pole_equator_radius(char*);
	int set_icosahedron_orient(char*);
	int Routine(char [][1024]); // for a 2D array. you must specify enough dimensional information to make it unique
	void InitialIcosahedron(double,Vertex); //初始化一个二十面体实例 需要给定一个默认半径值 二十面体顶点的经纬坐标 在init_para函数中调用
	void CreateBranch(int,int,int,int,int,int,QuadTreeNode**); //创建分枝
	void CreateTree(int,int,int,int,QuadTree*);//创建树
	void DeleteTree(QuadTreeNode**);//清空整颗树
	void ReturnLeaf(QuadTreeNode**);//返回叶子
	void ReturnDepth(QuadTreeNode**,int);
	void SortNeighbor(QuadTreeNodePointerArray); // sort neighboring relationship for input triangles
	void CutOutline(QuadTreeNode**);//切割模型范围 为了保证后续处理中树形结构的完整性 我们只添加node的属性值来控制是否输出该节点
	void CutHole(QuadTreeNode**); //cut holes in the created STT
	void CloseSurface(QuadTree**);
	int ReturnBranchDepth(QuadTreeNode**); //返回当前枝桠的最大深度
	int InTrianglePoint(QuadTreeNode*);//在球面上判断点和三角形关系的一种方法 直接使用矢量运算确定包含关系 更直接更简单
	int InTriangleLine(QuadTreeNode*);//判断插入线是否穿过节点三角形 使用的是球面下的方法 直接矢量计算 注意因为球面上的特殊关系 两个点之间的夹角不能大于等于180度 因为球面上总是沿着最短路径走 而且通常我们指的也是最短路径
	int InTrianglePolygon(QuadTreeNode*);//判断多边形与三角形的关系
	int InTriangleCircle(QuadTreeNode*);//判断圆与三角形的关系
	int OutPolyOutline(QuadTreeNode*);//判断多边形与三角形的关系 用于切割模型边界
	int InPolyOutline(QuadTreeNode*);//判断多边形与三角形的关系 用于切割模型边界 挖洞
	int OutputMshFile(char*,double,double);
	int OutputVertexLocation(char*,double,double);
	int OutputTriangleCenterLocation(char*,double,double);
	int OutputNeighbor(char*);
	int GetControlPoint(char*); //读取额外的点
	int GetControlCircle(char*); //读取额外的圆
	int GetControlLine(char*,ControlLineArray&); // Get control line arrays
private:
	// record input command line options for output records
	string command_record_;
	// minimal and maximal depths of quad-tree
	int tree_depth_, max_depth_;
	// pole and equator radius of the coordinate reference system
	double pole_radius_, equator_radius_;
	// orientation of the top vertex of the icosahedron
	Vertex icosahedron_orient_;
	// vertex array of the STT. This array defines the actual shape of the STT
	VertexArray array_stt_vert_;
	// map from the vertex's index to vertex. This map is used to find vertex by its index
	Int2VertexMap map_id_vertex_;
	Int2VertexMap::iterator ivd_;
	// map from the vertex's position to vertex. This map is used to find vertex by its position
	String2VertexMap map_str_vertex_;
	String2VertexMap::iterator ivm_;
	// base icosahedron used to construct the STT
	Icosahedron base_icosahedron_;
	// 20 quad-trees in which each of them represents the partition of one facet of the base icosahedron
	QuadTree* forest_[20];
	// pointer array of the extracted quad-tree nodes returned according to conditions
	QuadTreeNodePointerArray array_out_tri_pointer_;
	// external constraint information (point, line, polygons, circles, outline polygons and hole polygons)
	ControlPointArray array_control_point_;
	ControlCircleArray array_control_circle_;
	ControlLineArray array_control_line_;
	ControlLineArray array_control_polygon_;
	ControlLineArray array_outline_polygon_;
	ControlLineArray array_hole_polygon_;
};
#endif