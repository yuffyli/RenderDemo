//
//  main.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//
// #include "StdAfx.h"

#define WIN32_LEAN_AND_MEAN 

#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <math.h>
#include "Macro.hpp"
#include "Object.hpp"
#include "Render.hpp"
#include "Texture.hpp"
#include <atltypes.h>
#include <winuser.h>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


//////////////////////////////////////////////////////////////////////////
HWND mainWindowHandle = NULL; // 用于保存窗口句柄的全局变量
HINSTANCE mainInstance = NULL;		// 用于保存实例的全局变量
HDC mainWindowDC = NULL;
HBITMAP mainBitmap = NULL;		// DIB
HBITMAP oldBitmap = NULL;		// 老的 BITMAP

Object mainObj;
Camera mainCam;
Render mainRender;
Texture mainTexture;

int32_t nWindowWidth;
int32_t nWindowHeight;
unsigned char *pFrameBuffer = NULL;		// frame buffer

// 物体
float fScale = 1.0f;
float fTheta = 45.0f;
Point3 posObject = Point3(0.0f, .0f, 0.0f);
Vector3 rotAxis(.0f,1.0f, .0f);

// 相加
Point3 posCam(0.0f, .0f, -1.0f);
Vector3 vecDir(0, 0, 0);
Point3 posTarget(.0f, .0f, .0f);
Vector3 vecUp(.0f, 1.0f, .0f);
float fFov = 90.0f;
float fNear = 1.0f;
float fFar = 500.0f;

//////////////////////////////////////////////////////////////////////////
void Reset()
{
	fScale = 1.0f;
	fTheta = .0f;
	posObject = Point3(0.0f, .0f, .0f);
	rotAxis = Vector3(.0f,1.0f, .0f);

	posCam = Vector3(0.0f, .0f, -5.0f);
	posTarget = Vector3(.0f, .0f, .0f);
	vecUp = Vector3(.0f, 1.0f, .0f);

	fFov = 90.0f;
	fNear = 1.0f;
	fFar = 500.0f;
}


//////////////////////////////////////////////////////////////////////////
void ScreenInit(int32_t w, int32_t h)
{
	nWindowWidth = w;
	nWindowHeight = h;
	HDC hDC = GetDC(mainWindowHandle);
	// 创建一个与指定设备兼容的内存设备上下文环境(device context)
	mainWindowDC = CreateCompatibleDC(hDC); 
	// 释放设备上下文环境（DC）供其他应用程序使用
	ReleaseDC(mainWindowHandle, hDC); 

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, 
		w * h * 4, 0, 0, 0, 0 }  };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;

	mainBitmap = CreateDIBSection(mainWindowDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	oldBitmap = (HBITMAP)SelectObject(mainWindowDC, mainBitmap);


	pFrameBuffer = (unsigned char*)ptr;
	memset(pFrameBuffer, 0, 4*w*h);
}

void DrawInfo(HDC hDC)
{
	PAINTSTRUCT ps ;
	RECT        rect ;
	DRAWTEXTPARAMS  drawTextParams;
	drawTextParams.cbSize = sizeof(DRAWTEXTPARAMS);
	drawTextParams.iLeftMargin = 1;

	SetRect(&rect, 10,10,700,200);
	SetTextColor(hDC, RGB(0,0,255));
	//TEXT("How Soft Works, 0 1 2 9 ")
	//str.Format("%s%d", str, 123);
	char info[2048];
	sprintf(info, "object pos: (%3.1f, %3.1f, %3.1f)\nscale:%3.1f\nrotate Axis: (%3.1f, %3.1f, %3.1f)\nroatation: %3.1f\ncamera pos: (%3.1f, %3.1f, %3.1f)\ntarget: (%3.1f, %3.1f, %3.1f)\nzTest: %d", 
		mainObj.posWorld.x, mainObj.posWorld.y, mainObj.posWorld.z,
		fScale,
		mainObj.rotAxis.x, mainObj.rotAxis.y, mainObj.rotAxis.z,
		fTheta,
		mainCam.m_posCamera.x, mainCam.m_posCamera.y, mainCam.m_posCamera.z,
		mainCam.m_posTarget.x, mainCam.m_posTarget.y, mainCam.m_posTarget.z,
		(mainRender.nDepthState == ENABLE_DEPTH_BUFFER)
		);
	DrawTextEx(hDC, info, -1,&rect, DT_NOCLIP, &drawTextParams);
}


void ScreenUpdate()
{
	HDC hDC = GetDC(mainWindowHandle);

	BitBlt(hDC, 0, 0, nWindowWidth, nWindowHeight, mainWindowDC, 0, 0, SRCCOPY);

	DrawInfo(hDC);

	ReleaseDC(mainWindowHandle, hDC);
}

