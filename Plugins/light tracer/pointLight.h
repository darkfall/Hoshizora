/*
 *  pointLight.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_POINT_LIGHT_H_
#define RF_POINT_LIGHT_H_

#include "light.h"
#include "lightSample.h"
#include "ray.h"

namespace rfMath {
	
	class pointLight: public light {
	public:
		pointLight(const color& _intensity, const vector3& _position, bool _shadow=true):
		intensity(_intensity), position(_position), light(_shadow) {
		}
		
		lightSample sample(geometry* scene, const vector3& pos)	 {
			vector3 delta = position - pos;
			float rr = delta.sqrLength();
			float r = sqrt(rr);
			vector3 L = delta / r;
			
			if(shadow) {
				ray shadowRay(pos, L);
				intersectResult shadowResult = scene->intersect(shadowRay);
				if(shadowResult.geo && shadowResult.distance <= r) {
					return lightSample();
				}
			}
			
			float attenuation = 1.f / rr;
			return lightSample(L, intensity * attenuation);
		}
		
		vector3 position;
		color intensity;
	};
	
} // namespace rfMath


#endif