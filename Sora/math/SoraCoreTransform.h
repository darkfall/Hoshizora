/*
 *  SoraCoreTransform.cpp
 *  loliEngine Rewrite
 *
 *  Created by Robert Bu on 7/19/10.
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
	class SORA_API SoraCoreTransform {
	public:
		SoraCoreTransform(float _f);
		SoraCoreTransform(float _f1, float _f2);
		SoraCoreTransform(float _f1, float _f2, float _f3);
		SoraCoreTransform(float _f1, float _f2, float _f3, float _f4);
	
		SoraCoreTransform();
			
		void Set(float _f);
		void Set(float _f1, float _f2);
		void Set(float _f1, float _f2, float _f3);
		void Set(float _f1, float _f2, float _f3, float _f4);
	
		int GetKind() const { return iKind; }
	
		float Get1st() const;
		float Get2nd() const;
		float Get3rd() const;
		float Get4th() const;
	
		SoraCoreTransform GetDist(const SoraCoreTransform& t2, int divider);
	
		SoraCoreTransform operator / (float divider) const;
		SoraCoreTransform operator * (float m) const;
        SoraCoreTransform operator * (const SoraCoreTransform& rhs) const;
		SoraCoreTransform operator - (const SoraCoreTransform& rhs) const;
		SoraCoreTransform operator + (const SoraCoreTransform& rhs) const;
		void operator = (const SoraCoreTransform& rhs);
	
		SoraCoreTransform& operator += (const SoraCoreTransform& rhs);
		SoraCoreTransform& operator -= (const SoraCoreTransform& rhs);
		
		bool operator == (const SoraCoreTransform& rhs) const;
			
	private:
		float f1, f2, f3, f4;
		bool bClean;	
		int iKind;
	};
	
} // namespace sora

#endif