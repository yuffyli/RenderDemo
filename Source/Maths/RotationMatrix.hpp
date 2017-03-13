//
//  RotationMatrix.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/12.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef RotationMatrix_hpp
#define RotationMatrix_hpp

#include <stdio.h>

class Vector3;

class RotationMatrix
{
public:
    // 置为单位矩阵
    void identity();
    
    
public:
    float m11, m12, m13;
    float m21, m22, m23;
    float m31, m32, m33;
};


#endif /* RotationMatrix_hpp */
