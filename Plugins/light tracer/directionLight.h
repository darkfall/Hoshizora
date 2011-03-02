/*
 *  directionLight.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef	RF_DIRECTION_LIGHT_H_
#define RF_DIRECTION_LIGHT_H_

#include "light.h"
#include "lightSample.h"
#include "ray.h"

namespace rfMath {
	
	class geometry;
	
	class directionLight: public light {
	public:
		directionLight(const color& _irradiance, const vector3& _direction, bool _shadow=true):
		irradiance(_irradiance), direction(_direction), light(_shadow) {
			L = direction.normalize().negate();
		}
		
		lightSample sample(geometry* scene, const vector3& pos) {
			if(shadow) {
				ray shadowRay(pos, L);
				intersectResult shadowResult = scene->intersect(shadowRay);
				if(shadowResult.geo != NULL) {
					return lightSample();
				}
			}
			
			return lightSample(L, irradiance);
		}
		
		vector3 L;
		vector3 direction;
		color irradiance;
	};
	
	
} // namespace rfMath


#endif