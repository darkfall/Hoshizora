
#ifndef RF_SPHERE_H_
#define RF_SPHERE_H_

#include "vector3.h"
#include "intersectResult.h"
#include "ray.h"
#include "material.h"

namespace rfMath {

	class sphere: public geometry {
	public:
		sphere(vector3 _center, float _radius): center(_center), radius(_radius) {
			sqrRadius = radius*radius;
		}
		sphere(vector3 _center, float _radius, material* _mat): center(_center), radius(_radius) {
			sqrRadius = radius*radius;
			mat = _mat;
		}
		
		intersectResult intersect(ray& ray) {
			vector3 v = ray.origin - center;
			
			float a0 = v.sqrLength() - sqrRadius;
			float DdotV = ray.direction.dot(v);
						
			if(DdotV <= 0) {
				float discr = DdotV * DdotV - a0;
				if(discr >= 0) {
					intersectResult result;
					result.geo = this;
					result.distance = -DdotV - sqrt(discr);
					result.position = ray.getPoint(result.distance);
					result.normal = (result.position - center).normalize();
					return result;
				}
			} 
			return intersectResult();
		}
		
		vector3 center;
		float radius;
		float sqrRadius;
	};	
}

#endif