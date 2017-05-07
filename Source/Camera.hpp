//
//  Camera.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <math.h>
#include "Vector3.hpp"
#include "Plane.hpp"
#include "Matrix4x3.hpp"

enum CameraType
{
	CAMERA_EULER = 0,	// 欧拉相机
	CAMERA_UVN,			// UVN相机
};

class Camera
{
public:
	void init(CameraType ty, const Vector3 &pos, const Vector3 &dir, const Vector3 &target, const Vector3 &v, float fov, float nearZ, float farZ, float width, float height);
    
    void updateMatrix();
    
public:
	CameraType m_camTy; // 相机类型
	Vector3 m_posCamera; // 相机的世界坐标
	Vector3 m_direction; // 默认朝向
	Vector3 m_U, m_V, m_N; // UVN向量
	Vector3 m_posTarget; // 目标点的坐标
	float m_fFOV; // 视场角
	float m_fViewDistance; // 视距
	float m_fNearZ;	 // 近裁剪距离
	float m_fFarZ; // 远裁剪距离
	Plane m_planeClipTop, m_planeClipBottom, m_planeClipLeft, m_planeClipRight; // 上下左右裁剪面
	float m_fViewPlaneWidth, m_fViewPlaneHeight;	// 视平面宽高
	float m_fScreenWidth, m_fScreenHeight; // 屏幕宽高
	float m_fScreenCenterX, m_fScreenCenterY; // 屏幕中心坐标
	float m_fAspectRatio; // 宽高比
	Matrix4x3 m_matrixCamera; // 相机变换
	Matrix4x3 m_matrixProjection; // 投影变换
	Matrix4x3 m_matrixScreen; // 屏幕变换
};


#endif /* Camera_hpp */
