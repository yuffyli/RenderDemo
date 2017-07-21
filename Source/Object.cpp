//
//  Object.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//
// #include "StdAfx.h"

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
	sscanf(token_string, "%d %d ", &nLocalVerticesCnt, &nLocalPolyCnt);
	nTransVerticesCnt = nLocalVerticesCnt;
	nTransPolyCnt = nLocalPolyCnt;

	// 读取顶点信息
	for (int32_t i = 0; i < nLocalVerticesCnt; ++i)
	{
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			return false;
		}

		//sscanf(token_string, "%f %f %f %f %f", 
		//	&vertexList[i].posLocal.x,
		//	&vertexList[i].posLocal.y,
		//	&vertexList[i].posLocal.z,
		//	&vertexList[i].tu,
		//	&vertexList[i].tv
		//	);
		sscanf(token_string, "%f %f %f %f %f %f %f %f", 
			&localVertexList[i].pos.x,
			&localVertexList[i].pos.y,
			&localVertexList[i].pos.z,
			&localVertexList[i].tu,
			&localVertexList[i].tv,
			&localVertexList[i].color.r,
			&localVertexList[i].color.g,
			&localVertexList[i].color.b
			);
		//vertexList[i].w = 1.0f;
		//vertexList[i].rhw = 1.0f;
	}

	// 读取三角形信息
	for (int32_t i = 0; i < nLocalPolyCnt; ++i)
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

	calcMaxRadius();

	return true;
}

