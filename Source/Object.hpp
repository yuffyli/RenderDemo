//
//  Object.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "Macro.hpp"
#include "Vector3.hpp"
#include "Camera.hpp"

class Texture;


struct Color
{
	float r, g, b;

	Color operator + (const Color &rhs) const
	{
		Color res;
		res.r = r + rhs.r;
		res.g = g + rhs.g;
		res.b = b + rhs.b;

		return res;
	}

	Color &operator += (const Color &rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;

		return *this;
	}

	Color operator - (const Color &rhs) const
	{
		Color res;
		res.r = r - rhs.r;
		res.g = g - rhs.g;
		res.b = b - rhs.b;

		return res;
	}

	Color &operator -= (const Color &rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;

		return *this;
	}

	Color operator * (float k) const
	{
		Color res;
		res.r = r * k;
		res.g = g * k;
		res.b = b * k;

		return res;
	}

	Color &operator *= (float k)
	{
		r *= k;
		g *= k;
		b *= k;

		return *this;
	}
};

// 顶点
struct LocalVertex
{
	Point3 pos;
	float tu,tv;
	Color color;
};

struct Vertex
{
	float w;
	float tu, tv;
	float rhw;

	Point3 pos;
	Color color;

	Vertex &operator = (const Vertex &rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		pos = rhs.pos;
		w = rhs.w;
		tu = rhs.tu;
		tv = rhs.tv;
		color = rhs.color;
		rhw = rhs.rhw;

		return *this;
	}

	Vertex operator + (const Vertex &rhs) const
	{
		Vertex res;

		res.pos = pos + rhs.pos;
		res.w = w + rhs.w;
		res.tu = tu + rhs.tu;
		res.tv = tv + rhs.tv;
		res.color = color + rhs.color;
		res.rhw = rhw + rhs.rhw;

		return res;
	}

	Vertex &operator += (const Vertex &rhs)
	{
		pos += rhs.pos;
		w += rhs.w;
		tu += rhs.tu;
		tv += rhs.tv;
		color += rhs.color;
		rhw += rhs.rhw;

		return *this;
	}
	
	Vertex operator - (const Vertex &rhs) const
	{
		Vertex res;

		res.pos = pos - rhs.pos;
		res.w = w - rhs.w;
		res.tu = tu - rhs.tu;
		res.tv = tv - rhs.tv;
		res.color = color - rhs.color;
		res.rhw = rhw - rhs.rhw;

		return res;
	}

	Vertex &operator -= (const Vertex &rhs)
	{
		pos -= rhs.pos;
		w -= rhs.w;
		tu -= rhs.tu;
		tv -= rhs.tv;
		color -= rhs.color;
		rhw -= rhs.rhw;

		return *this;
	}

	Vertex operator * (float k) const
	{
		Vertex res;

		res.pos = k * pos;
		res.w = k * w;
		res.tu = k * tu;
		res.tv =  k * tv;
		res.color = color * k;
		res.rhw = k * rhw;

		return res;
	}

	Vertex &operator *= (float k)
	{
		pos *= k;
		w *= k;
		tu *= k;
		tv *= k;
		color *= k;
		rhw *= k;

		return *this;
	}

};

inline Vertex vertexInterp(const Vertex &start, const Vertex &end, float t)
{
	Vertex vRet;

	vRet.pos = vectorInterp(start.pos, end.pos, t);
	vRet.w = interpolate(start.w, end.w, t);
	vRet.tu = interpolate(start.tu, end.tu, t);
	vRet.tv = interpolate(start.tv, end.tv, t);
	vRet.color.r = interpolate(start.color.r, end.color.r, t);
	vRet.color.g = interpolate(start.color.g, end.color.g, t);
	vRet.color.b = interpolate(start.color.b, end.color.b, t);
	vRet.rhw = interpolate(start.rhw, end.rhw, t);

	return vRet;
}

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
	int32_t vertexIndex[3];    // 顶点索引
	int32_t nState;          // 多边形状态
	//int32_t color;            // 多边形颜色
};

class Object
{
public:
	bool init(char *fileName, Texture *pTexture);

	void setupWorldPos(const Point3 &pos);

	void setupScale(float fScale);
	void setupScale(const Vector3 &scale);
	void setupRotate(const Vector3 &axis, float theta);

    void resetStates();

	// 计算包围球半径
	void calcMaxRadius();

	// 物体剔除
    void cullObject(const Camera &cam, int nCullFlag);

	// 多边形裁剪
	void clipPolys(const Camera &cam, int32_t nClipFlags);

	// 背面消除
    void removeBackfaces(const Camera &cam);

	// 物体坐标到世界坐标的转换
	void worldTransform();

	// 相机变换
	void cameraTransform(const Camera &cam);

	// 投影变换
	void projectTransform(const Camera &cam);

	// 透视除法
	void  perspectiveDivision();

	// 屏幕变换
	void screenTranform(const Camera &cam);

	// 长方体空间裁剪
	void clipping();

	// 裁剪
	int32_t checkCVV(Vertex *pV);
    
public:
    int32_t nState;						// 物体状态
    
    Point3 posWorld;					// 世界坐标
    Vector3 vecDirection;			// 局部坐标中的朝向
    Vector3 axisX, axisY, axisZ;    // 局部坐标轴
	Vector3 vecScale;
	Vector3 rotAxis;
	float fTheta;
    float fMaxRadius;					// 包围球半径
    
    int32_t nLocalPolyCnt;					// 多边形数
	int32_t nTransPolyCnt;
    Poly polyList[OBJECT_MAX_POLYS];	// 多边形数组
    
    int32_t nLocalVerticesCnt;				// 顶点数
	int32_t nTransVerticesCnt;
	LocalVertex localVertexList[OBJECT_MAX_VERTICES];
    Vertex vertexList[OBJECT_MAX_VERTICES];    // 顶点数组
};


#endif /* Object_hpp */
