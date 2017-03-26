//
//  Matrix4x3.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include <assert.h>
#include <math.h>

#include "Matrix4x3.hpp"
#include "MathUtil.hpp"
#include "Vector3.hpp"
#include "EulerAngles.hpp"
#include "Quaternion.hpp"
#include "RotationMatrix.hpp"

void Matrix4x3::identity()
{
    m11 = 1.0f; m12 = .0f; m13 = .0f;
    m21 = .0f; m22 = 1.0f; m23 = .0f;
    m31 = .0f; m32 = .0f; m33 = 1.0f;
    tx = .0f; ty = .0f; tz = .0f;
}

void Matrix4x3::zeroTranslation()
{
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setTranslation(const Vector3 &d)
{
    // 平移部分赋值
    tx = d.x; ty = d.y; tz = d.z;
}

void Matrix4x3::setupTranslation(const Vector3 &d)
{
    // 线性变换部分置为单位矩阵，平移部分赋值
    m11 = 1.0f; m12 = .0f; m13 = .0f;
    m21 = .0f; m22 = 1.0f; m23 = .0f;
    m31 = .0f; m32 = .0f; m33 = 1.0f;
    
    tx = d.x; ty = d.y; tz = d.z;
}

void Matrix4x3::setupLocalToParent(const Vector3 &pos, const EulerAngles &orient)
{
    RotationMatrix orientMatrix;
    orientMatrix.setup(orient);
    setupLocalToParent(pos, orientMatrix);
}

void Matrix4x3::setupLocalToParent(const Vector3 &pos, const RotationMatrix &orient)
{
    // 约定旋转矩阵为 惯性->物体 的矩阵(父坐标->局部坐标)，所以这里要做转置
    m11 = orient.m11; m12 = orient.m21; m13 = orient.m31;
    m21 = orient.m12; m22 = orient.m22; m23 = orient.m32;
    m31 = orient.m13; m32 = orient.m23; m33 = orient.m33;
    
    tx = pos.x; ty = pos.y; tz = pos.z;
}

void Matrix4x3::setupParentToLocal(const Vector3 &pos, const EulerAngles &orient)
{
    RotationMatrix orientMatrix;
    orientMatrix.setup(orient);
    
    setupParentToLocal(pos, orientMatrix);
}

void Matrix4x3::setupParentToLocal(const Vector3 &pos, const RotationMatrix &orient)
{
    /* 
     构造两个矩阵T和R，再连接M=TR (先旋转再平移)
     | 1    0   0    0 |    | o.m11 o.m12 o.m13 0 |
     | 0    1   0    0 | *  | o.m21 o.m22 o.m23 0 |
     | 0    0   1    0 |    | o.m31 o.m32 o.m33 0 |
     |-p.x -p.y -p.z 1|     | 0         0       1     0 |
     */
    m11 = orient.m11; m12 = orient.m12; m13 = orient.m13;
    m21 = orient.m21; m22 = orient.m22; m23 = orient.m23;
    m31 = orient.m31; m32 = orient.m32; m33 = orient.m33;
    
    tx =  -(pos.x*m11 + pos.y*m21 + pos.z*m31);
    ty =  -(pos.x*m12 + pos.y*m22 + pos.z*m32);
    tx =  -(pos.x*m13 + pos.y*m23 + pos.z*m33);
}

void Matrix4x3::setupRotate(int axis, float theta)
{
    float sinTheta, cosTheta;
    sinCos(&sinTheta, &cosTheta, theta);
    switch (axis)
    {
        case 1:
            // 绕x轴旋转 p.90
            m11 = 1.0f; m12 = .0f; m13 = .0f;
            m21 = .0f; m22 = cosTheta; m23 = sinTheta;
            m31 = .0f; m32 = -sinTheta; m33 = cosTheta;
            break;
        case 2:
            // 绕y轴旋转 p.90
            m11 = cosTheta; m12 = .0f; m13 = -sinTheta;
            m21 = .0f; m22 = 1.0f; m23 = .0f;
            m31 = sinTheta; m32 = .0f; m33 = cosTheta;
            break;
        case 3:
            // 绕z轴旋转 p.91
            m11 = cosTheta; m12 = sinTheta; m13 = .0f;
            m21 = -sinTheta; m22 = cosTheta; m23 = .0f;
            m31 = .0f; m32 = .0f; m33 = 1.0f;
            break;
        default:
            assert(false);
            break;
    }
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupRotate(const Vector3 &axis, float theta)
{
    // 旋转轴必须为单位向量
    assert(fabs(axis*axis) - 1.0f < .01f);
    
    float sinTheta, cosTheta;
    sinCos(&sinTheta, &cosTheta, theta);
    
    // p.94
    float a = 1.0f - cosTheta;
    float ax = a * axis.x;
    float ay = a * axis.y;
    float az = a * axis.z;
    
    m11 = ax*axis.x + cosTheta;
    m12 = ax*axis.y + axis.z*sinTheta;
    m13 = ax*axis.z - axis.y*sinTheta;
    
    m21 = ay*axis.x - axis.z*sinTheta;
    m22 = ay*axis.y + cosTheta;
    m23 = ay*axis.z + axis.x*sinTheta;
    
    m31 = az*axis.x + axis.y*sinTheta;
    m32 = az*axis.y - axis.x*sinTheta;
    m33 = az*axis.z + cosTheta;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::fromQuternion(const Quaternion &q)
{
    // p.167
    float	ww = 2.0f * q.w;
    float	xx = 2.0f * q.x;
    float	yy = 2.0f * q.y;
    float	zz = 2.0f * q.z;
    
    m11 = 1.0f - yy*q.y - zz*q.z;
    m12 = xx*q.y + ww*q.z;
    m13 = xx*q.z - ww*q.x;
    
    m21 = xx*q.y - ww*q.z;
    m22 = 1.0f - xx*q.x - zz*q.z;
    m23 = yy*q.z + ww*q.x;
    
    m31 = xx*q.z + ww*q.y;
    m32 = yy*q.z - ww*q.x;
    m33 = 1.0f - xx*q.x - yy*q.y;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupScale(const Vector3 &s)
{
    // p.96  Formula8.7 沿坐标轴的3D缩放矩阵
    m11 = s.x; m12 = .0f; m13 = .0f;
    m21 = .0f; m22 = s.y; m23 = .0f;
    m31 = .0f; m32 = .0f; m33 = s.z;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupScaleAlongAxis(const Vector3 &axis, float k)
{
    // p.98  Formula8.9 沿任意轴的3D缩放矩阵
    
    // 旋转轴必须为单位向量
    assert(fabs(axis*axis - 1.0f) < .01f);
    
    float	a = k - 1.0f;
    float	ax = a * axis.x;
    float	ay = a * axis.y;
    float	az = a * axis.z;
    
    // 先计算对角线上的元素
    m11 = ax*axis.x + 1.0f;
    m22 = ay*axis.y + 1.0f;
    m32 = az*axis.z + 1.0f;
    
    // 对角元素相等
    m12 = m21 = ax*axis.y;
    m13 = m31 = ax*axis.z;
    m23 = m32 = ay*axis.z;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupShear(int axis, float s, float t)
{
    // p.103  Formula8.13 3D切变矩阵
    
    switch (axis)
    {
        case 1:
            // 用x切变y和z: y' = y+s*x; z' = z+t*x
            m11 = 1.0f; m12 = s;    m13 = t;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            break;
        case 2:
            // 用y切变x和z: x' = x+s*y; z' = z+t*y
            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m21 = s;    m22 = 1.0f; m23 = t;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
            break;
        case 3:
            // 用z切变x和y: x' = x+s*z; y' = y+s*z
            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = s;    m32 = t;    m33 = 1.0f;
            break;
        default:
            assert(false);
    }
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupProject(const Vector3 &n)
{
    // p.101  Formula8.16 向任意平面投影的3D矩阵
    
    // 检查n是否为单位向量
    assert(fabs(n*n - 1.0f) < .01f);
    
    m11 = 1.0f - n.x*n.x;
    m22 = 1.0f - n.y*n.y;
    m33 = 1.0f - n.z*n.z;
    
    m12 = m21 = -n.x*n.y;
    m13 = m31 = -n.x*n.z;
    m23 = m32 = -n.y*n.z;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}

void Matrix4x3::setupReflect(int axis, float k /* = .0f */)
{
    // p.102  Formula8.18 向任意平面投影的3D矩阵
    switch (axis)
    {
        case 1:
            // 沿平面x=k反射
            m11 = -1.0f; m12 =  0.0f; m13 =  0.0f;
            m21 =  0.0f; m22 =  1.0f; m23 =  0.0f;
            m31 =  0.0f; m32 =  0.0f; m33 =  1.0f;
            
            tx = 2.0f * k;
            ty = 0.0f;
            tz = 0.0f;
            
            break;
        case 2:
            // 沿平面y=k反射
            m11 =  1.0f; m12 =  0.0f; m13 =  0.0f;
            m21 =  0.0f; m22 = -1.0f; m23 =  0.0f;
            m31 =  0.0f; m32 =  0.0f; m33 =  1.0f;
            
            tx = 0.0f;
            ty = 2.0f * k;
            tz = 0.0f;
            
            break;
        case 3:
            // 沿平面z=k反射
            m11 =  1.0f; m12 =  0.0f; m13 =  0.0f;
            m21 =  0.0f; m22 =  1.0f; m23 =  0.0f;
            m31 =  0.0f; m32 =  0.0f; m33 = -1.0f;
            
            tx = 0.0f;
            ty = 0.0f;
            tz = 2.0f * k;
            
            break;
        default:
            assert(false);
            break;
    }
}

void Matrix4x3::setupReflect(const Vector3 &n)
{
    // p.102  Formula8.18 向任意平面投影的3D矩阵
    
    // 检查n是否为单位向量
    assert(fabs(n*n - 1.0f) < .01f);
    
    float	ax = -2.0f * n.x;
    float	ay = -2.0f * n.y;
    float	az = -2.0f * n.z;
    
    m11 = 1.0f + ax*n.x;
    m22 = 1.0f + ay*n.y;
    m32 = 1.0f + az*n.z;
    
    m12 = m21 = ax*n.y;
    m13 = m31 = ax*n.z;
    m23 = m32 = ay*n.z;
    
    // 平移部分置为0
    tx = ty = tz = .0f;
}


//////////////////////////////////////////////////////////////////////////
Vector3 operator * (const Vector3 &p, const Matrix4x3 &m)
{
    return Vector3(
                   p.x*m.m11 + p.y*m.m21 + p.z*m.m31 + m.tx,
                   p.x*m.m12 + p.y*m.m22 + p.z*m.m32 + m.ty,
                   p.x*m.m13 + p.y*m.m23 + p.z*m.m33 + m.tz
                   );
}

Vector3 &operator *= (Vector3 &p, const Matrix4x3 &m)
{
    p = p * m;
    return p;
}


Matrix4x3 operator * (const Matrix4x3 &lhs, const Matrix4x3 &rhs)
{
    Matrix4x3 m;
    
    m.m11 = lhs.m11*rhs.m11 + lhs.m12*rhs.m21 + lhs.m13*rhs.m31;
    m.m12 = lhs.m11*rhs.m12 + lhs.m12*rhs.m22 + lhs.m13*rhs.m32;
    m.m13 = lhs.m11*rhs.m13 + lhs.m12*rhs.m23 + lhs.m13*rhs.m33;
    
    m.m21 = lhs.m21*rhs.m11 + lhs.m22*rhs.m21 + lhs.m23*rhs.m31;
    m.m22 = lhs.m21*rhs.m12 + lhs.m22*rhs.m22 + lhs.m23*rhs.m32;
    m.m23 = lhs.m21*rhs.m13 + lhs.m22*rhs.m23 + lhs.m23*rhs.m33;
    
    m.m31 = lhs.m31*rhs.m11 + lhs.m32*rhs.m21 + lhs.m33*rhs.m31;
    m.m32 = lhs.m31*rhs.m12 + lhs.m32*rhs.m22 + lhs.m33*rhs.m32;
    m.m33 = lhs.m31*rhs.m13 + lhs.m32*rhs.m23 + lhs.m33*rhs.m33;
    
    m.tx = lhs.tx*rhs.m11 + lhs.ty*rhs.m21 + lhs.tz*rhs.m31 + rhs.tx;
    m.ty = lhs.tx*rhs.m12 + lhs.ty*rhs.m22 + lhs.tz*rhs.m32 + rhs.ty;
    m.tz = lhs.tx*rhs.m13 + lhs.ty*rhs.m23 + lhs.tz*rhs.m33 + rhs.tz;
    
    return m;
}

Matrix4x3 &operator *=(Matrix4x3 &lhs, const Matrix4x3 &rhs)
{
    lhs = lhs * rhs;
    return lhs;
}


float determinant(const Matrix4x3 &m)
{
    // p.112
    return
		  m.m11 * (m.m22*m.m33 - m.m23*m.m32)
        + m.m12 * (m.m23*m.m31 - m.m21*m.m33)
        + m.m13 * (m.m21*m.m32 - m.m22*m.m31);
}

Matrix4x3 inverse(const Matrix4x3 &m)
{
    //  先计算行列式
    float	det = determinant(m);
    // 检查行列式是否为0（奇异矩阵没有逆矩阵）
    assert(fabs(det) > 0.000001f);
    
    float	oneOverDet = 1.0f / det;
    
    Matrix4x3	r;
    
    // 计算3x3部分的逆
    r.m11 = (m.m22*m.m33 - m.m23*m.m32) * oneOverDet;
    r.m12 = (m.m13*m.m32 - m.m12*m.m33) * oneOverDet;
    r.m13 = (m.m12*m.m23 - m.m13*m.m22) * oneOverDet;
    
    r.m21 = (m.m23*m.m31 - m.m21*m.m33) * oneOverDet;
    r.m22 = (m.m11*m.m33 - m.m13*m.m31) * oneOverDet;
    r.m23 = (m.m13*m.m21 - m.m11*m.m23) * oneOverDet;
    
    r.m31 = (m.m21*m.m32 - m.m22*m.m31) * oneOverDet;
    r.m32 = (m.m12*m.m31 - m.m11*m.m32) * oneOverDet;
    r.m33 = (m.m11*m.m22 - m.m12*m.m21) * oneOverDet;
    
    // 计算平移部分的逆
    r.tx = -(m.tx*r.m11 + m.ty*r.m21 + m.tz*r.m31);
    r.ty = -(m.tx*r.m12 + m.ty*r.m22 + m.tz*r.m32);
    r.tz = -(m.tx*r.m13 + m.ty*r.m23 + m.tz*r.m33);
    
    return r;
}

Vector3 getTranslation(const Matrix4x3 &m)
{
    return Vector3(m.tx, m.ty, m.tz);
}

Vector3 getPositionFromParentToLocalMatrix(const Matrix4x3 &m)
{
    // 父->局部（世界->物体）变换中物体的位置其实就是负的平移乘以3x3部分的逆
    // 这里假设矩阵是正交的（逆和转置相等）
    return Vector3(
                   -(m.tx*m.m11 + m.ty*m.m12 + m.tz*m.m13),
                   -(m.tx*m.m21 + m.ty*m.m22 + m.tz*m.m23),
                   -(m.tx*m.m31 + m.ty*m.m32 + m.tz*m.m33)
                   );
}

Vector3 getPositionFromLocalToParentMatrix(const Matrix4x3 &m)
{
    // 局部->父（物体->世界）变换中物体的位置其实就是平移部分
    return Vector3(m.tx, m.ty, m.tz);
}

