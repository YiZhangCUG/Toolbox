#include "stt_class.h"

/**
 * 初始化二十面实例ICOSA
 * @param radius   二十面的顶点半径
 * @param orient 二十面最高点顶点方位
 */
void SttGenerator::InitialIcosahedron(double radius,Vertex orient)
{
	double constL, constZ;
	//计算二十面的十二个顶点位置 首先将顶点位置固定在地理北极点
	base_icosahedron_.vert[0].id = 0;
	base_icosahedron_.vert[0].posic.x = 0.0;
	base_icosahedron_.vert[0].posic.y = 0.0;
	base_icosahedron_.vert[0].posic.z = radius;
	base_icosahedron_.vert[0].posis = Cartesian2Sphere(base_icosahedron_.vert[0].posic);
	base_icosahedron_.vert[11].id = 11;
	base_icosahedron_.vert[11].posic.x = 0.0;
	base_icosahedron_.vert[11].posic.y = 0.0;
	base_icosahedron_.vert[11].posic.z = -1.0*radius;
	base_icosahedron_.vert[11].posis = Cartesian2Sphere(base_icosahedron_.vert[11].posic);

	constZ = radius*(GoldenMean*GoldenMean - 1.0)/(GoldenMean*GoldenMean + 1.0);
	constL = radius*sqrt(1.0 - pow((GoldenMean*GoldenMean - 1.0)/(GoldenMean*GoldenMean + 1.0),2));

	for(int i = 1; i <= 5; i++)
	{
		base_icosahedron_.vert[i].id = i;
		base_icosahedron_.vert[i].posic.x = cos(72.0*(i-1)*Pi/180.0)*constL;
		base_icosahedron_.vert[i].posic.y = sin(72.0*(i-1)*Pi/180.0)*constL;
		base_icosahedron_.vert[i].posic.z = constZ;
		base_icosahedron_.vert[i].posis = Cartesian2Sphere(base_icosahedron_.vert[i].posic);

		base_icosahedron_.vert[i+5].id = i+5;
		base_icosahedron_.vert[i+5].posic.x = cos((72.0*(i-1)+36.0)*Pi/180.0)*constL;
		base_icosahedron_.vert[i+5].posic.y = sin((72.0*(i-1)+36.0)*Pi/180.0)*constL;
		base_icosahedron_.vert[i+5].posic.z = -constZ;
		base_icosahedron_.vert[i+5].posis = Cartesian2Sphere(base_icosahedron_.vert[i+5].posic);
	}

	//给定二十面的面顶点索引，各个三角面顶点索引按逆时针排序
	base_icosahedron_.tri[0].ids[0] = 0; base_icosahedron_.tri[0].ids[1] = 1; base_icosahedron_.tri[0].ids[2] = 2;
	base_icosahedron_.tri[1].ids[0] = 0; base_icosahedron_.tri[1].ids[1] = 2; base_icosahedron_.tri[1].ids[2] = 3;
	base_icosahedron_.tri[2].ids[0] = 0; base_icosahedron_.tri[2].ids[1] = 3; base_icosahedron_.tri[2].ids[2] = 4;
	base_icosahedron_.tri[3].ids[0] = 0; base_icosahedron_.tri[3].ids[1] = 4; base_icosahedron_.tri[3].ids[2] = 5;
	base_icosahedron_.tri[4].ids[0] = 0; base_icosahedron_.tri[4].ids[1] = 5; base_icosahedron_.tri[4].ids[2] = 1;
	base_icosahedron_.tri[5].ids[0] = 1; base_icosahedron_.tri[5].ids[1] = 6; base_icosahedron_.tri[5].ids[2] = 2;
	base_icosahedron_.tri[6].ids[0] = 2; base_icosahedron_.tri[6].ids[1] = 6; base_icosahedron_.tri[6].ids[2] = 7;
	base_icosahedron_.tri[7].ids[0] = 2; base_icosahedron_.tri[7].ids[1] = 7; base_icosahedron_.tri[7].ids[2] = 3;
	base_icosahedron_.tri[8].ids[0] = 3; base_icosahedron_.tri[8].ids[1] = 7; base_icosahedron_.tri[8].ids[2] = 8;
	base_icosahedron_.tri[9].ids[0] = 3; base_icosahedron_.tri[9].ids[1] = 8; base_icosahedron_.tri[9].ids[2] = 4;
	base_icosahedron_.tri[10].ids[0] = 4; base_icosahedron_.tri[10].ids[1] = 8; base_icosahedron_.tri[10].ids[2] = 9;
	base_icosahedron_.tri[11].ids[0] = 4; base_icosahedron_.tri[11].ids[1] = 9; base_icosahedron_.tri[11].ids[2] = 5;
	base_icosahedron_.tri[12].ids[0] = 5; base_icosahedron_.tri[12].ids[1] = 9; base_icosahedron_.tri[12].ids[2] = 10;
	base_icosahedron_.tri[13].ids[0] = 5; base_icosahedron_.tri[13].ids[1] = 10; base_icosahedron_.tri[13].ids[2] = 1;
	base_icosahedron_.tri[14].ids[0] = 1; base_icosahedron_.tri[14].ids[1] = 10; base_icosahedron_.tri[14].ids[2] = 6;
	base_icosahedron_.tri[15].ids[0] = 6; base_icosahedron_.tri[15].ids[1] = 11; base_icosahedron_.tri[15].ids[2] = 7;
	base_icosahedron_.tri[16].ids[0] = 7; base_icosahedron_.tri[16].ids[1] = 11; base_icosahedron_.tri[16].ids[2] = 8;
	base_icosahedron_.tri[17].ids[0] = 8; base_icosahedron_.tri[17].ids[1] = 11; base_icosahedron_.tri[17].ids[2] = 9;
	base_icosahedron_.tri[18].ids[0] = 9; base_icosahedron_.tri[18].ids[1] = 11; base_icosahedron_.tri[18].ids[2] = 10;
	base_icosahedron_.tri[19].ids[0] = 10; base_icosahedron_.tri[19].ids[1] = 11; base_icosahedron_.tri[19].ids[2] = 6;

	//旋转二十面顶点的位置
	Vertex ref_vert = base_icosahedron_.vert[0]; //注意我们选取的参考点为z轴正方向
	for (int i = 0; i < 12; i++)
	{
		base_icosahedron_.vert[i] = RotateVertex(ref_vert,orient,base_icosahedron_.vert[i]);
	}
	return;
}