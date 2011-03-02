/*
 *  checkerMaterial.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_CHECKER_MATERIAL_H_
#define RF_CHECKER_MATERIAL_H_

#include "material.h"
#include "ray.h"
#include "vector3.h"

#include <cmath>

namespace rfMath {
	
	class checkerMaterial: public material {
	public:
		checkerMaterial(float _scale, float _reflectiveness=0.f): scale(_scale), material(_reflectiveness) {

		}
		
		color sample(const ray& r, const vector3& pos, const vector3& normal) {
			return abs((int)(floor(pos.x*scale)+floor(pos.z*scale))%2) < 1 ? BLACK_COLOR : WHITE_COLOR; 
		}
		
		float scale;
	};
		
} // namespace rfMath

#endif