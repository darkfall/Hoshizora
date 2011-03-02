/*
 *  light.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_LIGHT_H_
#define RF_LIGHT_H_

#include "vector3.h"

namespace rfMath {
	
	class lightSample;
	class geometry;
	
	class light {
	public:
		light(bool _shadow):shadow(true) {}
		virtual ~light() {}
		
		virtual lightSample sample(geometry* scene, const vector3& pos) = 0;
		
		bool shadow;
	};
	
} // namespace rfMath;


#endif