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


#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

#endif /* Macro_hpp */
