#ifndef RECT_4V_H
#define RECT_4V_H

#include "hgevector.h"
#include "hgerect.h"

namespace sora {

	typedef struct tagRect4V {
		float x1, y1;
		float x2, y2;
		float x3, y3;
		float x4, y4;

		bool bEmpty;

		tagRect4V(): x1(0.f), y1(0.f), x2(0.f), y2(0.f), x3(0.f), y3(0.f), x4(0.f), y4(0.f), bEmpty(true) {}
		tagRect4V(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4): bEmpty(false), x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3), x4(_x4), y4(_y4) {}
		tagRect4V(const tagRect4V& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x2; y2 = rect.y2;
			x3 = rect.x3; y3 = rect.y3;
			x4 = rect.x4; y4 = rect.y4;
		}

		void operator=(const hgeRect& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x1+rect.x2; y2 = rect.y1;
			x3 = rect.x1+rect.x2; y3 = rect.y1+rect.y2;
			x4 = rect.x1; y4 = rect.y1+rect.y2;
			bEmpty = false;
		}
		void operator=(const tagRect4V& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x2; y2 = rect.y2;
			x3 = rect.x3; y3 = rect.y3;
			x4 = rect.x4; y4 = rect.y4;
		}

		void set(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4) {
			x1 = _x1; y1 = _y1;
			x2 = _x2; y2 = _y2;
			x3 = _x3; y3 = _y3;
			x4 = _x4; y4 = _y4;
		}
	} Rect4V;

	static Rect4V Rect4VByRect(const hgeRect& rect, float rot=0.f) {
		if(rot != 0.f) {
			float cx = rect.x1 + rect.x2/2;
			float cy = rect.y1 + rect.y2/2;

			Rect4V vRect;
			
			hgeVector vec(rect.x2/2, rect.y2/2);
			vec.Rotate(rot);
			vRect.x1 = cx + vec.x; vRect.y1 = cy + vec.y;
			vec.x = -rect.x2/2; vec.y = rect.y2/2;
			vec.Rotate(rot);
			vRect.x2 = cx + vec.x; vRect.y2 = cy + vec.y;
			vec.x = -rect.x2/2; vec.y = -rect.y2/2;
			vec.Rotate(rot);
			vRect.x3 = cx + vec.x; vRect.y3 = cy + vec.y;
			vec.x = rect.x2/2; vec.y = -rect.y2/2;
			vec.Rotate(rot);
			vRect.x4 = cx + vec.x; vRect.y4 = cy + vec.y;
	
			return vRect;
		} else {
			return Rect4V(rect.x1, rect.y1, rect.x1+rect.x2, rect.y1+rect.y2, rect.x1, rect.y1+rect.y2, rect.x1+rect.x2, rect.y2);
		}
	}
} // namespace sora

#endif