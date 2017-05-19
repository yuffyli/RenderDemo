//
//  Texture.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include "Macro.hpp"



class Texture
{
public:
	void init(int32_t tW, int32_t tH);
	void setupTexture();
	void destroy();

	uint32_t getColorByUV(float u, float v);

public:
	int32_t nTexWidth;
	int32_t nTexHeight;
	float fMaxU;
	float fMaxV;
	uint32_t **pTexture;			// 纹理
};





#endif /* Texture_hpp */
