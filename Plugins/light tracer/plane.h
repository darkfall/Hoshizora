/*
 *  plane.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_PLANE_H_
#define RF_PLANE_H_

#include "geometry.h"

namespace rfMath {
	
	class plane: public geometry {
	public:
		plane(const vector3& _normal, float _d):
			normal(_normal), d(_d) {
				position = normal * d;
		}
		plane(const vector3& _normal, float _d, material* _mat):
			normal(_normal), d(_d) {
				position = normal * d;
				mat = _mat;
		}
		
		plane& operator=(const plane& rhs) {
			normal = rhs.normal;
			d = rhs.d;
			return *this;
		}
		
		intersectResult intersect(ray& r) {
			float a = r.direction.dot(normal);
			if(a >= 0)
				return intersectResult();
			
			float b = normal.dot(r.origin - position);
			intersectResult result;
			result.geo = this;
			result.distance = -b/a;
			result.position = r.getPoint(result.distance);
			result.normal = normal;
			return result;
		}
		
		vector3 normal;
		vector3 position;
		float	d;
			
	};
	
} // namespace rfMath


#endif