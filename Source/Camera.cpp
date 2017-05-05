//
//  Camera.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Camera.hpp"
#include "MathUtil.hpp"


Camera::init(CameraType ty, const Vector3 &pos, const Vector3 &dir, const Vector3 &target, const Vector3 &v, float fov, float nearZ, float farZ, float width, float height)
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

	if (m_fFOV == 90)
	{
		m_fViewDistance = 1.0f;
	}
	else
	{
		m_fViewDistance = .5f * m_fViewPlaneWidth * fovToZoom(degreeToRadian(m_fFOV));
	}

	Vector3 vecN(m_fViewDistance, 0, -1);
	m_planeClipRight.init(kZeroVector, vecN, true);

}
