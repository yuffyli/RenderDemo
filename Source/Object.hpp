//
//  Object.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>

#include "Macro.hpp"
#include "Vector3.hpp"
#include "Camera.hpp"

class Texture;

// 顶点
struct Vertex
{
	Point3 pos;
	float w;
	float tu, tv;
	//float r, g, b, a;
	float rhw;

	Vertex &operator = (const Vertex &rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		pos = rhs.pos;
		w = rhs.w;
		tu = rhs.tu;
		rhw = rhs.rhw;

		return *this;
	}

	Vertex operator + (const Vertex &rhs) const
	{
		Vertex res;
		res.pos = pos + rhs.pos;
		res.w = w + rhs.w;
		res.tu = tu + rhs.tu;
		//res.r = r + rhs.r;
		//res.g = g + rhs.g;
		//res.b = b + rhs.b;
		//res.a = a + rhs.a;
		res.rhw = rhw + rhs.rhw;

		return res;
	}

	Vertex &operator += (const Vertex &rhs)
	{
		pos += rhs.pos;
		w += rhs.w;
		tu += rhs.tu;
		tv += rhs.tv;
		//r += rhs.r;
		//g += rhs.g;
		//b += rhs.b;
		//a += rhs.a;
		rhw += rhs.rhw;

		return *this;
	}
	
	Vertex operator - (const Vertex &rhs) const
	{
		Vertex res;
		res.pos = pos - rhs.pos;
		res.w = w - rhs.w;
		res.tu = tu - rhs.tu;
		//res.r = r - rhs.r;
		//res.g = g - rhs.g;
		//res.b = b - rhs.b;
		//res.a = a - rhs.a;
		res.rhw = rhw - rhs.rhw;

		return res;
	}

	Vertex &operator -= (const Vertex &rhs)
	{
		pos -= rhs.pos;
		w -= rhs.w;
		tu -= rhs.tu;
		tv -= rhs.tv;
		//r -= rhs.r;
		//g -= rhs.g;
		//b -= rhs.b;
		//a -= rhs.a;
		rhw -= rhs.rhw;

		return *this;
	}

	Vertex operator * (float k) const
	{
		Vertex res;
		res.pos *= k;
		res.w *= k;
		res.tu *= k;
		res.tv *= k;
		//res.r *= k;
		//res.g *= k;
		//res.b *= k;
		//res.a *= k;
		res.rhw *= k;

		return res;
	}

	Vertex &operator *= (float k)
	{
		pos *= k;
		w *= k;
		tu *= k;
		tv *= k;
		//r *= k;
		//g *= k;
		//b *= k;
		//a *= k;
		rhw *= k;

		return *this;
	}

};

// 边
struct Edge
{
	Vertex vInfo;
	Vertex vStart;
	Vertex vEnd;
};

// 梯形
struct Trapezoid
{
	float fTop;
	float fBottom;
	Edge eLeft;
	Edge eRight;
};

// 线段（水平）
struct Line
{
	int32_t nXStart;	// 起点的x坐标
	int32_t nY;	// 线段的y坐标
	int32_t nLength;	// 线段长度
	Vertex v;	
	Vertex step;	// 步长
};

// 多边形
struct Poly
{
	Vertex *vertexList;    // 顶点列表
	Texture *pTexture;	// 纹理
	int vertexIndex[4];    // 顶点索引
	int32_t nState;          // 多边形状态
	int32_t color;            // 多边形颜色
};

class Object
{
public:
	void init();
    void reset();

	// 物体剔除
    void cullObject(const Camera &cam, int nCullFlag);

	// 背面消除
    void removeBackfaces(const Camera &cam);

	// 物体坐标到世界坐标的转换
	void worldTransform();

	// 相机变换
	void cameraTransform(const Camera &cam);

	// 投影变换
	void projectTransform(const Camera &cam);

	// 屏幕变换
	void screenTranform(const Camera &cam);
    
public:
    int nState;     // 物体状态
    
    Point3 posWorld;   // 世界坐标
    Vector3 vecDirection;   // 局部坐标中的朝向
    Vector3 axisX, axisY, axisZ;    // 局部坐标轴
    float fMaxRadius;   // 包围球半径
    
    int nPolyCnt;   // 多边形数

    Poly polyList[OBJECT_MAX_POLYS];
    
    int nVerticesCnt;   // 顶点数
	

    Vertex vertexListLocal[OBJECT_MAX_VERTICES];    // 顶点的局部坐标数组
    Vertex vertexListTrans[OBJECT_MAX_VERTICES];    // 顶点变换后的坐标数组
};


//struct RenderList
//{
//    int nPolyCnt;   // 多边形数
//    
//    Poly *polyHanleList[RENDERLIST_MAX_POLYS];
//    Poly polyDataList[RENDERLIST_MAX_POLYS];
//
//};



#endif /* Object_hpp */
