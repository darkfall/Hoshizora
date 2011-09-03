/*
 *  SoraTypes.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_MATH_H_
#define SORA_MATH_H_

#ifndef NULL
#define NULL 0
#endif

#include "SoraPlatform.h"
#include "SoraRect.h"
#include "SoraVector3.h"
#include "SoraCoreTransform.h"
#include "SoraQuaternion.h"

namespace sora {

	const float F_PI    = 3.14159265358f;
	const float F_PI_2  = 1.57079632679f;
	const float F_PI_4  = 0.78539826339f;
	const float F_PI_6  = 0.5233333f;
	const float F_PI_12 = 0.2616667f;

	const double D_PI	  =	3.1415926535897932384626433832795;
	const double D_PI_2 = 1.5707963267948966192313216916398;
	const double D_PI_4 = 0.7853981633974483096156608458198;
	const double D_PI_8 = 0.3926990816987241548078304229099;

	inline float DGR_RAD(float dgr)  {return (float)((D_PI * dgr) / 180.0); }
	inline float RAD_DGR(float rad)  {return (float)((180.0 * rad) / D_PI); }
    
    inline bool floatEqual(float f1, float f2) {
        return fabs(f1 - f2) < 0.0000001;
    }
	
	template<typename T>
	inline void soraswap(T& t1, T& t2) {
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	
	template<typename T>
	inline float getDistanceSqr(T x, T y, T x1, T y1) {
		return (x1-x)*(x1-x)+(y1-y)*(y1-y);
	}
	
	// compress a unit float to nBits integer
	// there would be some data loss but good for some condition
	// it is safe to cast the result to nBits integer, such as uint16 if you pass nBits as 16
	inline uint32 compressUnitFloat(float unitFloat, uint32 nBits) {
		// determine the number of intervals based on the number of output
		uint32 nInterval = 1u << nBits;
		
		// scale the input value from the range [0, 1] into the range [0, nIntervals - 1]
		// substract 1 because we want the largest output value to fit into nBits bits
		float scaled = unitFloat * (float)(nInterval - 1u);
		
		// finally round to the nearest interval center
		// by adding 0.5f, then truncating to the next-lowest interval index
		uint32 rounded = (uint32)(scaled + 0.5f);
		
		if(rounded > nInterval-1u)
			rounded = nInterval-1u;
		
		return rounded;
	}
	
	// decompress a unit float from a quantized uint
	inline float decompressUnitFloat(uint32 quantized, uint32 nBits) {
		// determine the number of intervals based on the number of bits
		uint32 nIntervals = 1u << nBits;
		
		// decode by simply converting the uint32 to an float, then scaling by the interval size
		float intervalSize = 1.f / (float)(nIntervals - 1u);
		
		float approxUnitFloat = (float)quantized * intervalSize;
		return approxUnitFloat;
	}
	
	// compress a float within a specific range
	inline uint32 compressFloat(float value, float min, float max, uint32 nBits) {
		float unitFloat = (value - min) / (max - min);
		uint32 quantized = compressUnitFloat(unitFloat, nBits);
		return quantized;
	}
	
	inline float decompressFloat(uint32 quantized, float min, float max, uint32 nBits) {
		float unitFloat = decompressUnitFloat(quantized, nBits);
		float value = min + (unitFloat * (max - min));
		return value;
	}
    
    inline uint16 flipBytes(uint16 value) {
        return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
    }
    
    inline uint32 flipBytes(uint32 value) {
        return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) |
            ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
    }
    
    inline int16 flipBytes(int16 value) {
        return int16(flipBytes(uint16(value)));
    }
    
    inline int32 flipBytes(int32 value) {
        return int32(flipBytes(uint32(value)));
    }
    
    inline float slerp(float start, float end, float r) {
        r >= 1.f ? r = 1.f: r;
        return start + (end - start) * r;
    }
    
    inline int32 slerp(int32 start, int32 end, float r) {
        r >= 1.f ? r = 1.f: r;
        return (int32)(start + (end - start) * r);
    }
    
    inline SoraVector slerp(const SoraVector& start, const SoraVector& end, float r) {
        r >= 1.f ? r = 1.f: r;
        return start + (end - start) * r;
    }
    
    inline SoraCoreTransform slerp(const SoraCoreTransform& start, const SoraCoreTransform& end, float r) {
        r >= 1.f ? r = 1.f: r;
        return start + (end - start) * r;
    }
        
    
} // namespace sora

#endif