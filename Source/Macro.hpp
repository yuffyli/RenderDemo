//
//  Macro.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Macro_hpp
#define Macro_hpp

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef signed char			char_t;
typedef unsigned char		uchar_t;
typedef signed short			short_t;
typedef unsigned short      ushort_t;
typedef signed int			int_t;
typedef unsigned int        uint_t;
typedef signed long			long_t;
typedef unsigned long       ulong_t;

typedef signed char			int8_t;
typedef unsigned char		uint8_t;
typedef signed short		int16_t;
typedef unsigned short		uint16_t;
typedef signed int			int32_t;
typedef unsigned int		uint32_t;
typedef unsigned char    byte;


// 窗口相关定义
#define MY_WINDOW_CLASS_NAME "RenderDemoWin"
#define MY_WINDOW_CLASS_TITLE	"My RenderDemo"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// 相机类型
#define CAMERA_TYPE_ELUER 1
#define CAMERA_TYPE_UVN 2

// 变换方式
#define TRANSFORM_METHOD_MANUAL 1
#define TRANSFORM_METHOD_MATRIX 2

// 物体最大顶点与面数
#define OBJECT_MAX_VERTICES 256
#define OBJECT_MAX_POLYS 256

// 渲染列表最大面数
#define RENDERLIST_MAX_POLYS 1024

// 多边形属性
#define POLY_ATTR_2SIDED 0x0001

#define POLY_VERTICES_NUM 3

// 多边形状态
#define POLY_STATE_ACTIVE 0x0001
#define POLY_STATE_CLIPPED 0x0002
#define POLY_STATE_BACKFACE 0x0004

// 剔除标记
#define CULL_OBJECT_X_PLANE 0x0001  // 根据左右裁剪面进行剔除
#define CULL_OBJECT_Y_PLANE 0x0002  // 根据上下裁剪面进行剔除
#define CULL_OBJECT_Z_PLANE 0x0004  // 根据远近裁剪面进行剔除
#define CULL_OBJECT_XYZ_PLANE (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE)

// 物体状态
#define OBJECT_STATE_ACTIVE 0x0001
#define OBJECT_STATE_VISIBLE 0x0002
#define OBJECT_STATE_CULLED 0x0004

// 相机跟随
#define CAMERA_FOLLOW_NONE 0x0001
#define CAMERA_FOLLOW_OBJECT 0x0002
#define CAMERA_FOLLOW_STATE CAMERA_FOLLOW_NONE


// 渲染列表变换标记
#define RENDER_TRANSFORM_LOCAL 1
#define RENDER_TRANSFORM_TRANS 2
#define RENDER_TRANSFORM_LOCALTOTRANS 3



#define COLOR0	0x11ffff
#define COLOR1	0xccffff
#define COLOR2	0x00ffff
#define COLOR3	0xff9966

#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// 渲染方式
#define RENDER_STATE_WIREFRAME 0x0001
#define RENDER_STATE_TEXTURE 0x0002
#define RENDER_STATE_COLOR 0x0004
#define RENDER_STATE	 RENDER_STATE_TEXTURE /*RENDER_STATE_WIREFRAME*/

// 背面剔除
#define CULLING_NONE 0x0001
#define CULLING_BACK_FACE 0x0002
#define CULLING_FRONT_FACE 0x0004
#define CULLING_STATE CULLING_BACK_FACE


// 裁剪
#define AREA_LEFT 1			// 0001
#define AREA_RIGHT 2		// 0010
#define AREA_BOTTOM 4	// 0100
#define AREA_TOP 8			// 1000

#define CLIP_POLY_X_PLANE	0x0001
#define CLIP_POLY_Y_PLANE		0x0002
#define CLIP_POLY_Z_PLANE	0x0004
#define CLIP_POLY_PLANE CLIP_POLY_X_PLANE|CLIP_POLY_Y_PLANE|CLIP_POLY_Z_PLANE

#define CLIP_CODE_GZ		0x0001
#define CLIP_CODE_LZ		0x0002
#define CLIP_CODE_IZ		0x0004
#define CLIP_CODE_GX		0x0001
#define CLIP_CODE_LX		0x0002
#define CLIP_CODE_IX		0x0004
#define CLIP_CODE_GY		0x0001
#define CLIP_CODE_LY		0x0002
#define CLIP_CODE_IY		0x0004


// 深度测试
#define ENABLE_DEPTH_BUFFER 0x0001
#define DISABLE_DEPTH_BUFFER 0x0002
#define DEPTH_BUFFER_STATE ENABLE_DEPTH_BUFFER

#define RES_OBJECT_CUBE_TRI_FILE "Resources\\cube2.plg"
#define RES_OBJECT_POKER_TRI_FILE "Resources\\poker.plg"
#define RES_OBJECT_FILE RES_OBJECT_CUBE_TRI_FILE


#define RES_PNG_CUBE_FILE		"Resources\\images.png"
#define RES_PNG_POKER_FILE	"Resources\\poker.png"
#define RES_PNG_FILE	RES_PNG_CUBE_FILE


#endif /* Macro_hpp */

