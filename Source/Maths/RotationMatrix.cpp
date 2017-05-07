//
//  RotationMatrix.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/12.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include <math.h>
#include "RotationMatrix.hpp"
#include "EulerAngles.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"
#include "MathUtil.hpp"

void RotationMatrix::identity()
{
    m11 = 1.0f; m12 = .0f; m13 = .0f;
    m21 = .0f; m22 = 1.0f; m23 = .0f;
    m31 = .0f; m32 = .0f; m33 = 1.0f;
}

void RotationMatrix::setupUVN(const Vector3 &u, const Vector3 &v, const Vector3 &n)
{
    m11 = u.x; m12 = u.y; m13 = u.z;
    m21 = v.x; m22 = v.y; m23 = v.z;
    m31 = n.x; m32 = n.y; m33 = n.z;
}

void RotationMatrix::setup(const EulerAngles &orientation)
{
    float sh, sp, sb;
    float ch, cp, cb;
    sinCos(&sh, &ch, orientation.heading);
    sinCos(&sp, &cp, orientation.pitch);
    sinCos(&sb, &cb, orientation.bank);
    
    m11 = ch * cb + sh * sp * sb;
    m12 = -ch * sb + sh * sp * cb;
    m13 = sh * cp;
    
    m21 = sb * cp;
    m22 = cb * cp;
    m23 = -sp;
    
    m31 = -sh * cb + ch * sp * sb;
    m32 = sb * sh + ch * sp * cb;
    m33 = ch * cp;
}

void RotationMatrix::fromInertialToObjectQuaternion(const Quaternion &q)
{
    m11 = 1.0f - 2.0f * (q.y*q.y + q.z*q.z);
    m12 = 2.0f * (q.x*q.y + q.w*q.z);
    m13 = 2.0f * (q.x*q.z - q.w*q.y);
    
    m21 = 2.0f * (q.x*q.y - q.w*q.z);
    m22 = 1.0f - 2.0f * (q.x*q.x + q.z*q.z);
    m23 = 2.0f * (q.y*q.z + q.w*q.x);
    
    m31 = 2.0f * (q.x*q.z + q.w*q.y);
    m32 = 2.0f * (q.y*q.z - q.w*q.x);
    m33 = 1.0f - 2.0f * (q.x*q.x + q.y*q.y);
}

void RotationMatrix::fromObjectToInertialQuaternion(const Quaternion &q)
{
    m11 = 1.0f - 2.0f * (q.y*q.y + q.z*q.z);
    m12 = 2.0f * (q.x*q.y - q.w*q.z);
    m13 = 2.0f * (q.x*q.z + q.w*q.y);
    
    m21 = 2.0f * (q.x*q.y + q.w*q.z);
    m22 = 1.0f - 2.0f * (q.x*q.x + q.z*q.z);
    m23 = 2.0f * (q.y*q.z - q.w*q.x);
    
    m31 = 2.0f * (q.x*q.z - q.w*q.y);
    m32 = 2.0f * (q.y*q.z + q.w*q.x);
    m33 = 1.0f - 2.0f * (q.x*q.x + q.y*q.y);
}

Vector3 RotationMatrix::inertialToObject(const Vector3 &v) const
{
    /*
                      | m11 m12 m13 |
        [ix iy iz] * | m21 m22 m23 | = [ox oy oz]
                      | m31 m32 m33 |
      */
    return Vector3(
        m11*v.x + m21*v.y + m31*v.z,
        m12*v.x + m22*v.y + m32*v.z,
        m13*v.z + m23*v.y + m33*v.z
    );
}

Vector3 RotationMatrix::objectToInertial(const Vector3 &v) const
{
    /*
                        | m11 m21 m31 |
        [ox oy oz] * | m12 m22 m32 | = [ix iy iz]
                        | m13 m23 m33 |
     */
    return Vector3(
                   m11*v.x + m12*v.y + m13*v.z,
                   m21*v.x + m22*v.y + m23*v.z,
                   m31*v.z + m32*v.y + m33*v.z
                   );
}



RotationMatrix inverse(const RotationMatrix &m)
{
    RotationMatrix r;
    r.m11 = m.m11; r.m12 = m.m21; r.m13 = m.m31;
    r.m21 = m.m12; r.m22 = m.m22; r.m23 = m.m32;
    r.m31 = m.m13; r.m32 = m.m23; r.m33 = m.m33;
    
    return r;
}
