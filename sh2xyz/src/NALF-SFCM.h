// Fully normalized associated Legendre functions calculated by standard forward column methods
// Holmes, S. A., & Featherstone, W. E. (2002).
// A unified approach to the Clenshaw summation and the recursive computation of very high degree and order normalised associated Legendre functions.
// Journal of Geodesy, 76(5), 279–299. https://doi.org/10.1007/s00190-002-0216-2
// Author: Yi Zhang (zhangyi.cugwuhan@gmail.com)
#ifndef _NALF_SFCM_H
#define _NALF_SFCM_H
#include "sysDefine.h"
//计算向前列推的系数 避免重复计算 这里不要使用vector at() 速度比较慢 直接使用[] 注意调用size()函数也会降低执行效率
_2dArray get_a_nm_array(int MaxOrder)
{
	int i,j;
	_2dArray cs;
	cs.resize(MaxOrder);
	for (i = 0; i < MaxOrder; i++)
		cs[i].resize(i+1);
	//向下列推计算
#pragma omp parallel for private(i,j) schedule(guided)
	for (j = 0; j < MaxOrder; j++)
	{
		cs[j][j] = 0; //对角线上的值直接给0 反正用不到
		for (i = j+1; i < MaxOrder; i++)
		{
			cs[i][j] = sqrt(((2.0*i-1)*(2.0*i+1))/((i-j)*(i+j)));
		}
	}
	return cs;
}

_2dArray get_b_nm_array(int MaxOrder)
{
	int i,j;
	_2dArray cs;
	cs.resize(MaxOrder);
	for (i = 0; i < MaxOrder; i++)
		cs[i].resize(i+1);
	//向下列推计算
#pragma omp parallel for private(i,j) schedule(guided)
	for (j = 0; j < MaxOrder; j++)
	{
		cs[j][j] = 0; //对角线上的值直接给0 反正用不到
		for (i = j+1; i < MaxOrder; i++)
		{
			cs[i][j] = sqrt(((2.0*i+1)*(i+j-1)*(i-j-1))/((i-j)*(i+j)*(2.0*i-3)));
		}
	}
	return cs;
}

// 计算标准前向列推法计算规格化的勒让德多项式 输入参数为需要计算的最大阶次MaxOrder 余纬度theta(度) 返回一个下半三角二维数组
// 最大维度为MaxOrder+1，二维数组中行数代表阶数列数为次数
_2dArray NALF_SFCM(int MaxOrder,_2dArray a_nm,_2dArray b_nm,double theta)
{
	//声明数组 初始化一个下半三角二维数组
	_2dArray nalf;
	nalf.resize(MaxOrder);
	for (int i = 0; i < MaxOrder; i++)
		nalf.at(i).resize(i+1);

	//赋初值给前两个对角线上的值
	nalf.at(0).at(0) = 1.0;
	nalf.at(1).at(1) = sqrt(3.0)*sin(theta*pi/180.0);
	//计算对角线上的值 递归计算 不能并行
	for (int i = 2; i < nalf.size(); i++)
		nalf.at(i).at(i) = sin(theta*pi/180.0)*sqrt(0.5*(2.0*i+1)/i)*nalf.at(i-1).at(i-1);

	//声明系数和迭代变量
	int i,j;
	double Pn_2m,Pn_1m; //Pn-1,m Pn-2,m
	//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
#pragma omp parallel for private(i,j,Pn_2m,Pn_1m) schedule(guided)
	for (j = 0; j < nalf.size()-1; j++)
	{
		Pn_2m = 0;
		Pn_1m = nalf.at(j).at(j);
		for (i = j+1; i < nalf.size(); i++)
		{
			nalf.at(i).at(j) = a_nm.at(i).at(j)*cos(theta*pi/180.0)*Pn_1m - b_nm.at(i).at(j)*Pn_2m;

			Pn_2m = nalf.at(i-1).at(j);
			Pn_1m = nalf.at(i).at(j);
		}
	}
	return nalf;
}

// 计算标准前向列推法计算规格化的勒让德多项式 输入参数 一个下半三角二维矩阵 余纬度theta(度) 无返回值
// 二维数组中行数代表阶数列数为次数
void NALF_SFCM2(_2dArray& nalf,_2dArray a_nm,_2dArray b_nm,double theta)
{
	//赋初值给前两个对角线上的值
	nalf.at(0).at(0) = 1.0;
	nalf.at(1).at(1) = sqrt(3.0)*sin(theta*pi/180.0);
	//计算对角线上的值 递归计算 不能并行
	for (int i = 2; i < nalf.size(); i++)
		nalf.at(i).at(i) = sin(theta*pi/180.0)*sqrt(0.5*(2.0*i+1)/i)*nalf.at(i-1).at(i-1);

	//声明系数和迭代变量
	int i,j;
	double Pn_2m,Pn_1m; //Pn-1,m Pn-2,m
	//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
#pragma omp parallel for private(i,j,Pn_2m,Pn_1m) schedule(guided)
	for (j = 0; j < nalf.size()-1; j++)
	{
		Pn_2m = 0;
		Pn_1m = nalf.at(j).at(j);
		for (i = j+1; i < nalf.size(); i++)
		{
			nalf.at(i).at(j) = a_nm.at(i).at(j)*cos(theta*pi/180.0)*Pn_1m - b_nm.at(i).at(j)*Pn_2m;

			Pn_2m = nalf.at(i-1).at(j);
			Pn_1m = nalf.at(i).at(j);
		}
	}
}

void NALF_SFCM3(_2dArray& nalf,_2dArray a_nm,_2dArray b_nm,int maxOrder,double theta,double norSum)
{
	//赋初值给前两个对角线上的值
	//norSum为1时第一个值为1/sqrt(4.0*pi),归一化值为1, norSum为4.0*pi时第一个值为4.0*pi/sqrt(4.0*pi)=1,归一化值为4.0*pi
	nalf[0][0] = sqrt(norSum)/sqrt(4.0*pi);
	nalf[1][1] = sqrt(3.0)*sin(theta*pi/180.0);
	//计算对角线上的值 递归计算 不能并行
	for (int i = 2; i < maxOrder; i++)
		nalf[i][i] = sin(theta*pi/180.0)*sqrt(0.5*(2.0*i+1)/i)*nalf[i-1][i-1];

	//计算次对角线(m+1,m)上的值 递归计算 不能并行
	for (int i = 0; i < maxOrder-1; i++)
		nalf[i+1][i] = cos(theta*pi/180.0)*sqrt(2.0*i+3)*nalf[i][i];

	//声明系数和迭代变量
	int i,j;
	//这里可以使用并行加速计算外层循环 内层计算因为是递归计算因此不能并行
#pragma omp parallel for private(i,j) schedule(guided)
	for (j = 0; j < maxOrder-1; j++)
	{
		for (i = j+2; i < maxOrder; i++)
		{
			nalf[i][j] = a_nm[i][j]*cos(theta*pi/180.0)*nalf[i-1][j] - b_nm[i][j]*nalf[i-2][j];
		}
	}
}
#endif