//
//  EulerAngles.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//

//#include "StdAfx.h"
#include <math.h>
#include "EulerAngles.hpp"
#include "Quaternion.hpp"
#include "Matrix4x3.hpp"
#include "RotationMatrix.hpp"
#include "MathUtil.hpp"

const EulerAngles kEulerAnglesIdentity(.0f, .0f, .0f);


void EulerAngles::canonize()
{
    // 将pitch转换到 [-pi, pi] 之间
    pitch = wrapPI(pitch);
    // 再将pitch转换到 [-pi/2, pi/2]之间
    if (pitch < -kPiOver2)
    {
        pitch = -kPi - pitch;
        heading += kPi;
        bank += kPi;
    }
    else if(pitch > kPiOver2)
    {
        pitch = kPi - pitch;
        heading += kPi;
        bank += kPi;
    }
    
    // 检查万向锁
    if (fabs(pitch) > kPiOver2 - 1e-4)
    {
        heading += bank;
        bank = .0f;
    }
    else
    {
        bank = wrapPI(bank);
    }
    
    heading = wrapPI(heading);
}

void EulerAngles::fromObjectToInertialQuaternion(const Quaternion &q)
{
    // p.172 从惯性-物体四元数转换到欧拉角
    // 先计算sin(pitch)
    float sp = -2.0f * (q.y*q.z + q.w*q.x);
    
    // 检查万向锁
    if (fabs(sp) > 0.9999f)
    {
        pitch = kPiOver2*sp;
        heading = atan2(-q.x*q.z - q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
        bank = 0.0f;
    }
    else
    {
        pitch = asin(sp);
        heading	= atan2(q.x*q.z - q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
        bank	= atan2(q.x*q.y - q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
    }
}

void EulerAngles::fromInertialToObjectQuaternion(const Quaternion &q)
{
    // p.173 从物体-惯性四元数转换到欧拉角
    // 与fromObjectToInertialQuaternion类似，只是将x,y,z的值变负
    // 因为物体-惯性四元数 和 惯性-物体四元数 互为共轭
    float sp = -2.0f * (q.y*q.z + q.w*q.x);
    
    // 检查万向锁
    if (fabs(sp) > 0.9999f)
    {
        pitch = kPiOver2*sp;
        heading = atan2(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
        bank = 0.0f;
    }
    else
    {
        pitch = asin(sp);
        heading	= atan2(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
        bank	= atan2(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
    }
}

void EulerAngles::fromObjectToWorldMatrix(const RotationMatrix &m)
{
    // p.162 从物体-世界变换矩阵 转换到欧拉角
    float	sp = -m.m32;
    
    // 检查万向锁
    if (fabs(sp) > 9.99999f)
    {
        pitch = kPiOver2 * sp;
        heading = atan2(-m.m23, m.m11);
        bank = 0.0f;
    }
    else
    {
        heading = atan2(m.m31, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m12, m.m22);
    }
    
}

void EulerAngles::fromWorldToObjectMatrix(const RotationMatrix &m)
{
    // p.162 从世界-物体变换矩阵 转换到欧拉角
    
    float	sp = -m.m23;
    
    // 检查万向锁
    if (fabs(sp) > 9.99999f)
    {
        pitch = kPiOver2 * sp;
        heading = atan2(-m.m31, m.m11);
        bank = 0.0f;
        
    }
    else
    {
        heading = atan2(m.m13, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m21, m.m22);
    }
    
}

void EulerAngles::fromRotationMatrix(const RotationMatrix &m)
{
    // 根据旋转矩阵构造欧拉角
    // 这里的旋转矩阵代表惯性-物体的旋转，其实跟世界-物体的旋转一样
    float	sp = -m.m23;
    
    // 检查万向锁
    if (fabs(sp) > 9.99999f)
    {
        pitch = kPiOver2 * sp;
        heading = atan2(-m.m31, m.m11);
        bank = 0.0f;
    }
    else {
        heading = atan2(m.m13, m.m33);
        pitch = asin(sp);
        bank = atan2(m.m21, m.m22);
    }
}
