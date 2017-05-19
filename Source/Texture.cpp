//
//  Texture.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Texture.hpp"
#include "MathUtil.hpp"

//////////////////////////////////////////////////////////////////////////
void Texture::init(int32_t tW, int32_t tH)
{
	pTexture = new uint32_t *[tH];
	for (int32_t j = 0; j < tH; ++j)
	{
		pTexture[j] = new uint32_t[tW];
	}

	nTexWidth = tW;
	nTexHeight = tH;
	fMaxU = (float)(tW-1);
	fMaxV = (float)(tH-1);

	setupTexture();
}

void Texture::setupTexture()
{
	// 将纹理分成32x32的小块，并为每个小块填充同一种颜色
	uint32_t color[4] = {COLOR0, COLOR1, COLOR2, COLOR3};
	for (int32_t j = 0; j < nTexHeight; ++j)
	{
		for (int32_t i = 0; i < nTexWidth; ++i)
		{
			int32_t x = i/32;
			int32_t y = j/32;
			pTexture[j][i] = color[(x+y)%4];
		}
	}
}

void Texture::destroy()
{
	for (int32_t j = 0; j < nTexHeight; ++j)
	{
		delete []pTexture[j];
	}

	delete []pTexture;
	pTexture = NULL;
}

uint32_t Texture::getColorByUV(float u, float v)
{
	int32_t x = middle(0, nTexWidth, rounding(u*fMaxU));
	int32_t y = middle(0, nTexHeight, rounding(v*fMaxV));
	return pTexture[y][x];
}
