/*
 *  spotLight.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_SPOT_LIGHT_H_
#define RF_SPOT_LIGHT_H_

#include "light.h"
#include "vector3.h"
#include "lightSample.h"
#include <cmath>

namespace rfMath {
		
	class spotLight: public light {
	public:
		spotLight(const color& _intensity, const vector3& _position, const vector3& _direction, float _theta, float _phi, float _falloff, bool shadow=true):
		intensity(_intensity), position(_position), direction(_direction), theta(_theta), phi(_phi), falloff(_falloff), light(shadow) {
			S = direction.normalize().negate();
			cosTheta = cos(theta * D_PI / 360);
			cosPhi = cos(phi * D_PI / 360);
			baseMultiplier = 1.f / (cosTheta - cosPhi);
		}
		
		lightSample sample(geometry* scene, const vector3& pos) {
			vector3 delta = position - pos;
			float rr = delta.sqrLength();
			float r = sqrt(rr);
			vector3 L = delta / r;
			
			float spot;
			float SdotL = S.dot(L);
			if(SdotL >= cosTheta)
				spot = 1.f;
			else if(SdotL <= cosPhi)
				spot = 0.f;
			else 
				spot = pow((SdotL - cosPhi) * baseMultiplier, falloff);
		
			if(shadow) {
				
			}
			
			if(shadow) {
				ray shadowRay(pos, L);
				intersectResult shadowResult = scene->intersect(shadowRay);
				if(shadowResult.geo && shadowResult.distance <= r) {
					return lightSample();
				}
			}
			
			float attenuation = 1.f / rr;
			return lightSample(L, intensity * (attenuation * spot));

		}
		
		vector3 S;
		vector3 position;
		vector3 direction;
		color intensity;
		float theta, phi, falloff;
		float cosTheta, cosPhi;
		float baseMultiplier;
	};
	
} // rfMath


#endif