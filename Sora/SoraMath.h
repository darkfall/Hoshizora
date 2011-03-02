/*
 *  SoraTypes.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SORA_TYPES_H_
#define SORA_TYPES_H_

#ifndef NULL
#define NULL 0
#endif

#include "SoraPlatform.h"

namespace sora {

	const float32 F_PI    = 3.14159265358f;
	const float32 F_PI_2  = 1.57079632679f;
	const float32 F_PI_4  = 0.78539826339f;
	const float32 F_PI_6  = 0.5233333f;
	const float32 F_PI_12 = 0.2616667f;

	const double32 D_PI	  =	3.1415926535897932384626433832795;
	const double32 D_PI_2 = 1.5707963267948966192313216916398;
	const double32 D_PI_4 = 0.7853981633974483096156608458198;
	const double32 D_PI_8 = 0.3926990816987241548078304229099;

	inline float32 DGR_RAD(float32 dgr)  {return (float32)((D_PI * dgr) / 180.0); }
	inline float32 RAD_DGR(float32 rad)  {return (float32)((180.0 * rad) / D_PI); }
	
	// to do
	typedef struct {
		float32 x, y;
	} Vertex2;

	typedef struct {
		float32 x, y, z;
	} Vertex3;
	
	template<typename T>
	void soraswap(T& t1, T& t2) {
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	
	template<typename T>
	inline float32 getDistanceSqr(T x, T y, T x1, T y1) {
		return (x1-x)*(x1-x)+(y1-y)*(y1-y);
	}
} // namespace sora

#endif