//
//  Object.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Object.hpp"

void Object::init()
{
	static Vertex vertexList[8] = {
		{ Point3(1.0f, -1.0f,  1.0f), 1, 0, 0, 1 },
		{ Point3( -1.0f, -1.0f,  1.0f), 1, 0, 1, 1 },
		{ Point3(-1.0f,  1.0f,  1.0f), 1, 1, 1, 1 },
		{ Point3( 1.0f,  1.0f,  1.0f), 1, 1, 0 , 1 },
		{ Point3(1.0f, -1.0f, -1.0f), 1, 0, 0 ,1 },
		{ Point3(-1.0f, -1.0f, -1.0f), 1, 0, 1 , 1 },
		{ Point3( -1.0f,  1.0f, -1.0f), 1,  1, 1 , 1 },
		{ Point3(1.0f,  1.0f, -1.0f), 1,  1, 0 , 1 },
	};

	nPolyCnt = 6;
	polyList[0].vertexList = vertexList;
	polyList[0].vertexIndex[0] = 0;
	polyList[0].vertexIndex[1] = 1;
	polyList[0].vertexIndex[2] = 2;
	polyList[0].vertexIndex[3] = 3;
	polyList[0].nState = POLY_STATE_ACTIVE;

	polyList[1].vertexList = vertexList;
	polyList[1].vertexIndex[0] = 4;
	polyList[1].vertexIndex[1] = 6;
	polyList[1].vertexIndex[2] = 6;
	polyList[1].vertexIndex[3] = 7;
	polyList[1].nState = POLY_STATE_ACTIVE;

	polyList[2].vertexList = vertexList;
	polyList[2].vertexIndex[0] = 0;
	polyList[2].vertexIndex[1] = 4;
	polyList[2].vertexIndex[2] = 5;
	polyList[2].vertexIndex[3] = 1;
	polyList[2].nState = POLY_STATE_ACTIVE;

	polyList[3].vertexList = vertexList;
	polyList[3].vertexIndex[0] = 1;
	polyList[3].vertexIndex[1] = 5;
	polyList[3].vertexIndex[2] = 6;
	polyList[3].vertexIndex[3] = 2;
	polyList[3].nState = POLY_STATE_ACTIVE;

	polyList[4].vertexList = vertexList;
	polyList[4].vertexIndex[0] = 2;
	polyList[4].vertexIndex[1] = 6;
	polyList[4].vertexIndex[2] = 7;
	polyList[4].vertexIndex[3] = 3;
	polyList[4].nState = POLY_STATE_ACTIVE;

	polyList[5].vertexList = vertexList;
	polyList[5].vertexIndex[0] = 3;
	polyList[5].vertexIndex[1] = 7;
	polyList[5].vertexIndex[2] = 4;
	polyList[5].vertexIndex[3] = 0;
	polyList[5].nState = POLY_STATE_ACTIVE;

	nVerticesCnt = 8;
}

void Object::reset()
{
    // 重置物体被剔除的标记
    RESET_BIT(nState, OBJECT_STATE_CULLED);
    
    // 重置多边形的被裁剪标记和背面标记
    for (int i = 0; i < nPolyCnt; ++i)
    {
        Poly &poly = polyList[i];
        
        // 如果多边形不可见，跳过
        if (! (poly.nState & POLY_STATE_ACTIVE))
        {
            continue;
        }
        
        RESET_BIT(poly.nState, POLY_STATE_CLIPPED);
        RESET_BIT(poly.nState, POLY_STATE_BACKFACE);
    }
}

void Object::cullObject(const Camera &cam, int nCullFlag)
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

void Object::removeBackfaces(const Camera &cam)
{
    // 在执行世界坐标到相机坐标变换之前执行背面消除
    
    // 把物体背面的多边形标记为POLY_STATE_BACKFACE
    for (int i = 0; i < nPolyCnt; ++i)
    {
		Poly &poly = polyList[i];
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

void Object::worldTransform()
{

}

void Object::cameraTransform(const Camera &cam)
{

}

void Object::projectTransform(const Camera &cam)
{

}

void Object::screenTranform(const Camera &cam)
{

}
