//
//  MathUtil.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/12.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "MathUtil.hpp"

float wrapPI(float theta)
{
    theta += kPi;
    theta -= floor(theta * k1Over2Pi)*k2Pi;
    theta -= kPi;
    return theta;
}

float safeAcos(float x)
{
    //  先进行边界检查
    if (x <= -1.0f)
    {
        return kPi;
    }
    
    if (x >= 1.0f)
    {
        return .0f;
    }
    
    return acos(x);
}
