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

// 顶点
struct Vertex
{
	Point3 pos;
	float w;
	float u, v;
	float r, g, b, a;
	float rhw;
};

// 多边形
struct Poly
{
	Vertex *vertexList;    // 顶点列表
	int vertexIndex[3];    // 顶点索引
	int nState;                // 多边形状态
	int color;            // 多边形颜色
};

class Object
{
public:
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
    Point3 vertexListLocal[OBJECT_MAX_VERTICES];    // 顶点的局部坐标数组
    Point3 vertexListTrans[OBJECT_MAX_VERTICES];    // 顶点变换后的坐标数组
};


struct RenderList
{
    int nPolyCnt;   // 多边形数
    
    Poly *polyHanleList[RENDERLIST_MAX_POLYS];
    Poly polyDataList[RENDERLIST_MAX_POLYS];

};



#endif /* Object_hpp */
