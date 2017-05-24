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

void Render::init(int32_t w, int32_t h, unsigned char *fb)
{
	initRenderState();
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

void Render::initRenderState()
{
	nRenderState = RENDER_STATE_TEXTURE;
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
	}

	for (int32_t j = 0; j < nWinHeight; ++j)
	{
		for (int32_t i = 0; i < nWinWidth; ++i)
		{
			pFrameBuffer[j][i] = 0x99ccffff;
		}
	}
}



//void Device::initTexture()
//{
//	// 将纹理分成32x32的小块，并为每个小块填充同一种颜色
//	static uint32_t texture[TEXTURE_HEIGHT][TEXTURE_WIDTH];
//	uint32_t color[4] = {COLOR0, COLOR1, COLOR2, COLOR3};
//	for (int32_t j = 0; j < TEXTURE_HEIGHT; ++j)
//	{
//		for (int32_t i = 0; i < TEXTURE_WIDTH; ++i)
//		{
//			int32_t x = i/32;
//			int32_t y = j/32;
//			texture[j][i] = color[(x+y)%4];
//		}
//	}
//
//	setupTexture(texture, TEXTURE_WIDTH, TEXTURE_HEIGHT);
//}
//
//void Device::setupTexture(void *pBits, int32_t tW, int32_t tH)
//{
//	char *p = (char *)pBits;
//	pTexture = new uint32_t *[tH];
//	for (int32_t j = 0; j < tH; ++j)
//	{
//		pTexture[j] = new uint32_t[tW];
//	}
//
//	for(int32_t j = 0; j < tH; ++j)
//	{
//		pTexture[j] = (uint32_t*)p;
//		p += 4*tW;
//	}
//
//	nTexWidth = tW;
//	nTexHeight = tH;
//	fMaxU = (float)(tW-1);
//	fMaxV = (float)(tH-1);
//}

//////////////////////////////////////////////////////////////////////////
void Render::initLineByY(Trapezoid *pTrapezoid, Line *pLine, int32_t y)
{
	Vertex &v1 = pTrapezoid->eLeft.vInfo;
	Vertex &v2 = pTrapezoid->eRight.vInfo;

	pLine->nY = y;
	pLine->v = v1;
	pLine->nXStart = rounding(v1.pos.x);
	pLine->nLength = rounding(v2.pos.x) - pLine->nXStart;
	float fLength = v2.pos.x - v1.pos.x;
	if (fLength <= 0.000001f)
	{
		pLine->nLength = 0;
		return;
	}
	
	float f1OverL = 1.0f/fLength;
	pLine->step = (v2 - v1) * f1OverL;
}



void Render::drawObject(Object *pObject)
{
	for (int32_t i = 0; i < pObject->nPolyCnt; ++i)
	{
		drawPoly(&pObject->polyList[i]);
	}
}

void Render::drawPoly(Poly *pPoly)
{
	Vertex *pVList = pPoly->vertexList;
	Vertex *pV0 = &pVList[pPoly->vertexIndex[0]];
	Vertex *pV1 = &pVList[pPoly->vertexIndex[1]];
	Vertex *pV2 = &pVList[pPoly->vertexIndex[2]];
	Vertex *pV3 = &pVList[pPoly->vertexIndex[3]];

	//// 线框模式
	//if (nRenderState & RENDER_STATE_WIREFRAME)
	//{
	//	drawLine(rounding(pV0->pos.x), rounding(pV0->pos.y), rounding(pV1->pos.x), rounding(pV1->pos.y), 8);
	//	drawLine(rounding(pV0->pos.x), rounding(pV0->pos.y), rounding(pV2->pos.x), rounding(pV2->pos.y), 8);
	//	drawLine(rounding(pV1->pos.x), rounding(pV1->pos.y), rounding(pV2->pos.x), rounding(pV2->pos.y), 8);
	//}

	// 纹理渲染
	if (nRenderState & RENDER_STATE_TEXTURE)
	{
		drawTriangle(pPoly->pTexture, pV0, pV1, pV2);
		drawTriangle(pPoly->pTexture, pV2, pV3, pV0);
	}
}

void Render::drawTriangle(Texture *pTexture, Vertex *pV0, Vertex *pV1, Vertex *pV2)
{
	if ((pV0->pos.y == pV1->pos.y && pV1->pos.y == pV2->pos.y)
		|| (pV0->pos.x == pV1->pos.x && pV1->pos.x == pV2->pos.x) )
	{
		return;
	}
	
	Trapezoid trap0, trap1;
	if (pV0->pos.y > pV1->pos.y)
		swap(pV0, pV1);
	if (pV0->pos.y > pV2->pos.y)
		swap(pV0, pV2);
	if (pV1->pos.y > pV2->pos.y)
		swap(pV1, pV2);

	if (pV0->pos.y == pV1->pos.y)
	{
		if (pV0->pos.x > pV1->pos.x)
			swap(pV0, pV1);
		trap0.fTop = pV0->pos.y;
		trap0.fBottom = pV2->pos.y;
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV2;
		trap0.eRight.vStart = *pV1;
		trap0.eRight.vEnd = *pV2;

		drawTrapezoid(pTexture,&trap0);
		return;
	}

	if (pV1->pos.y == pV2->pos.y)
	{
		if (pV1->pos.x > pV2->pos.x)
			swap(pV1, pV2);

		trap0.fTop = pV0->pos.y;
		trap0.fBottom = pV2->pos.y;
		trap0.eLeft.vStart = *pV0;
		trap0.eLeft.vEnd = *pV1;
		trap0.eRight.vStart = *pV0;
		trap0.eRight.vEnd = *pV2;

		drawTrapezoid(pTexture,&trap0);
		return;
	}

	trap0.fTop = pV0->pos.y;
	trap0.fBottom = pV1->pos.y;
	trap1.fTop = pV1->pos.y;
	trap1.fBottom = pV2->pos.y;

	if (pV0->pos.x+(pV1->pos.x - pV0->pos.x)*(pV2->pos.y - pV0->pos.y)/(pV1->pos.y - pV0->pos.y) <= pV2->pos.x)
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
				float u = pLine->v.tu*w;
				float v = pLine->v.tv*w;

				// 颜色渲染
				if (nRenderState & RENDER_STATE_COLOR) 
				{

				}

				// 纹理渲染
				if (nRenderState & RENDER_STATE_TEXTURE) 
				{
					pFrameBuffer[y][x] = pTexture->getColorByUV(u, v);
					//int32_t nTx = middle(0, nTexWidth, rounding(pLine->v.tu*w*fMaxU));
					//int32_t nTy = middle(0, nTexHeight, rounding(pLine->v.tv*w*fMaxV));
					//pFrameBuf[x] = pTexture[nTy][nTx];
				}
			}
		}
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
