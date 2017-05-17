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

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


//////////////////////////////////////////////////////////////////////////
HWND mainWindowHandle = NULL; // 用于保存窗口句柄的全局变量
HINSTANCE mainInstance = NULL;		// 用于保存实例的全局变量

Object mainObj;
Camera mainCam;


//////////////////////////////////////////////////////////////////////////
void GameInit()
{
	// 设置相机
	Point3 posCam(5, 0, 6);
	Vector3 vecDir(0, 0, 0);
	Point3 posTarget(0, 0, 5);
	Vector3 vecUp(0, 0, 1);
	float fFov = 90.0f;
	mainCam.init(CAMERA_TYPE_UVN, posCam, vecDir, posTarget, vecUp, fFov, 5.0f, 50.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
	mainCam.updateMatrix();
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
	mainObj.cameraTransform(mainCam);

	// 屏幕变换
	mainObj.screenTranform(mainCam);

	// 执行渲染


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

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		GameMain();
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
