//
//  EulerAngles.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef EulerAngles_hpp
#define EulerAngles_hpp

#include <stdio.h>


class Quaternion;
class Matrix4x3;
class RotationMatrix;

// 基本思想：将角位移分解为绕三个互相垂直轴的三个旋转组成的序列
// heading-pitch-bank（惯性坐标系-物体坐标系）
class EulerAngles
{
public:
    EulerAngles()
    {
        
    }
    
    EulerAngles(float h, float p, float b)
    : heading(h)
    , pitch(p)
    , bank(b)
    {
        
    }
    
    // 置为0
    void identity()
    {
        heading = pitch = bank = 0;
    }
    
    // 使欧拉角处于限制集里
    // 即heading和bank在 [-pi, pi] 之间，pitch在 [-pi/2, pi/2]之间，如果pitch=+-pi/2，则bank=0
    void canonize();

    // 从四元数转换到欧拉角
    // 输入为物体坐标系-惯性坐标系旋转的四元数
    void  fromObjectToInertialQuaternion(const Quaternion &q);
    // 输入为惯性坐标系-物体坐标系旋转的四元数
    void fromInertialToObjectQuaternion(const Quaternion &q);
    
    // 从矩阵的旋转部分的方位转换到欧拉角(省略平移部分，并假设矩阵是正交的)
    // 输入为物体坐标系-世界坐标系的矩阵
    void fromObjectToWorldMatrix(const RotationMatrix &m);
    // 输入为世界坐标系-物体坐标系的矩阵
    void fromWorldToObjectMatrix(const RotationMatrix &m);
    
    // 从旋转矩阵转换到欧拉角
    void fromRotationMatrix(const RotationMatrix &m);
    
public:
    float heading;
    float pitch;
    float bank;
};

// 全局的单位欧拉角
extern const EulerAngles kEulerAnglesIdentity;

#endif /* EulerAngles_hpp */
