//
//  MathUtil.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/12.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef MathUtil_hpp
#define MathUtil_hpp

#include <stdio.h>
#include <math.h>

const float kPI = 3.14159265f;
const float k2PI = 2.0f *kPI;
const float kPIOver2 = kPI * 0.5f;
const float k1OverPI = 1.0f/kPI;
const float k1Over2PI = 1.0f/k2PI;

// 将theta限制在[-PI, PI]内
extern float wrapPI(float theta);

// “安全”反三角函数
extern float safeAcos(float x);


inline void sinCos(float *returnSin, float *returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}




#endif /* MathUtil_hpp */
