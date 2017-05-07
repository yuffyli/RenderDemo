//
//  Plane.hpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/3/11.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include "Vector3.hpp"

struct Plane
{
public:
	void set(const Vector3 &p0, const Vector3 &n0, bool bNormalize)
	{
		p = p0;
		n = n0;
		if (bNormalize)
		{
			n.normalize();
		}
	}

public:
	Vector3 p;		// 平面上的一点
	Vector3 n;		// 平面的法向量
};



#endif /* Plane_hpp */
