//
//  Render.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>

#include "Macro.hpp"

class Object;
struct Poly;
struct Trapezoid;
struct Line;
struct Vertex;
class Texture;

class Render
{
public:
	void init(int32_t w, int32_t h, int32_t renderState, int32_t depthState, unsigned char *fb);
	void destroy();

	void initBuffer(int32_t w, int32_t h, void *fb);

	void clearBuffer();

	//void initTexture();
	//void setupTexture(void *pBits, int32_t tW, int32_t tH);

	void initLineByY(Trapezoid *pTrapezoid, Line *pLine, int32_t y);
	void drawTexture(Texture *pTexture);
	void drawObject(Object *pObject);
	void drawPoly(Poly *pPoly);
	void drawTriangle(Vertex *pV0, Vertex *pV1, Vertex *pV2);
	void drawTriangle(Texture *pTexture, Vertex *pV0, Vertex *pV1, Vertex *pV2);
	void drawTrapezoid(Texture *pTexture, Trapezoid *pTrapezoid);
	void drawHorizontalLine(Texture *pTexture, Line *pLine);

	void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
	void drawPixel(int32_t x, int32_t y, uint32_t color);

	
public:
	int32_t nWinWidth;
	int32_t nWinHeight;
	int32_t nRenderState;
	int32_t nDepthState;

	uint32_t **pFrameBuffer;	// 像素缓存
	float **pZBuffer;				// 深度缓存

	int32_t nTexWidth;
	int32_t nTexHeight;
	float fMaxU;
	float fMaxV;
	//uint32_t **pTexture;			// 纹理
};



#endif /* Render_hpp */
