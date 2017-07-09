//
//  Camera.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Camera.hpp"
#include "MathUtil.hpp"
#include "Macro.hpp"


void Camera::init(int ty, const Point3 &pos, const Vector3 &dir, const Point3 &target, const Vector3 &v, float fov, float nearZ, float farZ, float width, float height)
{
	m_camTy = ty;
	m_posCamera = pos;
	m_direction = dir;
	m_posTarget = target;
	m_V = v;
	m_fFOV = fov;
	m_fNearZ = nearZ;
	m_fFarZ = farZ;
	m_fScreenWidth = width;
	m_fScreenHeight = height;
	m_fScreenCenterX = width/2 - 1;
	m_fScreenCenterY = height/2 - 1;
	m_fAspectRatio = width/height;
	m_fViewPlaneWidth = 2.0f;
	m_fViewPlaneHeight = 2.0f/m_fAspectRatio;

	if (m_fFOV == 90.0f)
	{
		m_fViewDistance = 1.0f;
	}
	else
	{
		m_fViewDistance = .5f * m_fViewPlaneWidth * fovToZoom(degreeToRadian(m_fFOV));
	}

    // 面法向量
    Vector3 vecN;
    // 上裁剪面
    vecN.setup(.0f, m_fViewDistance, -1.0f/m_fAspectRatio);
    m_planeClipTop.set(kZeroVector, vecN, true);
    
    // 下裁剪面
    vecN.setup(.0f, -m_fViewDistance, -1.0f/m_fAspectRatio);
    m_planeClipBottom.set(kZeroVector, vecN, true);
    
    // 左裁剪面
    vecN.setup(-m_fViewDistance, .0f, -1.0f);
    m_planeClipLeft.set(kZeroVector, vecN, true);
    
    // 右裁剪面
    vecN.setup(m_fViewDistance, .0f, -1.0f);
    m_planeClipRight.set(kZeroVector, vecN, true);
}

void Camera::updateMatrix()
{
    // 根据目标位置创建旋转矩阵（逆）
    Matrix4x3 matrixUVN;
    if(m_camTy == CAMERA_TYPE_ELUER)
    {
    }
    else if (m_camTy == CAMERA_TYPE_UVN)
    {
        m_N = m_posTarget - m_posCamera;
        m_N.normalize();
        
        m_U = crossProduct(m_V, m_N);
        m_U.normalize();
        
        m_V = crossProduct(m_N, m_U);
        matrixUVN.setUVN(m_U, m_V, m_N);
    }
    
    m_matrixCamera = matrixUVN;

	// 根据相机位置设置平移向量（逆）
	Vector3 vTran(-m_posCamera*m_U, -m_posCamera*m_V, -m_posCamera*m_N);
	m_matrixCamera.setTranslation(vTran);
    
	// 设置透视投影矩阵
    m_matrixProjection.setupProjection(m_fAspectRatio, m_fFOV, m_fNearZ, m_fFarZ);
    
	// 设置屏幕变换矩阵
    m_matrixScreen.setupScreen(m_fScreenWidth, m_fScreenHeight);
}

void Camera::setUpTarget(const Point3 &t)
{
	m_posTarget = t;
}
