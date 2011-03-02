/*
 *  lightSample.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */


#ifndef RF_LIGHT_SAMPLE_H_
#define RF_LIGHT_SAMPLE_H_

#include "vector3.h"

namespace rfMath {
	
	class lightSample {
	public:
		lightSample(const vector3& _L, const color& _EL):
			L(_L), EL(_EL) {}
		lightSample() {}
		~lightSample() {}
		
		bool isZero() {
			return (L.length() == 0);
		}
		
		vector3 L;
		color EL;
	};
	
} // namespace rfMath


#endif