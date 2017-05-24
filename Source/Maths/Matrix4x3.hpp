//
//  Matrix4x3.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/18.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Matrix4x3_hpp
#define Matrix4x3_hpp

#include <stdio.h>

// 一般仿射变换矩阵，支持旋转，缩放，镜像，投影和平移变换

class Vector3;
class EulerAngles;
class Quaternion;
class RotationMatrix;

class Matrix4x3
{
public:
	Matrix4x3();
    // 置为单位矩阵
    void identity();
    
    // 平移部分的相关操作
    void zeroTranslation();    
    void setTranslation(const Vector3 &d);
    void setupTranslation(const Vector3 &d);
    
    // 根据UVN向量构造变换矩阵
    void setUVN(const Vector3 &u, const Vector3 &v, const Vector3 &n);
    void setupUVN(const Vector3 &u, const Vector3 &v, const Vector3 &n);
    
    //  根据视距d构造投影矩阵
    void setupProjection(float d);
    
    // 根据屏幕宽高构造屏幕变换矩阵
    void setupScreen(float w, float h);
    
    // 构造父坐标与局部空间之间的变换矩阵
    // 物体空间到世界空间的变换，首先从 物体空间 -> 惯性空间 -> 世界空间，方位由欧拉角或旋转矩阵指定
    void setupLocalToParent(const Vector3 &pos, const EulerAngles &orient);
    void setupLocalToParent(const Vector3 &pos, const RotationMatrix &orient);
    // 世界空间到物体空间的变换，首先从 世界空间 -> 惯性空间 -> 物体空间，方位由欧拉角或旋转矩阵指定
    void setupParentToLocal(const Vector3 &pos, const EulerAngles &orient);
    void setupParentToLocal(const Vector3 &pos, const RotationMatrix &orient);
    
    // 构造绕坐标轴旋转的矩阵
    void setupRotate(int axis, float theta);
    // 构造绕任意轴旋转的矩阵
    void setupRotate(const Vector3 &axis, float theta);
    
    // 通过四元数给出的角位移构造旋转矩阵
    void fromQuternion(const Quaternion &q);
    
    // 构造沿坐标轴缩放的矩阵
    void setupScale(const Vector3 &s);
    // 构造沿任意轴缩放的矩阵
    void setupScaleAlongAxis(const Vector3 &axis, float k);
    
    // 构造切变矩阵
    void setupShear(int axis, float s, float t);
    
    // 构造投影矩阵，投影平面经过原点
    void setupProject(const Vector3 &n);
    
    // 构造反射矩阵
    void setupReflect(int axis, float k = .0f);
    // 构造沿任意平面反射的矩阵
    void setupReflect(const Vector3 &n);

	// 设置摄像机
	void setupLookAt(const Vector3 &eye, const Vector3 &at, const Vector3 &up);
    
public:
    /* 
        这里使用行向量， 所以乘法形式为：
                    | m11 m12 m13 |
        [ x y z ]  | m21 m22 m23 | = [ x' y' z' ]
                    | m31 m32 m33 |
                    | tx    ty    tz    |
     
        为了求矩阵的逆，假设有第4列，[0 0 0 1]
        | m11 m12 m13 0 |
        | m21 m22 m23 0 |
        | m31 m32 m33 0 |
        | tx    ty    tz    1 |
        
        乘法形式变为：
                        | m11 m12 m13 0 |
        [ x y z 1 ]   | m21 m22 m23 0 | = [ x' y' z' 1]
                        | m31 m32 m33 0 |
                        | tx    ty    tz    1 |
     */
    
    // 前三行包含线性变换，最后一行包含平移
    float m11, m12, m13;
    float m21, m22, m23;
    float m31, m32, m33;
    float tx, ty, tz;
};

// 向量与矩阵相乘
Vector3 operator * (const Vector3 &p, const Matrix4x3 &m);
Vector3 &operator *= (Vector3 &p, const Matrix4x3 &m);

// 矩阵与矩阵相乘
Matrix4x3 operator * (const Matrix4x3 &a, const Matrix4x3 &b);
Matrix4x3 &operator *=(Matrix4x3 &a, const Matrix4x3 &b);

// 计算3x3部分的行列式
float determinant(const Matrix4x3 &m);

// 计算矩阵的逆
Matrix4x3 inverse(const Matrix4x3 &m);

// 提取矩阵的平移部分
Vector3 getTranslation(const Matrix4x3 &m);

// 获取从局部矩阵 -> 父矩阵的位置
Vector3 getPositionFromParentToLocalMatrix(const Matrix4x3 &m);
// 获取从父矩阵 -> 局部矩阵的位置
Vector3 getPositionFromLocalToParentMatrix(const Matrix4x3 &m);

#endif /* Matrix4x3_hpp */
