//
//  Render.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Render.hpp"
#include "Object.hpp"
#include "Macro.hpp"
#include "MathUtil.hpp"
#include "Texture.hpp"
#include <string.h>

void Render::init(int32_t w, int32_t h, int state, unsigned char *fb)
{
	initRenderState(state);
	initBuffer(w, h,fb);
}

void Render::destroy()
{
	for (int32_t i = 0; i < nWinHeight; ++i)
	{
		delete []pFrameBuffer[i];
	}
	//delete []pFrameBuffer;
	pFrameBuffer = NULL;

	for (int32_t i = 0; i < nWinHeight; ++i)
	{
		delete []pZBuffer[i];
	}
	delete []pZBuffer;
	pZBuffer = NULL;
}

void Render::initRenderState(int32_t state)
{
	nRenderState = state;
}

void Render::initBuffer(int32_t w, int32_t h, void *fb)
{
	nWinWidth = w;
	nWinHeight = h;

	pFrameBuffer = new uint32_t *[nWinHeight];
	//for (int32_t j = 0; j < nWinHeight; ++j)
	//{
	//	pFrameBuffer[j] = new uint32_t [nWinWidth];
	//}

	char *framebuf = (char*)fb;

	for (int32_t j = 0; j < nWinHeight; ++j) 
	{
		pFrameBuffer[j] = (uint32_t*)(framebuf + nWinWidth * 4 * j);
	}


	pZBuffer = new float *[nWinHeight];
	for (int32_t j = 0; j < nWinHeight; ++j)
	{
		pZBuffer[j] = new float [nWinWidth];
		memset(pZBuffer[j], 0, 4*nWinWidth);
	}
}

