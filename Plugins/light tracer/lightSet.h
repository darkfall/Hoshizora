/*
 *  lightSet.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_LIGHT_SET_H_
#define RF_LIGHT_SET_H_

#include "light.h"
#include "lightSample.h"
#include "color.h"

#include <vector>

namespace rfMath {
	
	class lightSet {
	public:
		lightSet() {}
		virtual ~lightSet() {}
		
		void add(light* l) {
			lights.push_back(l);
		}
		void del(light* l) {
			for(size_t i=0; i<lights.size(); ++i) {
				if(lights[i] == l) {
					lights.erase(lights.begin()+i);
					break;
				}
			}
		}
		
		color sample(geometry* scene, intersectResult* result) {
			color c = BLACK_COLOR;
			for(size_t i=0; i<lights.size(); ++i) {
				lightSample lsample = lights[i]->sample(scene, result->position);
				
				if(!lsample.isZero()) {
					float NdotL = result->normal.dot(lsample.L);
					
					if(NdotL >= 0.f) 
						c = c + lsample.EL * NdotL;
				}
			}
			return c;
		}
		
	private:
		std::vector<light*> lights;
	};
	
} // namespace rfMath

#endif