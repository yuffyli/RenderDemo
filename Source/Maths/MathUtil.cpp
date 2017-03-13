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
    theta += kPI;
    theta -= floorf(theta * k1Over2PI)*k2PI;
    theta -= kPI;
    return theta;
}

float safeAcos(float x)
{
    //  先进行边界检查
    if (x <= -1.0f)
    {
        return kPI;
    }
    
    if (x >= 1.0f)
    {
        return .0f;
    }
    
    return acos(x);
}
