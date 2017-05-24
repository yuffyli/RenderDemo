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

// 渲染列表变换标记
#define RENDER_TRANSFORM_LOCAL 1
#define RENDER_TRANSFORM_TRANS 2
#define RENDER_TRANSFORM_LOCALTOTRANS 3

#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256

#define COLOR0	0xffffff
#define COLOR1	0xccffff
#define COLOR2	0x99ccff
#define COLOR3	0xff9966

#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// 渲染方式
#define RENDER_STATE_WIREFRAME 0x0001
#define RENDER_STATE_TEXTURE 0x0002
#define RENDER_STATE_COLOR 0x0004

#define RENDER_STATE RENDER_STATE_WIREFRAME


#endif /* Macro_hpp */
