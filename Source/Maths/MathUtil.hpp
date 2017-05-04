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

const float kPi = 3.14159265f;
const float k2Pi = 2.0f *kPi;
const float kPiOver2 = kPi * 0.5f;
const float k1OverPi = 1.0f/kPi;
const float k1Over2Pi = 1.0f/k2Pi;
const float kPiOver180 = kPi/180.0f;
const float k180OverPi = 180.0f/kPi;

// 将theta限制在[-PI, PI]内
extern float wrapPI(float theta);

// “安全”反三角函数(通过对边界进行检查，返回合适的角度)
extern float safeAcos(float x);

// 角度转弧度
inline float degreeToRadian(float degree)
{
    return degree * kPiOver180;
}

// 弧度转角度
inline float RadianToDegree(float radian)
{
    return radian * k180OverPi;
}

// 视场中的缩放(zoom)和视场角(fov)之间的转换
inline float fovToZoom(float fov)
{
    return 1.0f/tan(fov*.5f);
}

inline float zoomToFov(float zoom)
{
    return 2.0f*atan(1.0f/zoom);
}

// 计算theta的sin值和cos值
inline void sinCos(float *returnSin, float *returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}








#endif /* MathUtil_hpp */
