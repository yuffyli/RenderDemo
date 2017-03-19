//
//  Vector3.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Vector3_hpp
#define Vector3_hpp

#include <stdio.h>
#include <math.h>

class Vector3
{
public:
    Vector3()
    {
    }
    
    Vector3(const Vector3 &rhs)
    : x(rhs.x)
    , y(rhs.y)
    , z(rhs.z)
    {
    }
    
    Vector3(float newX, float newY, float newZ)
    : x(newX)
    , y(newY)
    , z(newZ)
    {
    }
    
    Vector3 &operator = (const Vector3 &rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    
    // 判断两个向量是否相等
    bool operator == (const Vector3 &rhs) const
    {
        return (x==rhs.x && y==rhs.y && z==rhs.z);
    }
    
    bool operator != (const Vector3 &rhs) const
    {
        return !(*this == rhs);
        //return (x!=rhs.x || y!=rhs.y || z!=rhs.z);
    }
    
    // 置为零向量
    void setZero()
    {
        x = y = z = .0f;
    }
    
    // 向量变负
    Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }
    
    // 向量的加法
    Vector3 operator + (const Vector3 &rhs) const
    {
        return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
    }
    
    Vector3 &operator +=(const Vector3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z +=rhs.z;
        return *this;
    }
    
    // 向量的减法
    Vector3 operator - (const Vector3 &rhs) const
    {
        return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
    }
    
    Vector3 &operator -= (const Vector3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    
    // 标量与向量的乘法
    Vector3 operator * (float k) const
    {
        return Vector3(x*k, y*k, z*k);
    }
    
    Vector3 &operator *= (float k)
    {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }
    
    // 向量与标量的除法
    Vector3 operator / (float k) const
    {
        float oneOverK = 1.0f/k;
        return Vector3(x*oneOverK, y*oneOverK, z*oneOverK);
    }
    
    Vector3 & operator /= (float k)
    {
        float oneOverK = 1.0f/k;
        x *= oneOverK;
        y *= oneOverK;
        z *= oneOverK;
        return *this;
    }
    
    // 向量的点乘
    float operator * (const Vector3 &rhs) const
    {
        return (x*rhs.x + y*rhs.y + z*rhs.z);
    }
    
    // 向量的标准化
    void normalize()
    {
        float fSquare = x*x + y*y + z*z;
        if (fSquare > .0f)
        {
            float fOneOverSqrt = 1.0f/(sqrt(fSquare));
            x *= fOneOverSqrt;
            y *= fOneOverSqrt;
            z *= fOneOverSqrt;
        }
    }
    
public:
    float x;
    float y;
    float z;
};


// 向量相加
inline Vector3 operator + (const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs) += rhs;
}

// 向量相减
inline Vector3 operator - (const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs) -= rhs;
}

// 向量左乘标量
inline Vector3 operator * (const Vector3 &lhs, float k)
{
    return Vector3(lhs) *= k;
}

// 标量左乘
inline Vector3 operator * (float k, const Vector3 &rhs)
{
    return Vector3(rhs) *= k;
}

//// 向量除以标量
//const Vector3 operator / (const Vector3 &lhs, float k)
//{
//    return Vector3(lhs) /= k;
//}

// 计算向量大小（模）
inline float vectorMag(const Vector3 &v)
{
    return  sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// 向量的叉乘
inline Vector3 crossProduct(const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(
        lhs.y*rhs.z - lhs.z*rhs.y,
        lhs.z*rhs.x - lhs.x*rhs.z,
        lhs.x*rhs.y - lhs.y*rhs.x
    );
}

// 计算两点距离
inline float distance(const Vector3 &lhs, const Vector3 &rhs)
{
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    float dz = lhs.z - rhs.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// 距离平方
inline float distanceSquared(const Vector3 &lhs, const Vector3 &rhs)
{
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    float dz = lhs.z - rhs.z;
    return (dx*dx + dy*dy + dz*dz);
}


// 全局零向量
extern const Vector3 kZeroVector;

#endif /* Vector3_hpp */
