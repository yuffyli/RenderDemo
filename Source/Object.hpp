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

// 多边形
struct Polygon
{
    Vector3 *vertexList;    // 顶点列表
    int vertexIndex[3];    // 顶点索引
    int nState;                // 多边形状态
    int color;            // 多边形颜色
};

struct Object
{
public:
    void reset();

    void cull(const Camera &cam, int nCullFlag);
    void deleteBackface(const Camera &cam);
    
public:
    int nState;     // 物体状态
    
    Point3 posWorld;   // 世界坐标
    Vector3 vecDirection;   // 局部坐标中的朝向
    Vector3 axisX, axisY, axisZ;    // 局部坐标轴
    float fMaxRadius;   // 包围球半径
    
    int nPolyCnt;   // 多边形数
    Polygon polyList[OBJECT_MAX_POLYS];
    
    int nVerticesCnt;   // 顶点数
    Point3 vertexListLocal[OBJECT_MAX_VERTICES];    // 顶点的局部坐标数组
    Point3 vertexListTrans[OBJECT_MAX_VERTICES];    // 顶点变换后的坐标数组
};


struct RenderList
{
    int nPolyCnt;   // 多边形数
    
    Polygon *polyHanleList[RENDERLIST_MAX_POLYS];
    Polygon polyDataList[RENDERLIST_MAX_POLYS];
};






#endif /* Object_hpp */
