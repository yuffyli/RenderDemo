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

struct Poly;

class Device
{
public:
	void init(int32_t w, int32_t h, int32_t s);
	void destroy();

	void drawPoly(Poly *poly);
	void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color);
	void drawPixel(int32_t x, int32_t y, int32_t color);
	

public:
	int32_t nWinWidth;
	int32_t nWinHeight;
	int32_t nRenderState;

	int32_t **pFrameBuffer;	// 像素缓存
	float **pZBuffer;				// 深度缓存

	int32_t nTexWidth;
	int32_t nTexHeight;
	int32_t **pTexture;			// 纹理

};





#endif /* Render_hpp */
