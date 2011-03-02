/*
 *  phongMeterial.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_PHONG_METERIAL_H_
#define RF_PHONG_MERETIAL_H_

#include "material.h"
#include "color.h"
#include "vector3.h"

#include <cmath>
#include <iostream>

namespace rfMath {
	
	class phongMaterial: public material {
	public:
		phongMaterial(const color& _diffuse, const color& _specular, float _shininess, float _reflection=0.f):
		diffuse(_diffuse), specular(_specular), shininess(_shininess), material(_reflection) {
			lightDir = vector3(1, 1, 1).normalize();
			lightColor = WHITE_COLOR;
		}
		
		void setLightDir(const vector3& dir) {
			lightDir = dir;
		}
		void setLightColor(const color& c) {
			lightColor = c;
		}
		
		color sample(const ray& r, const vector3& pos, const vector3& normal) {
			vector3 tnormal = normal;
			float NdotL = tnormal.dot(lightDir);
			vector3 h = (lightDir-r.direction).normalize();
			float NdotN = tnormal.dot(h);
			color diffuseTerm = diffuse*std::max(NdotL, 0.f);
			color specularTerm = specular*pow(std::max(NdotN, 0.f), shininess);
			return lightColor.modulate(diffuseTerm+specularTerm);
		}
		
		color diffuse;
		color specular;
		color lightColor;
		vector3 lightDir;
		float shininess;
	};
		
} // namespace rfMath


#endif