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

void Device::init(int32_t w, int32_t h, int32_t s)
{
	nWinWidth = w;
	nWinHeight = h;
	nRenderState = s;

	pFrameBuffer = new int32_t *[h];
	for (int32_t i = 0; i < h; ++i)
	{
		pFrameBuffer[i] = new int32_t [w];
	}

	pZBuffer = new float *[h];
	for (int32_t i = 0; i < h; ++i)
	{
		pZBuffer[i] = new float [w];
	}
}

void Device::destroy()
{
	for (int32_t i = 0; i < nWinHeight; ++i)
	{
		delete []pFrameBuffer[i];
	}
	delete []pFrameBuffer;
	pFrameBuffer = NULL;

	for (int32_t i = 0; i < nWinHeight; ++i)
	{
		delete []pZBuffer[i];
	}
	delete []pZBuffer;
	pZBuffer = NULL;
}




//////////////////////////////////////////////////////////////////////////
void Device::drawPoly(Poly *poly)
{
	// 线框模式
	if (nRenderState & RENDER_STATE_WIREFRAME)
	{
		Vertex *pVList = poly->vertexList;
		Vertex &v0 = pVList[poly->vertexIndex[0]];
		Vertex &v1 = pVList[poly->vertexIndex[1]];
		Vertex &v2 = pVList[poly->vertexIndex[2]];
		drawLine(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, 8);
		drawLine(v0.pos.x, v0.pos.y, v2.pos.x, v2.pos.y, 8);
		drawLine(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, 8);
	}

	// 纹理渲染
	if (nRenderState & RENDER_STATE_TEXTURE)
	{
		

		
	}
}

void Device::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color)
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

void Device::drawPixel(int32_t x, int32_t y, int32_t color)
{
	if (x < nWinWidth && y < nWinHeight)
	{
		pFrameBuffer[y][x] = color;
	}
	
}
