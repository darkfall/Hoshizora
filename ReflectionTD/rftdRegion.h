/*
 *  rftdRegion.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_REGION_H_
#define RFTD_REGION_H_

#include "hgerect.h"
#include "hgevector.h"

#include "AutoContainer.h"

namespace rftd {
	
	// all regions belongs to a list
	class rftdRegion: public sora::AutoListElement {
	public:	
		virtual bool testPoint(float32 x, float32 y) = 0;
	};
	
	class rftdSquareRegion: public rftdRegion {
	public:
		rftdSquareRegion(float32 x, float32 y, float32 width, float32 height) {
			rect.Set(x, y, width, height);
		}
		
		void set(float32 x, float32 y, float32 width, float32 height) {
			rect.Set(x, y, width, height);
		}
		
		bool testPoint(float32 x, float32 y) {
			return rect.TestPoint(x, y);
		};
		
	private;
		hgeRect rect;
	};
	
	class rftdCircleRegion: public rftdRegion {
	public:
		rftdCircleRegion(float32 _x, float32 _y, float32 _r):
			x(_x), y(_y), r(_r) {}
		
		void set(float32 _x, float32 _y, float32 _r) {
			x = _x;
			y = _y;
			r = _r;
			r2 = r*r;
		}
		
		bool testPoint(float32 _x, float32 _y) {
			float32 dist = (_x-x)*(_x-x) + (_y-y)*(_y-y);
			return dist<r2;
		}
		
	private:
		float32 x, y, r;
		float32 r2;
	}
	
}

#endif // RFTD_REGION_H_