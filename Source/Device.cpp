//
//  Device.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//


#include <math.h>
#include "Device.hpp"
#include "Vector3.hpp"


void Transform::init(int nWidth, int nHeight)
{
	// 计算宽高比
	float fAspect = (float)nWidth/((float)nHeight);
	m_world.identity();
	m_view.identity();
	Vector3 origin(.0f, .0f, .0f);
	m_projection.setupProject(origin);
	
	m_fWidth = (float)nWidth;
	m_fHeight = (float)nHeight;

	updateTransform();
}

void Transform::updateTransform()
{
	m_transform = m_world * m_view * m_projection;
}

//void Transform::setCamerAtZero(const Vector3 &eye)
//{
//	Vector3 at = kZeroVector;
//	Vector3 up(0, 0, 1);
//	m_view.setupLookAt(eye, at, up);
//	updateTransform();
//}