void Render::resetBuffer()
{
	for (int32_t j = 0; j < nWinHeight; ++j) 
	{
		for (int32_t i = 0; i < nWinWidth; ++i)
		{
			pFrameBuffer[j][i] = 0;
			pZBuffer[j][i] = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Render::initLineByY(Trapezoid *pTrapezoid, Line *pLine, int32_t y)
{
	float s1 = pTrapezoid->eLeft.vEnd.posTrans.y - pTrapezoid->eLeft.vStart.posTrans.y;
	float s2 = pTrapezoid->eRight.vEnd.posTrans.y - pTrapezoid->eRight.vStart.posTrans.y;
	float t1 = (y-pTrapezoid->eLeft.vStart.posTrans.y)/s1;
	float t2 = (y-pTrapezoid->eRight.vStart.posTrans.y)/s2;

	pTrapezoid->eLeft.vInfo = vertexInterp(pTrapezoid->eLeft.vStart, pTrapezoid->eLeft.vEnd, t1);
	pTrapezoid->eRight.vInfo = vertexInterp(pTrapezoid->eRight.vStart, pTrapezoid->eRight.vEnd, t2);
	Vertex &v1 = pTrapezoid->eLeft.vInfo;	
	Vertex &v2 = pTrapezoid->eRight.vInfo;

	pLine->nY = y;
	pLine->v = v1;
	pLine->nXStart = rounding(v1.posTrans.x);
	pLine->nLength = rounding(v2.posTrans.x) - pLine->nXStart;
	float fLength = v2.posTrans.x - v1.posTrans.x;
	if (fLength <= 0.000001f)
	{
		pLine->nLength = 0;
		return;
	}
	
	float f1OverL = 1.0f/fLength;
	pLine->step = (v2 - v1) * f1OverL;
}



void Render::drawTexture(Texture *pTexture)
{
	resetBuffer();
	for (int32_t j = 0; j < pTexture->nHeight; ++j)
	{
		for (int32_t i = 0; i < pTexture->nWidth; ++i)
		{
			pFrameBuffer[j][i] = pTexture->pTexture[j][i];
		}
	}
}

void Render::drawObject(Object *pObject)
{
	resetBuffer();
	for (int32_t i = 0; i < pObject->nPolyCnt; ++i)
	{
		if (pObject->polyList[i].nState & POLY_STATE_ACTIVE)
		{
			drawPoly(&pObject->polyList[i]);
		}
	}
}

void Render::drawPoly(Poly *pPoly)
{
	Vertex *pVList = pPoly->vertexList;
	Vertex *pV0 = &pVList[pPoly->vertexIndex[0]];
	Vertex *pV1 = &pVList[pPoly->vertexIndex[1]];
	Vertex *pV2 = &pVList[pPoly->vertexIndex[2]];
	//Vertex *pV3 = &pVList[pPoly->vertexIndex[3]];

	// 线框模式
	if (nRenderState & RENDER_STATE_WIREFRAME)
	{
		drawTriangle(pV0, pV1, pV2);
		//drawTriangle(pV2, pV3, pV0);
	}

	// 纹理渲染
	if (nRenderState & RENDER_STATE_TEXTURE)
	{
		drawTriangle(pPoly->pTexture, pV0, pV1, pV2);
		//drawTriangle(pPoly->pTexture, pV2, pV3, pV0);
	}
}

void Render::drawTriangle(Vertex *pV0, Vertex *pV1, Vertex *pV2)
{
	// 线框模式
	drawLine(rounding(pV0->posTrans.x), rounding(pV0->posTrans.y), rounding(pV1->posTrans.x), rounding(pV1->posTrans.y), COLOR2);
	drawLine(rounding(pV0->posTrans.x), rounding(pV0->posTrans.y), rounding(pV2->posTrans.x), rounding(pV2->posTrans.y), COLOR2);
	drawLine(rounding(pV1->posTrans.x), rounding(pV1->posTrans.y), rounding(pV2->posTrans.x), rounding(pV2->posTrans.y), COLOR2);
}

void Render::drawTriangle(Texture *pTexture, Vertex *pV0, Vertex *pV1, Vertex *pV2)
{
	// 纹理渲染
	if ((pV0->posTrans.y == pV1->posTrans.y && pV1->posTrans.y == pV2->posTrans.y)
		|| (pV0->posTrans.x == pV1->posTrans.x && pV1->posTrans.x == pV2->posTrans.x) )
	{
		return;
	}

	Trapezoid trap0, trap1;
	if (pV0->posTrans.y > pV1->posTrans.y)
		swap(pV0, pV1);
	if (pV0->posTrans.y > pV2->posTrans.y)
		swap(pV0, pV2);
	if (pV1->posTrans.y > pV2->posTrans.y)
		swap(pV1, pV2);

	if (pV0->posTrans.y == pV1->posTrans.y)
	{
		if (pV0->posTrans.x > pV1->posTrans.x)
			swap(pV0, pV1);
		trap0.fTop = pV0->posTrans.y;
		trap0.fBottom = pV2->posTrans.y;
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV2;
		trap0.eRight.vStart = *pV1;
		trap0.eRight.vEnd = *pV2;

		drawTrapezoid(pTexture,&trap0);
		return;
	}

	if (pV1->posTrans.y == pV2->posTrans.y)
	{
		if (pV1->posTrans.x > pV2->posTrans.x)
			swap(pV1, pV2);

		trap0.fTop = pV0->posTrans.y;
		trap0.fBottom = pV2->posTrans.y;
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV1;
		trap0.eRight.vStart = *pV0;
		trap0.eRight.vEnd = *pV2;

		drawTrapezoid(pTexture,&trap0);
		return;
	}

	trap0.fTop = pV0->posTrans.y;
	trap0.fBottom = pV1->posTrans.y;
	trap1.fTop = pV1->posTrans.y;
	trap1.fBottom = pV2->posTrans.y;

	if (pV0->posTrans.x+(pV1->posTrans.x - pV0->posTrans.x)*(pV2->posTrans.y - pV0->posTrans.y)/(pV1->posTrans.y - pV0->posTrans.y) <= pV2->posTrans.x)
	{
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV1;
		trap0.eRight.vStart = *pV0;
		trap0.eRight.vEnd = *pV2;
		trap1.eLeft.vStart = *pV1;
		trap1.eLeft.vEnd = *pV2;
		trap1.eRight.vStart = *pV0;
		trap1.eRight.vEnd = *pV2;
	}
	else
	{
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV2;
		trap0.eRight.vStart = *pV0;
		trap0.eRight.vEnd = *pV1;
		trap1.eLeft.vStart = *pV0;
		trap1.eLeft.vEnd = *pV2;
		trap1.eRight.vStart = *pV1;
		trap1.eRight.vEnd = *pV2;
	}

	drawTrapezoid(pTexture,&trap0);
	drawTrapezoid(pTexture,&trap1);
}



void Render::drawTrapezoid(Texture *pTexture, Trapezoid *pTrapezoid)
{
	int32_t nTop = (int32_t)(pTrapezoid->fTop + 0.5f);
	int32_t nBottom = (int32_t)(pTrapezoid->fBottom + 0.5f);
	Line line;
	for (int32_t j = nTop; j < nBottom; ++j)
	{
		if ( j >= 0 && j < nWinHeight)
		{
			initLineByY(pTrapezoid, &line, j);
			drawHorizontalLine(pTexture,&line);
		}
		else
		{
			break;
		}
	}
}

void Render::drawHorizontalLine(Texture *pTexture, Line *pLine)
{
	int32_t y = pLine->nY;
	//uint32_t *pFrameBuf = pFrameBuffer[pLine->nY];
	//float *pZBuf = pZBuffer[pLine->nY];

	for (int32_t x = pLine->nXStart, l = pLine->nLength;  l > 0; ++x, --l)
	{
		if (x >= 0 && x < nWinWidth)
		{
			// 判断z缓存
			float rhw = pLine->v.rhw;
			if (rhw >= pZBuffer[y][x])
			{
				float w = 1.0f / rhw;
				pZBuffer[y][x] = rhw;
				float u = pLine->v.tu;
				float v = pLine->v.tv;
				// 颜色渲染
				if (nRenderState & RENDER_STATE_COLOR) 
				{

				}

				// 纹理渲染
				if (nRenderState & RENDER_STATE_TEXTURE) 
				{
					pFrameBuffer[y][x] = pTexture->getColorByUV(u, v);
				}
			}
		}

		pLine->v += pLine->step;

		if (x >= nWinWidth) 
			break;
	}
}

void Render::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
	int32_t x, y, rem;
	// 如果两个点重合
	if (x1 == x2 && y1 == y2)
	{
		drawPixel(x1, y1, color);
		return;
	}

	// 垂直线
	if (x1 == x2)
	{
		int32_t inc = (y1 < y2) ? 1 : -1;
		for ( y = y1; y != y2; y+= inc)
		{
			drawPixel(x1, y, color);
		}
		return;
	}

	// 水平线
	if (y1 == y2)
	{
		int inc = (x1 < x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc)
		{
			drawPixel(x, y1, color);
		}
		return;
	}

	rem = 0;
	int dx = (x1 < x2)? x2 - x1 : x1 - x2;
	int dy = (y1 < y2)? y2 - y1 : y1 - y2;
	if (dx >= dy) 
	{
		if (x2 < x1)
		{
			int32_t x, y;
			x = x1; y = y1;
			x1 = x2; y1 = y2;
			x2 = x; y2 = y;
		}

		for (x = x1, y = y1; x <= x2; ++x) 
		{
			drawPixel(x, y, color);
			rem += dy;
			if (rem >= dx) 
			{
				rem -= dx;
				y += (y2 >= y1) ? 1 : -1;
				drawPixel(x, y, color);
			}
		}
		drawPixel(x2, y2, color);
	}
	else
	{
		if (y2 < y1)
		{
			int32_t x, y;
			x = x1; y = y1;
			x1 = x2; y1 = y2;
			x2 = x; y2 = y;
		}

		for (x = x1, y = y1; y <= y2; ++y) 
		{
			drawPixel(x, y, color);
			rem += dx;
			if (rem >= dy) 
			{
				rem -= dy;
				x += (x2 >= x1) ? 1 : -1;
				drawPixel(x, y, color);
			}
		}
		drawPixel(x2, y2, color);
	}
}

void Render::drawPixel(int32_t x, int32_t y, uint32_t color)
{
	if (x < nWinWidth && y < nWinHeight)
	{
		pFrameBuffer[y][x] = color;
	}
}