void Object::setupWorldPos(const Point3 &pos)
{
	posWorld = pos;
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

void Object::resetStates()
{
    // 重置物体被剔除的标记
    RESET_BIT(nState, OBJECT_STATE_CULLED);
    
    // 重置多边形的被裁剪标记和背面标记
    for (int i = 0; i < nLocalPolyCnt; ++i)
    {
        Poly &poly = polyList[i];
        
		poly.nState = POLY_STATE_ACTIVE;
        // 如果多边形不可见，跳过
        //if (! (poly.nState & POLY_STATE_ACTIVE))
        //{
        //    continue;
        //}
        
        //RESET_BIT(poly.nState, POLY_STATE_CLIPPED);
        //RESET_BIT(poly.nState, POLY_STATE_BACKFACE);
    }
}

void Object::calcMaxRadius()
{
	fMaxRadius = .0f;
	float fCurrRadius;
	
	for (int32_t i = 0; i < nLocalVerticesCnt; ++i)
	{
		Point3 &pos = localVertexList[i].pos;
		if (fCurrRadius = (pos.x*pos.x + pos.y*pos.y + pos.z*pos.z) > fMaxRadius )
		{
			fMaxRadius = fCurrRadius;
		}
	}

	fMaxRadius = sqrt(fMaxRadius);
}

void Object::cullObject(const Camera &cam, int nCullFlag)
{
    // 包围球中心坐标
    Point3 posSphere;
    posSphere = posWorld * cam.m_matrixCamera;

	float fMaxScale = vecScale.x;
	if (vecScale.y > fMaxScale)
	{
		fMaxScale = vecScale.y;
	}
	if (vecScale.z > fMaxScale)
	{
		fMaxScale = vecScale.z;
	}
	float fRadius = fMaxRadius * fMaxScale;
    
    // 远近裁剪面剔除
    if (nCullFlag & CULL_OBJECT_Z_PLANE)
    {
        if (((posSphere.z - fRadius) > cam.m_fFarZ) || ((posSphere.z + fRadius) < cam.m_fNearZ))
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
    
    // 左右裁剪面剔除
    if (nCullFlag & CULL_OBJECT_X_PLANE)
    {
        float fTest = 0.5f * cam.m_fViewPlaneWidth * posSphere.z / cam.m_fViewDistance;
        if (((posSphere.x - fRadius) > fTest) || (posSphere.x + fRadius) < -fTest)
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
    
    // 上下裁剪面剔除
    if (nCullFlag & CULL_OBJECT_Y_PLANE)
    {
        float fTest = 0.5f * cam.m_fViewPlaneHeight * posSphere.z / cam.m_fViewDistance;
        if (((posSphere.y - fRadius) > fTest) || (posSphere.y + fRadius) < -fTest)
        {
            SET_BIT(nState, OBJECT_STATE_CULLED);
            return ;
        }
    }
}

void Object::clipPolys(const Camera &cam, int32_t nClipFlags)
{
	nTransVerticesCnt = nLocalVerticesCnt;
	nTransPolyCnt = nLocalPolyCnt;
	for (int32_t j = 0; j < nLocalPolyCnt; ++j)
	{
		Poly *pPoly = &polyList[j];
		// 如果多边形不是active的，直接跳过
		if (!(pPoly->nState & POLY_STATE_ACTIVE))
			continue;

		Vertex *pVList = pPoly->vertexList;
		Vertex *pV0 = &pVList[pPoly->vertexIndex[0]];
		Vertex *pV1 = &pVList[pPoly->vertexIndex[1]];
		Vertex *pV2 = &pVList[pPoly->vertexIndex[2]];

		int32_t codes[POLY_VERTICES_NUM];
		// 左右裁剪面裁剪
		if (nClipFlags & CLIP_POLY_X_PLANE)
		{
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
			{
				Vertex *pV = &pVList[pPoly->vertexIndex[i]];
				if (pV->pos.x > pV->w)
					codes[i] = CLIP_CODE_GX;
				else if (pV->pos.x < -pV->w)
					codes[i] = CLIP_CODE_LX;
				else
					codes[i] = CLIP_CODE_IX;
			}

			bool bGX = true, bLX = true;
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bGX = bGX&& (codes[i] == CLIP_CODE_GX);
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bLX = bLX&& (codes[i] == CLIP_CODE_LX);
			if (bGX || bLX)
			{
				SET_BIT(pPoly->nState, POLY_STATE_CLIPPED);
				continue;
			}
		}

		// 上下裁剪面裁剪
		if (nClipFlags & CLIP_POLY_Y_PLANE)
		{
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
			{
				Vertex *pV = &pVList[pPoly->vertexIndex[i]];
				if (pV->pos.y > pV->w)
					codes[i] = CLIP_CODE_GY;
				else if (pV->pos.y < -pV->w)
					codes[i] = CLIP_CODE_LY;
				else
					codes[i] = CLIP_CODE_IY;
			}

			bool bGY = true, bLY = true;
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bGY = bGY&& (codes[i] == CLIP_CODE_GY);
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bLY = bLY&& (codes[i] == CLIP_CODE_LY);
			if (bGY || bLY)
			{
				SET_BIT(pPoly->nState, POLY_STATE_CLIPPED);
				continue;
			}
		}

		// 远近裁剪面裁剪Y
		if (nClipFlags & CLIP_POLY_Z_PLANE)
		{
			int32_t nVertsIn = 0;
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
			{
				Vertex *pV = &pVList[pPoly->vertexIndex[i]];
				if (pV->pos.z > 1)
					codes[i] = CLIP_CODE_GZ;
				else if (pV->pos.z < 0)
				{
					codes[i] = CLIP_CODE_LZ;
					nVertsIn++;
				}
				else
				{
					codes[i] = CLIP_CODE_IZ;
					//nVertsIn++;
				}
			}

			bool bGZ = true, bLZ = true;
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bGZ = bGZ && (codes[i] == CLIP_CODE_GZ);
			for (int32_t i = 0; i < POLY_VERTICES_NUM; ++i )
				bLZ = bLZ && (codes[i] == CLIP_CODE_LZ);
			if (bGZ || bLZ)
			{
				SET_BIT(pPoly->nState, POLY_STATE_CLIPPED);
				continue;
			}

			// 如果有跨过近裁剪面的，裁剪
			if ( ((codes[0] | codes[1] | codes[2]) & CLIP_CODE_LZ) )
			{
				if (nVertsIn == 2)
				{
					// 有2个点在远近裁剪面之间，不需要新增三角形
					Vertex *pV0, *pV1, *pV2;
					if (codes[0] != CLIP_CODE_LZ)
					{
						pV0 = &pVList[pPoly->vertexIndex[0]];
						pV1 = &pVList[pPoly->vertexIndex[1]];
						pV2 = &pVList[pPoly->vertexIndex[2]];
					}
					else if (codes[1] != CLIP_CODE_LZ)
					{
						pV0 = &pVList[pPoly->vertexIndex[1]];
						pV1 = &pVList[pPoly->vertexIndex[2]];
						pV2 = &pVList[pPoly->vertexIndex[0]];
					}
					else
					{
						pV0 = &pVList[pPoly->vertexIndex[2]];
						pV1 = &pVList[pPoly->vertexIndex[0]];
						pV2 = &pVList[pPoly->vertexIndex[1]];
					}

					if (pV1->pos.z < 0)
					{
						float t1 = (.0f - pV0->pos.z)/(pV1->pos.z - pV0->pos.z);
						(*pV1) = (*pV0) + (((*pV1)-(*pV0)) * t1);
						pV1->pos.z = .0f;
						pV1->w = cam.m_fNearZ;
					}
					
					
					if (pV2->pos.z < 0)
					{
						float t2 = (.0f - pV0->pos.z)/(pV2->pos.z - pV0->pos.z);
						(*pV2) = (*pV0) + (((*pV2)-(*pV0)) * t2);
						pV2->pos.z = .0f;
						pV2->w = cam.m_fNearZ;
					}
					
				}

				if (nVertsIn == 1)
				{
					// 有1个点在远近裁剪面之间，需要重新分割三角形
					if (codes[0] == CLIP_CODE_LZ)
					{
						pV0 = &pVList[pPoly->vertexIndex[0]];
						pV1 = &pVList[pPoly->vertexIndex[1]];
						pV2 = &pVList[pPoly->vertexIndex[2]];
					}
					else if (codes[1] == CLIP_CODE_LZ)
					{
						pV0 = &pVList[pPoly->vertexIndex[1]];
						pV1 = &pVList[pPoly->vertexIndex[2]];
						pV2 = &pVList[pPoly->vertexIndex[0]];
					}
					else if (codes[2] == CLIP_CODE_LZ)
					{
						pV0 = &pVList[pPoly->vertexIndex[2]];
						pV1 = &pVList[pPoly->vertexIndex[0]];
						pV2 = &pVList[pPoly->vertexIndex[1]];
					}
					else
					{
						continue;
					}

					float t1 = (.0f - pV0->pos.z)/(pV1->pos.z - pV0->pos.z);
					Vertex vT1 = (*pV0) + (((*pV1)-(*pV0))  * t1);
					vT1.pos.z = .0f;
					vT1.w = cam.m_fNearZ;

					float t2 = (.0f - pV0->pos.z)/(pV2->pos.z - pV0->pos.z);
					Vertex vT2 = (*pV0) + (((*pV2)-(*pV0)) * t2);
					vT2.pos.z = .0f;
					vT2.w = cam.m_fNearZ;

					(*pV0) = vT1;

					vertexList[nTransVerticesCnt] = vT1;
					vertexList[nTransVerticesCnt+1] = vT2;
					vertexList[nTransVerticesCnt+2] = *pV2;
					
					polyList[nTransPolyCnt] = *pPoly;
					polyList[nTransPolyCnt].vertexIndex[0] = nTransVerticesCnt;
					polyList[nTransPolyCnt].vertexIndex[1] = nTransVerticesCnt+1;
					polyList[nTransPolyCnt].vertexIndex[2] = nTransVerticesCnt+2;
					nTransVerticesCnt += 3;
					nTransPolyCnt++;

				}
			}
		}
	}
}

void Object::removeBackfaces(const Camera &cam)
{
    // 在执行世界坐标到相机坐标变换之前执行背面消除
    // 把物体背面的多边形标记为POLY_STATE_BACKFACE
    for (int i = 0; i < nLocalPolyCnt; ++i)
    {
		Poly &poly = polyList[i];

		if (CULLING_STATE & CULLING_NONE)
		{
			poly.nState = POLY_STATE_ACTIVE;
			continue;
		}

		int idx0 = poly.vertexIndex[0];
		int idx1 = poly.vertexIndex[1];
		int idx2 = poly.vertexIndex[2];

		Vector3 u = vertexList[idx1].pos - vertexList[idx0].pos;
		Vector3 v = vertexList[idx2].pos - vertexList[idx0].pos;

		Vector3 n;
		if (CULLING_STATE & CULLING_BACK_FACE)
		{
			n = crossProduct(u, v);
		}
		
		if (CULLING_STATE & CULLING_FRONT_FACE)
		{
			n = crossProduct(v, u);
		}

		Vector3 view = cam.m_posCamera - vertexList[idx0].pos;

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

	for (int32_t i = 0; i < nLocalVerticesCnt; ++i)
	{
		//vertexList[i].pos = vertexList[i].posLocal * mWorld;
		vertexList[i].pos = localVertexList[i].pos * mWorld;
		vertexList[i].tu = localVertexList[i].tu;
		vertexList[i].tv = localVertexList[i].tv;
		vertexList[i].color = localVertexList[i].color;
		vertexList[i].w = 1.0f;
		vertexList[i].rhw = 1.0f;
	}
}

void Object::cameraTransform(const Camera &cam)
{
	// 相机变换
	for (int32_t i = 0; i < nLocalVerticesCnt; ++i)
	{
		vertexList[i].pos *= cam.m_matrixCamera;
	}
}

void Object::projectTransform(const Camera &cam)
{
	// 透视投影，变换到裁剪空间
	for (int32_t i = 0; i < nLocalVerticesCnt; ++i)
	{
		vertexList[i].w = vertexList[i].pos.z;
		vertexList[i].pos *= cam.m_matrixProjection;
		//float rhw = 1.0f/vertexList[i].w;
		//vertexList[i].posTrans *= rhw;
		//vertexList[i].rhw = rhw;
	}
}

void Object::perspectiveDivision()
{
	//透视除法，变换到标准化设备坐标 NDC（Normalized Device Coordinates）
	for (int32_t i = 0; i< nTransPolyCnt; ++i)
	{
		Poly &poly = polyList[i];
		if (poly.nState & POLY_STATE_ACTIVE)
		{
			Vertex *vertexList = poly.vertexList;
			for (int32_t j = 0; j < POLY_VERTICES_NUM; ++j)
			{
				int32_t k = poly.vertexIndex[j];
				float rhw = 1.0f/vertexList[k].w;
				vertexList[k].pos *= rhw;
				vertexList[k].rhw = rhw;
			}
		}
	}
	//for (int32_t i = 0; i < nTransVerticesCnt; ++i)
	//{
	//	float rhw = 1.0f/vertexList[i].w;
	//	vertexList[i].pos *= rhw;
	//	vertexList[i].rhw = rhw;
	//}
}

void Object::screenTranform(const Camera &cam)
{
	// 屏幕变换
	for (int32_t i = 0; i< nTransPolyCnt; ++i)
	{
		Poly &poly = polyList[i];
		if (poly.nState & POLY_STATE_ACTIVE)
		{
			Vertex *vertexList = poly.vertexList;
			for (int32_t j = 0; j < POLY_VERTICES_NUM; ++j)
			{
				int32_t k = poly.vertexIndex[j];
				vertexList[k].pos *= cam.m_matrixScreen;
			}
		}
	}

	//for (int32_t i = 0; i < nTransVerticesCnt; ++i)
	//{
	//	vertexList[i].pos *= cam.m_matrixScreen;
	//}
}


void Object::clipping()
{
	for (int i = 0; i < nLocalPolyCnt; ++i)
	{
		Poly &poly = polyList[i];
		int idx0 = poly.vertexIndex[0];
		int idx1 = poly.vertexIndex[1];
		int idx2 = poly.vertexIndex[2];

		if (checkCVV(&vertexList[idx0])!=0 || checkCVV(&vertexList[idx1])!=0 || checkCVV(&vertexList[idx2])!=0 )
		{
			poly.nState = POLY_STATE_BACKFACE;
		}
	}
}

int32_t Object::checkCVV(Vertex *pV)
{
	int32_t nCheck = 0;
	float w = pV->w;
	if (pV->pos.z < 0.0f) 
		nCheck |= 1;
	if (pV->pos.z >  w) 
		nCheck |= 2;
	if (pV->pos.x < -w) 
		nCheck |= 4;
	if (pV->pos.x >  w) 
		nCheck |= 8;
	if (pV->pos.y < -w) 
		nCheck |= 16;
	if (pV->pos.y >  w) 
		nCheck |= 32;

	return nCheck;
}