void GameInit(int32_t w, int32_t h)
{
	Reset();

	// 设置相机
	mainCam.init(CAMERA_TYPE_UVN, posCam, vecDir, posTarget, vecUp, fFov, fNear, fFar, w, h);
	mainCam.updateMatrix();

	mainTexture.initByPng(RES_PNG_FILE);

	// 生成物体
	mainObj.init(RES_OBJECT_FILE, &mainTexture);

	mainObj.setupWorldPos(posObject);

	// 初始化渲染器
	mainRender.init(w, h, RENDER_STATE, DEPTH_BUFFER_STATE, pFrameBuffer);
}

void GameMain()
{
	if (KEY_DOWN('0'))
	{
		Reset();
	}

	if (KEY_DOWN('1'))
	{
		rotAxis = Vector3(.0f, 1.0f, .0f);
	}

	if (KEY_DOWN('2'))
	{
		rotAxis = Vector3(1.0f, 1.0f, 1.0f);
	}

	// 深度测试设置
	if (KEY_DOWN('O'))
	{
		mainRender.setZTestState(ENABLE_DEPTH_BUFFER);
	}
	if (KEY_DOWN('L'))
	{
		mainRender.setZTestState(DISABLE_DEPTH_BUFFER);
	}

	// 改变物体位置
	if (KEY_DOWN('A'))
	{
		posObject.x -= 0.1f;
	}
	if (KEY_DOWN('D'))
	{
		posObject.x += 0.1f;
	}

	if (KEY_DOWN('W'))
	{
		posObject.y += 0.1f;
	}
	if (KEY_DOWN('S'))
	{
		posObject.y -= 0.1f;
	}

	if (KEY_DOWN('F'))
	{
		posObject.z += 0.1f;
	}
	if (KEY_DOWN('B'))
	{
		posObject.z -= 0.1f;
	}

	// 旋转物体
	if (KEY_DOWN(VK_LEFT))
	{
		fTheta -= 5.0f;
	}
	if (KEY_DOWN(VK_RIGHT))
	{
		fTheta += 5.0f;
	}

	if (fTheta > 360.0f)
	{
		fTheta -= 360.0f*((int32_t)fTheta/360.0f);
	}

	// 缩放物体
	if (KEY_DOWN(VK_PRIOR))
	{
		fScale -= 0.01f;
	}

	if (KEY_DOWN(VK_NEXT))
	{
		fScale += 0.01f;
	}


	// 更新相机
	if (KEY_DOWN(VK_DOWN))
	{
		posCam.z -= 0.1f;
	}
	if (KEY_DOWN(VK_UP))
	{
		posCam.z += 0.1f;
	}

	if (posCam.z - .0f >= .0f && posCam.z - .0f <= 0.01f)
	{
		posCam.z = 0.1f;
	}
	else if (.0f - posCam.z >= .0f && .0f - posCam.z <= 0.01f)
	{
		posCam.z = -0.1f;
	}


	//////////////////////////////////////////////////////////////////////////
	mainObj.setupWorldPos(posObject);
	mainObj.setupScale(fScale);
	rotAxis.normalize();
	mainObj.setupRotate(rotAxis, degreeToRadian(fTheta));

	//////////////////////////////////////////////////////////////////////////
	mainCam.setUpPosistion(posCam);
	mainCam.setUpTarget(posTarget);

	mainCam.updateMatrix();

	/************************************************************************/
	/* 各种变换                                                                     */
	/************************************************************************/
	mainObj.resetStates();

	// 世界变换
	mainObj.worldTransform();

	// 背面消除
	mainObj.removeBackfaces(mainCam);

	// 相机变换
	mainObj.cameraTransform(mainCam);

	// 物体剔除
	//mainObj.cullObject(mainCam, CULL_OBJECT_XYZ_PLANE);

	// 投影变换
	mainObj.projectTransform(mainCam);

	// 裁剪
	//mainObj.clipping();
	mainObj.clipPolys(mainCam, CLIP_POLY_PLANE);

	// 透视除法
	mainObj.perspectiveDivision();

	// 屏幕变换
	mainObj.screenTranform(mainCam);

	// 执行渲染
	mainRender.drawObject(&mainObj);

	//mainRender.drawTexture(&mainTexture);
}

void GameShutdown()
{

}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{
	case WM_CREATE:
		{
			return 0;
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);

			return 0;
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);

			return 0;
		}
		break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon	= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = MY_WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass))
	{
		return 0;
	}

	if (!(hwnd = CreateWindowEx(
		NULL, 
		MY_WINDOW_CLASS_NAME,
		MY_WINDOW_CLASS_TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL)))
	{
		return 0;
	}

	mainWindowHandle = hwnd;
	mainInstance = hInstance;

	ScreenInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	GameInit(SCREEN_WIDTH, SCREEN_HEIGHT);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		GameMain();
		ScreenUpdate();
		//Sleep(1);
	}

	GameShutdown();

	return (msg.wParam);
}
