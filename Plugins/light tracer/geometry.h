/*
 *  geometry.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/16/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */
#ifndef RF_GEOMETRY_H_
#define RF_GEOMETRY_H_

#include "material.h"

namespace rfMath {
	
	class intersectResult;
	class ray;

class geometry {
public:
	geometry(): mat(0) {}
	virtual ~geometry() {}
	
	virtual intersectResult intersect(ray& ray) = 0;
	
	material* mat;
};

}
#endif