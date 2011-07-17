/*
 *  CoreTransform.cpp
 *  loliEngine Rewrite
 *
 *  Created by griffin clare on 7/19/10.
 *  Copyright 2010 GM&Symphonie&ring0dev. All rights reserved.
 *  Modified by Griffin Bu for Sora 14/10/10
 *
 */

#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "SoraPlatform.h"
#include "SoraException.h"

namespace sora {
	enum {
		TRANSFORM_1F = 1,
		TRANSFORM_2F = 2,
		TRANSFORM_3F = 3,
		TRANSFORM_4F = 4,
	};

	/* from loliEngine rewrite */
	class SORA_API CoreTransform {
	public:
		CoreTransform(float32 _f): f1(_f), bClean(false), iKind(TRANSFORM_1F) {}
		CoreTransform(float32 _f1, float32 _f2): f1(_f1), f2(_f2), bClean(false), iKind(TRANSFORM_2F) {}
		CoreTransform(float32 _f1, float32 _f2, float32 _f3): f1(_f1), f2(_f2), f3(_f3), bClean(false), iKind(TRANSFORM_3F) {}
		CoreTransform(float32 _f1, float32 _f2, float32 _f3, float32 _f4): f1(_f1), f2(_f2), f3(_f3), f4(_f4), bClean(false), iKind(TRANSFORM_4F) {}
	
		CoreTransform(): bClean(true), iKind(0) {}
			
		void Set(float32 _f) { f1 = _f; bClean = false; iKind = TRANSFORM_1F; }
		void Set(float32 _f1, float32 _f2) { f1 = _f1; f2 = _f2; bClean = false; iKind = TRANSFORM_2F; }
		void Set(float32 _f1, float32 _f2, float32 _f3) { f1 = _f1; f2 = _f2; f3 = _f3; bClean = false; iKind = TRANSFORM_3F; }
		void Set(float32 _f1, float32 _f2, float32 _f3, float32 _f4) { f1 = _f1; f2 = _f2; f3 = _f3; f4 = _f4; bClean = false; iKind = TRANSFORM_4F; }
	
		int GetKind() const { return iKind; }
	
		float32 Get1st() const;
		float32 Get2nd() const;
		float32 Get3rd() const;
		float32 Get4th() const;
	
		CoreTransform GetDist(const CoreTransform& t2, int divider);
	
		CoreTransform operator / (float32 divider);
		CoreTransform operator * (float32 m);
		CoreTransform operator - (const CoreTransform& rhs);
		CoreTransform operator + (const CoreTransform& rhs);
		void operator = (const CoreTransform& rhs);
	
		CoreTransform& operator += (const CoreTransform& rhs);
		CoreTransform& operator -= (const CoreTransform& rhs);
		
		bool operator == (const CoreTransform& rhs);
			
	private:
		float32 f1, f2, f3, f4;
		bool bClean;	
		int iKind;
	};
	
} // namespace sora

#endif