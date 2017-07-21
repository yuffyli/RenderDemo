//
//  Quaternion.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//
// #include "StdAfx.h"
#include <assert.h>
#include <math.h>
#include "Quaternion.hpp"
#include "Vector3.hpp"
#include "EulerAngles.hpp"
#include "MathUtil.hpp"

const Quaternion kQuaternionIdentity = Quaternion(1.0f, .0f, .0f, .0f);

void Quaternion::normalize()
{
    float mag = (float)sqrt(w*w + x*x + y*y + z*z);
    
    // 这里必须检测除0的情况
    if (mag > .0f)
    {
        float oneOverMag = 1.0f/mag;
        w *= oneOverMag;
        x *= oneOverMag;
        y *= oneOverMag;
        z *= oneOverMag;
    }
    else
    {
        assert(false);
        
        identity();
    }
}

float Quaternion::getRotationAngle() const
{
    // 由 w=cos(theta/2)算出theta/2
    float theataOver2 = safeAcos(w);
    return 2.0f*theataOver2;
}

Vector3 Quaternion::getRotationAxis() const
{
    // 先算出sin^2(theta/2)
    float sinThetaOver2Square = 1.0f-w*w;
    
    if (sinThetaOver2Square <= .0f)
    {
        return Vector3(1.0f, .0f, .0f);
    }
    
    // sin(theta/2)*axis.x = x
    float oneOverSinThetaOver2 = 1.0f/sqrt(sinThetaOver2Square);
    return Vector3(
                   x*oneOverSinThetaOver2,
                   y*oneOverSinThetaOver2,
                   z*oneOverSinThetaOver2
                   );
}

void Quaternion::setToRotateAboutX(float theta)
{
    float thetaOver2 = 0.5f*theta;
    w = cos(thetaOver2);
    x = sin(thetaOver2);
    y = .0f;
    z = .0f;
}

void Quaternion::setToRotateAboutY(float theta)
{
    float thetaOver2 = 0.5f*theta;
    w = cos(thetaOver2);
    x = .0f;
    y = sin(thetaOver2);
    z = .0f;
}

void Quaternion::setToRotateAboutZ(float theta)
{
    float thetaOver2 = 0.5f*theta;
    w = cos(thetaOver2);
    x = .0f;
    y = .0f;
    z = sin(thetaOver2);
}

void Quaternion::setToRotateAboutAxis(const Vector3 &axis, float theta)
{
    // 旋转轴必须标准化
    assert(fabs(vectorMag(axis)-1.0f) < .01f);
    float thetaOver2 = 0.5f*theta;
    w = cos(thetaOver2);
    x = sin(thetaOver2) * axis.x;
    y = sin(thetaOver2) * axis.y;
    z = sin(thetaOver2) * axis.z;
}

void Quaternion::setToRotateObjectToInertial(const EulerAngles &orientation)
{
    // p.171
    float sp, sb, sh;
    float cp, cb, ch;
    sinCos(&sp, &cp, 0.5f*orientation.pitch);
    sinCos(&sb, &cb, 0.5f*orientation.bank);
    sinCos(&sh, &ch, 0.5f*orientation.heading);
    
    w = ch*cp*cb + sh*sp*sb;
    x =  ch*sp*cb + sh*cp*sb;
    y = -ch*sp*sb + sh*cp*cb;
    z = -sh*sp*cb + ch*cp*sb;
}

void Quaternion::setToRotateInertialToObject(const EulerAngles &orientation)
{
    // 惯性-物体四元数 是 物体-惯性四元数 的共轭
    float sp, sb, sh;
    float cp, cb, ch;
    sinCos(&sp, &cp, 0.5f*orientation.pitch);
    sinCos(&sb, &cb, 0.5f*orientation.bank);
    sinCos(&sh, &ch, 0.5f*orientation.heading);
    
    w =  ch*cp*cb + sh*sp*sb;
    x = -ch*sp*cb - sh*cp*sb;
    y =  ch*sp*sb - sh*cb*cp;
    z =  sh*sp*cb - ch*cp*sb;
}

Quaternion Quaternion::operator*(const Quaternion &rhs) const
{
    // [w1 v1][w2 v2] = [w1w2-v1v2 w1v2+w2v1+v2xv1]
    Quaternion result;
    result.w = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
    result.x = w*rhs.x + x*rhs.w + z*rhs.y - y*rhs.z;
    result.y = w*rhs.y + y*rhs.w + x*rhs.z - z*rhs.x;
    result.z = w*rhs.z + z*rhs.w + y*rhs.x - x*rhs.y;
    
    return result;
}

Quaternion &Quaternion::operator*=(const Quaternion &rhs)
{
    *this = *this * rhs;
    return *this;
}

///////////////////////////////////////////////////////
float dotProduct(const Quaternion &lhs, const Quaternion &rhs)
{
    return (lhs.w*rhs.w + lhs.x+rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}

Quaternion conjugate(const Quaternion &q)
{
    return Quaternion(q.w, -q.x, -q.y, -q.z);
}

Quaternion pow(const Quaternion &q, float exponent)
{
    if (fabs(q.w) > 0.99999f)
    {
        return q;
    }
    
    // alpha = theta/2
    float alpha = acos(q.w);
    float newAlpha = alpha*exponent;
    float nw = cos(newAlpha);
    float mult = sin(newAlpha)/sin(alpha);
    
    return Quaternion(nw, q.x*mult, q.y*mult, q.z*mult);
}

Quaternion slerp(const Quaternion &q0, const Quaternion &q1, float t)
{
    // p.156 slerp(q0, q1, t) = (sin((1-t)*omega)/sin omega)*q0 + (sin(t*omega))/(sin omega) * q1
    // 先做边界检查
    if (t <= .0f)
    {
        return q0;
    }
    
    if (t >= 1.0f)
    {
        return q1;
    }
    
    // 计算两个值的差(即用点乘计算两个四元数之间的夹角)
    float q1w = q1.w;
    float q1x = q1.x;
    float q1y = q1.y;
    float q1z = q1.z;
    
    float cosOmega = dotProduct(q0, q1);
    // 如果点乘为负，则反转一个四元数以取得短的“4D”弧
    if (cosOmega < .0f)
    {
        q1w = -q1w;
        q1x = -q1x;
        q1y = -q1y;
        q1z = -q1z;
        cosOmega = -cosOmega;
    }
    
    assert(cosOmega < 1.1f);
    
    float k0, k1;
    if (cosOmega > 0.9999f)
    {
        k0 = 1.0f-t;
        k1 = t;
    }
    else
    {
        float sinOmega = sqrt(1.0f - cosOmega*cosOmega);
        float omega = atan2(sinOmega, cosOmega);
        float oneOverSinOmega = 1.0f/sinOmega;
        
        k0 = sin((1.0f-t)*omega) * oneOverSinOmega;
        k1 = sin(t*omega) * oneOverSinOmega;
    }
    
    return Quaternion(
                      k0*q0.w + k1*q1w,
                      k0*q0.x + k1*q1x,
                      k0*q0.y + k1*q1y,
                      k0*q0.z + k1*q1z
    );
}







