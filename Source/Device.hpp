//
//  Device.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Device_hpp
#define Device_hpp

#include <stdio.h>
#include "Matrix4x3.hpp"

class Matrix4x3;

enum RenderState
{
	RENDER_STATE_WIREFRAME,		// 线框
	RENDER_STATE_TEXTURE,			// 纹理
	RENDER_STATE_COLOR,			// 颜色
};


// 坐标转换噿
class Transform
{
public:
	const Matrix4x3 &getTransformMatrix()
	{
		return m_transform;
	}

	void init(int nWidth, int nHeight);

	void updateTransform();

protected:
	Matrix4x3 m_world;		// 世界变换
	Matrix4x3 m_view;		// 相机变化
	Matrix4x3 m_projection;	// 投影变换

	Matrix4x3 m_transform;	// transform = world*view*projection

	float m_fWidth;				// 屏幕宽度
	float m_fHeight;				// 屏幕高度


};

class Device
{
	Transform m_transform;	// 坐标转换噿
	int m_nWidth;					// 窗口宽度
	int m_nHeight;					// 窗口高度
	int m_nTexWidth;				// 纹理宽度
	int m_nTexHeight;			// 纹理高度
	float m_fMaxU;					// 纹理最大宽度（标准化到1＿
	float m_fMaxV;					// 纹理最大高度（标准化到1＿
	RenderState m_eRenderState;	// 渲染状怿
	int m_nBackground;			// 背景颜色
	int m_nForeground;			// 线框颜色
};


#endif /* Device_hpp */