//
//  Object.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Object.hpp"

void Object::reset()
{
    // 重置物体被剔除的标记
    RESET_BIT(nState, OBJECT_STATE_CULLED);
    
    // 重置多边形的被裁剪标记和背面标记
    for (int i = 0; i < nPolyCnt; ++i)
    {
        Polygon &poly = polyList[i];
        
        // 如果多边形不可见，跳过
        if (! (poly.nState & POLY_STATE_ACTIVE))
        {
            continue;
        }
        
        RESET_BIT(poly.nState, POLY_STATE_CLIPPED);
        RESET_BIT(poly.nState, POLY_STATE_BACKFACE);
    }
}

void Object::cull(const Camera &cam, int nCullFlag)
{
    // 包围球中心坐标
    Point3 posSphere;
    posSphere = posWorld * cam.m_matrixCamera;
    
    // 远近裁剪面剔除
    if (nCullFlag & CULL_OBJECT_Z_PLANE)
    {
        if (((posSphere.z - fMaxRadius) > cam.m_fFarZ) || ((posSphere.z + fMaxRadius) < cam.m_fNearZ))
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
    
    // 左右裁剪面剔除
    if (nCullFlag & CULL_OBJECT_X_PLANE)
    {
        float fTest = 0.5f * cam.m_fViewPlaneWidth * posSphere.z / cam.m_fViewDistance;
        if (((posSphere.x - fMaxRadius) > fTest) || (posSphere.x + fMaxRadius) < -fTest)
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
    
    // 上下裁剪面剔除
    if (nCullFlag & CULL_OBJECT_Y_PLANE)
    {
        float fTest = 0.5f * cam.m_fViewPlaneHeight * posSphere.z / cam.m_fViewDistance;
        if (((posSphere.y - fMaxRadius) > fTest) || (posSphere.y + fMaxRadius) < -fTest)
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
}

void Object::deleteBackface(const Camera &cam)
{
    // 在执行世界坐标到相机坐标变换之前执行背面消除
    
    // 把物体背面的多边形标记为POLY_STATE_BACKFACE
    for (int i = 0; i < nPolyCnt; ++i)
    {
        
        Polygon &poly = polyList[i];
        int idx0 = poly.vertexIndex[0];
        int idx1 = poly.vertexIndex[1];
        int idx2 = poly.vertexIndex[2];
        
        Vector3 u = vertexListTrans[idx1] - vertexListTrans[idx0];
        Vector3 v = vertexListTrans[idx2] - vertexListTrans[idx0];
        Vector3 n = crossProduct(u, v);
        
        Vector3 view = cam.m_posCamera - vertexListTrans[idx0];
        
        float fDotResult = n*view;
        poly.nState = (fDotResult <= .0f) ? POLY_STATE_BACKFACE : POLY_STATE_ACTIVE;
    }
}
