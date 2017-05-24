//
//  main.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//
//#include "StdAfx.h"

//#include <GLUT/GLUT.h> // GLUT窗口库
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>

// 自定义一个绘制函数
//void display()
//{
//    // 清空
//    glClear(GL_COLOR_BUFFER_BIT);
//    // 开始绘制一个多边形
//    glBegin(GL_POLYGON);
//    // 多边形的顶点（三角形）
//    glVertex2f(-0.5, -0.5);
//    glVertex2f(0, 0.5);
//    glVertex2f(0.5, -0.5);
//    //绘制结束
//    glEnd();
//    glFlush();
//}

#define WIN32_LEAN_AND_MEAN 

#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <math.h>
#include "Macro.hpp"
#include "Object.hpp"
#include "Render.hpp"

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

int32_t nWindowWidth;
int32_t nWindowHeight;
unsigned char *pFrameBuffer = NULL;		// frame buffer

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

	//AdjustWindowRect(&rect, GetWindowLong(mainWindowHandle, GWL_STYLE), 0);
	//wx = rect.right - rect.left;
	//wy = rect.bottom - rect.top;
	//sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	//sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	//if (sy < 0) sy = 0;
	//SetWindowPos(mainWindowHandle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	//SetForegroundWindow(mainWindowHandle);

	//ShowWindow(mainWindowHandle, SW_NORMAL);


	pFrameBuffer = (unsigned char*)ptr;
	memset(pFrameBuffer, 0, 4*w*h);
}


void ScreenUpdate()
{
	HDC hDC = GetDC(mainWindowHandle);
	BitBlt(hDC, 0, 0, nWindowWidth, nWindowHeight, mainWindowDC, 0, 0, SRCCOPY);
	ReleaseDC(mainWindowHandle, hDC);
}

void GameInit(int32_t w, int32_t h)
{
	// 设置相机
	Point3 posCam(3.5f, .0f, .0f);
	Vector3 vecDir(0, 0, 0);
	Point3 posTarget(.0f, .0f, .0f);
	Vector3 vecUp(.0f, .0f, 1.0f);
	float fFov = 90.0f;
	mainCam.init(CAMERA_TYPE_UVN, posCam, vecDir, posTarget, vecUp, fFov, 1.0f, 500.0f, w, h);
	mainCam.updateMatrix();

	// 生成物体
	mainObj.init();

	// 初始化渲染器
	mainRender.init(w, h, pFrameBuffer);
}

void GameMain()
{
	// 更新相机
	if (KEY_DOWN(VK_DOWN))
	{
		mainCam.m_posCamera.z -= 1.0f;
	}
	if (KEY_DOWN(VK_UP))
	{
		mainCam.m_posCamera.z += 1.0f;
	}
	mainCam.updateMatrix();


	/************************************************************************/
	/* 各种变换                                                                     */
	/************************************************************************/
	// 世界变换
	mainObj.worldTransform();

	// 旋转物体

	// 相机变换
	mainObj.cameraTransform(mainCam);

	// 物体剔除
	mainObj.cullObject(mainCam, CULL_OBJECT_XYZ_PLANE);

	// 背面消除
	mainObj.removeBackfaces(mainCam);

	// 投影变换
	mainObj.projectTransform(mainCam);

	// 屏幕变换
	mainObj.screenTranform(mainCam);

	// 执行渲染
	mainRender.drawObject(&mainObj);

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
		Sleep(1);
	}

	GameShutdown();

	return (msg.wParam);
}



// 程序入口
//int main(int argc, char * argv[])
//{
//
//    ////窗口初始化
//    //glutInit(&argc, argv);
//    ////创建一个名为Demo的窗口
//    //glutCreateWindow("Demo");
//    ////绘图函数
//    //glutDisplayFunc(display);
//    //glutMainLoop();
//
//	return 0;
//}
