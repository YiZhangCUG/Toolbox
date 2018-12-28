#include "struct_functions.h"

// mathematic functions regarding declared structures
bool operator ==(Cpoint a,Cpoint b){
	if(fabs(a.x-b.x)<ZERO&&fabs(a.y-b.y)<ZERO&&fabs(a.z-b.z)<ZERO){
		return 1;
	}
	else return 0;
}

bool operator !=(Cpoint a,Cpoint b){
	if(fabs(a.x-b.x)>=ZERO || fabs(a.y-b.y)>=ZERO || fabs(a.z-b.z)>=ZERO){
		return 1;
	}
	else return 0;
}

Cpoint operator +(Cpoint a,Cpoint b){
	Cpoint m;
	m.x=a.x+b.x; m.y=a.y+b.y; m.z=a.z+b.z;
	return m;
}

Cpoint operator -(Cpoint a,Cpoint b){
	Cpoint m;
	m.x=a.x-b.x; m.y=a.y-b.y; m.z=a.z-b.z;
	return m;
}

Cpoint operator *(double sign,Cpoint b){
	Cpoint m;
	m.x=sign*b.x; m.y=sign*b.y; m.z=sign*b.z;
	return m;
}

double DotProduct(Cpoint a,Cpoint b){
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

Cpoint CrossProduct(Cpoint a,Cpoint b){
	Cpoint v;
	v.x = a.y*b.z-a.z*b.y;
	v.y = a.z*b.x-a.x*b.z;
	v.z = a.x*b.y-a.y*b.x;
	return v;
}

Cpoint CloudCenter(VertexArray input_verts){
	Cpoint c;
	c.x = 0; c.y = 0; c.z = 0;
	if (!input_verts.empty()){
		for (int i = 0; i < input_verts.size(); i++){
			c.x += input_verts[i].posic.x;
			c.y += input_verts[i].posic.y;
			c.z += input_verts[i].posic.z;
		}
		c.x /= input_verts.size();
		c.y /= input_verts.size();
		c.z /= input_verts.size();
	}
	return c;
}

double ModuleLength(Cpoint v){
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

double ProjectAngle(Cpoint a,Cpoint b){
	return acos((a.x*b.x+a.y*b.y+a.z*b.z)/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)*sqrt(b.x*b.x+b.y*b.y+b.z*b.z)))*180.0/Pi;
}

Cpoint Sphere2Cartesian(Spoint s){
	Cpoint c;
	c.x = s.rad*sin((0.5 - s.lat/180.0)*Pi)*cos((2.0 + s.lon/180.0)*Pi);
	c.y = s.rad*sin((0.5 - s.lat/180.0)*Pi)*sin((2.0 + s.lon/180.0)*Pi);
	c.z = s.rad*cos((0.5 - s.lat/180.0)*Pi);
	return c;
}

Spoint Cartesian2Sphere(Cpoint c){
	Spoint s;
	s.rad = sqrt(pow(c.x,2)+pow(c.y,2)+pow(c.z,2));
	//点距离原点极近 将点置于原点
	if (fabs(s.rad)<ZERO){
		s.lat = s.lon = s.rad = 0.0;
	}
	else{
		s.lat = 90.0 - acos(c.z/s.rad)*180.0/Pi;
		s.lon = atan2(c.y,c.x)*180.0/Pi;
	}
	return s;
}

Vertex RotateVertex(Vertex olda,Vertex newa,Vertex oldb){
	Vertex newb;
	Vertex temp_ref,temp_b;
	double yz_angle = (newa.posis.lat - olda.posis.lat)*Pi/180.0;
	//首先绕olda.lon即x轴旋转oldb
	temp_b.posic.x = oldb.posic.x;
	temp_b.posic.y = oldb.posic.y*cos(-1.0*yz_angle)+oldb.posic.z*sin(-1.0*yz_angle);
	temp_b.posic.z = oldb.posic.z*cos(-1.0*yz_angle)-oldb.posic.y*sin(-1.0*yz_angle);
	temp_b.posis = Cartesian2Sphere(temp_b.posic);
	//计算绕x轴旋转后olda的位置 这是后一步旋转需要的参考值
	temp_ref.posic.x = olda.posic.x;
	temp_ref.posic.y = olda.posic.y*cos(-1.0*yz_angle)+olda.posic.z*sin(-1.0*yz_angle);
	temp_ref.posic.z = olda.posic.z*cos(-1.0*yz_angle)-olda.posic.y*sin(-1.0*yz_angle);
	temp_ref.posis = Cartesian2Sphere(temp_ref.posic);
	//注意绕z轴旋转的经度参考位置为olda绕x轴旋转后的经度值
	double xy_angle = (newa.posis.lon - temp_ref.posis.lon)*Pi/180.0;
	//绕z轴旋转temp_b z值不变
	newb.id = oldb.id;
	newb.posic.x = temp_b.posic.x*cos(-1.0*xy_angle)+temp_b.posic.y*sin(-1.0*xy_angle);
	newb.posic.y = temp_b.posic.y*cos(-1.0*xy_angle)-temp_b.posic.x*sin(-1.0*xy_angle);
	newb.posic.z = temp_b.posic.z;
	newb.posis = Cartesian2Sphere(newb.posic);
	return newb;
}

Cpoint LineCrossPlane(Cpoint c,Cpoint normal,Cpoint p){
	Cpoint m;
	m.x = 0; m.y = 0; m.z = 0;
	double t;
	if (DotProduct(normal,p) != 0) //平面与矢量平行
	{
		t = DotProduct(normal,c)/DotProduct(normal,p);
		m.x += p.x*t;
		m.y += p.y*t;
		m.z += p.z*t;
	}
	return m;
}

string GetStringIndex(Vertex input_vertex)
{
	stringstream sstemp;
	string vert_id,mid_id;

	sstemp.str(""); sstemp.clear();
	sstemp << setprecision(16) << input_vertex.posic.x;
	sstemp >> vert_id;
	sstemp.str(""); sstemp.clear();
	sstemp << setprecision(16) << input_vertex.posic.y;
	sstemp >> mid_id;
	vert_id = vert_id + " " + mid_id;
	sstemp.str(""); sstemp.clear();
	sstemp << setprecision(16) << input_vertex.posic.z;
	sstemp >> mid_id;
	vert_id = vert_id + " " + mid_id;
	return vert_id;
}

int LocalIndex(int id, Triangle t)
{
	for (int i = 0; i < 3; i++)
		if (id == t.ids[i])	return i;
	return -1;
}