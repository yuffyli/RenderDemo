//
//  Object.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Object.hpp"
#include <stdio.h>

bool Object::init(char *fileName, Texture *pTexture)
{

	char buffer[256];
	char *token_string;

	FILE *fp = fopen(fileName, "r");
	if (NULL == fp)
	{
		return false;
	}

	if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
	{
		return false;
	} 

	// 读取顶点和三角形数目
	sscanf(token_string, "%d %d ", &nVerticesCnt, &nPolyCnt);

	// 读取顶点信息
	for (int32_t i = 0; i < nVerticesCnt; ++i)
	{
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			return false;
		}

		sscanf(token_string, "%f %f %f %f %f", 
			&vertexList[i].posLocal.x,
			&vertexList[i].posLocal.y,
			&vertexList[i].posLocal.z,
			&vertexList[i].tu,
			&vertexList[i].tv
			);
		vertexList[i].w = 1.0f;
		vertexList[i].rhw = 1.0f;
	}

	// 读取三角形信息
	for (int32_t i = 0; i < nPolyCnt; ++i)
	{
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			return false;
		}

		sscanf(token_string, "%d %d %d", 
			&polyList[i].vertexIndex[0],
			&polyList[i].vertexIndex[1],
			&polyList[i].vertexIndex[2]);

		polyList[i].vertexList = vertexList;
		polyList[i].pTexture = pTexture;
		polyList[i].nState = POLY_STATE_ACTIVE;
	}

	fclose(fp);

	return true;
}

void Object::setupScale(const Vector3 &scale)
{
	if (scale.x < .0f || scale.y < .0f || scale.z < .0f)
	{
		return;
	}

	vecScale = scale;
}

void Object::setupScale(float fScale)
{
	if (fScale > .0f)
	{
		vecScale = fScale * Vector3(1.0f, 1.0f, 1.0f);
	}

}

void Object::setupRotate(const Vector3 &axis, float theta)
{
	rotAxis = axis;
	fTheta = theta;
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

		Vector3 u = vertexList[idx1].posTrans - vertexList[idx0].posTrans;
		Vector3 v = vertexList[idx2].posTrans - vertexList[idx0].posTrans;
		Vector3 n = crossProduct(u, v);

		Vector3 view = cam.m_posCamera - vertexList[idx0].posTrans;

		float fDotResult = n*view;
		poly.nState = (fDotResult <= .0f) ? POLY_STATE_BACKFACE : POLY_STATE_ACTIVE;
    }
}

void Object::worldTransform()
{
	// 世界变换
	Matrix4x3 mScale, mRotate, mTrans, mWorld;;

	// 先做缩放
	mScale.setupScale(vecScale);
	// 再做旋转
	mRotate.setupRotate(rotAxis, fTheta);
	// 最后做平移
	mTrans.setupTranslation(posWorld);

	mWorld = mScale*mRotate*mTrans;

	for (int32_t i = 0; i < nVerticesCnt; ++i)
	{
		vertexList[i].posTrans = vertexList[i].posLocal * mWorld;;
	}
}

void Object::cameraTransform(const Camera &cam)
{
	// 相机变换
	for (int32_t i = 0; i < nVerticesCnt; ++i)
	{
		vertexList[i].posTrans *= cam.m_matrixCamera;
	}
}

void Object::projectTransform(const Camera &cam)
{
	// 透视变换，修改位置，w和rhw
	for (int32_t i = 0; i < nVerticesCnt; ++i)
	{
		vertexList[i].w = vertexList[i].posTrans.z;
		vertexList[i].posTrans *= cam.m_matrixProjection;
		float rhw = 1.0f/vertexList[i].w;
		vertexList[i].posTrans *= rhw;
		vertexList[i].rhw = rhw;
	}
}

void Object::screenTranform(const Camera &cam)
{
	// 屏幕变换
	for (int32_t i = 0; i < nVerticesCnt; ++i)
	{
		vertexList[i].posTrans *= cam.m_matrixScreen;
	}
}


