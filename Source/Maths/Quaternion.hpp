//
//  Quaternion.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Quaternion_hpp
#define Quaternion_hpp

#include <stdio.h>
class Vector3;
class EulerAngles;


/*
 [w, (x, y, x)] : w + xi + yj + zk
 i^2 = j^2 = k^2 = -1
 ij = k, ji = -k
 jk = i, kj = -i
 ki = j, ik = -j
  */
class Quaternion
{
public:
    Quaternion()
    {
        
    }
    
    Quaternion(float nw, float nx, float ny, float nz)
    : w(nw)
    , x(nx)
    , y(ny)
    , z(nz)
    {
        
    }
    
    // 置为单位四元数
    void identity()
    {
        w = 1.0f;
        x = y = z = .0f;
    }
    
    // 正则化
    void normalize();
    
    // 提取旋转角
    float getRotationAngle() const;
    
    // 提取旋转轴
    Vector3 getRotationAxis() const;

    // 构造执行旋转的四元数(p.144)
    void setToRotateAboutX(float theta);
    void setToRotateAboutY(float theta);
    void setToRotateAboutZ(float theta);
    void setToRotateAboutAxis(const Vector3 &axis, float theta);
    
    // 根据欧拉角给出的方位参数构造执行物体坐标系-惯性坐标系的旋转的四元数
    void setToRotateObjectToInertial(const EulerAngles &orientation);
    // 根据欧拉角给出的方位参数构造执行惯性坐标系-物体坐标系的旋转的四元数
    void setToRotateInertialToObject(const EulerAngles &orientation);
    
    // 叉乘
    Quaternion operator * (const Quaternion &rhs) const;
    Quaternion & operator *= (const Quaternion &rhs);
    
public:
    float w, x, y, z;
};



// 四元数点乘
extern float dotProduct(const Quaternion &lhs, const Quaternion &rhs);

// 四元数共轭
extern Quaternion conjugate(const Quaternion &q);

// 四元数幂
extern Quaternion pow(const Quaternion &q, float exponent);

// 球面线性插值
extern  Quaternion slerp(const Quaternion &p, const Quaternion &q, float t);

// 全局单位四元数
extern const Quaternion kQuaternionIdentity;

#endif /* Quaternion_hpp */
