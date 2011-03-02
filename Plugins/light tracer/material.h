/*
 *  material.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_MATERIAL_H_
#define RF_MATERIAL_H_

#include "color.h"

namespace rfMath {
	
	class ray;
	class vector3;
	
	class material {
	public:
		material(float _reflectiveness=0.f): reflectiveness(_reflectiveness) {}
		virtual ~material() {}
		
		virtual color sample(const ray& r, const vector3& pos, const vector3& normal) = 0;
		
		float reflectiveness;
	};
	
} // namespace rfMath;


#endif

