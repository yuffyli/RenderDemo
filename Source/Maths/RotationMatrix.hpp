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
class EulerAngles;
class Quaternion;

// 物体空间(Object Space)与惯性空间(Inertial Space)之间转换用到的旋转矩阵类定义(3*3)
class RotationMatrix
{
public:
    // 置为单位矩阵
    void identity();
    
    // 根据欧拉角指定的方位构造矩阵
    void fromEulerAngles(const EulerAngles &orientation);
    
    // 根据四元数指定的方位构造矩阵
    void fromInertialToObjectQuaternion(const Quaternion &q);
    void fromObjectToInertialQuaternion(const Quaternion &q);
    
    Vector3 inertialToObject(const Vector3 &v) const;
    Vector3 objectToInertial(const Vector3 &v) const;
    
public:
    /* 这里矩阵表示惯性-物体的变换
        m11, m12, m13
        m21, m22, m23
        m31, m32, m33
        物体-惯性的变换为其转置，即
        m11, m21, m31
        m12, m22, m32
        m13, m23, m33
      */
    float m11, m12, m13;
    float m21, m22, m23;
    float m31, m32, m33;
};


#endif /* RotationMatrix_hpp */
