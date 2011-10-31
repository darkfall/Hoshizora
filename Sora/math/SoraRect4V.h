#ifndef RECT_4V_H
#define RECT_4V_H

#include "SoraVector.h"
#include "SoraRect.h"

namespace sora {

	struct SORA_API SoraRect4v {
		real x1, y1;
		real x2, y2;
		real x3, y3;
		real x4, y4;

		bool bEmpty;

		SoraRect4v(): x1(0.f), y1(0.f), x2(0.f), y2(0.f), x3(0.f), y3(0.f), x4(0.f), y4(0.f), bEmpty(true) {}
		SoraRect4v(real _x1, real _y1, real _x2, real _y2, real _x3, real _y3, real _x4, real _y4): bEmpty(false), x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3), x4(_x4), y4(_y4) {}
		SoraRect4v(const SoraRect4v& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x2; y2 = rect.y2;
			x3 = rect.x3; y3 = rect.y3;
			x4 = rect.x4; y4 = rect.y4;
		}

		void operator=(const SoraRect& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x1+rect.x2; y2 = rect.y1;
			x3 = rect.x1+rect.x2; y3 = rect.y1+rect.y2;
			x4 = rect.x1; y4 = rect.y1+rect.y2;
			bEmpty = false;
		}
		void operator=(const SoraRect4v& rect) {
			x1 = rect.x1; y1 = rect.y1;
			x2 = rect.x2; y2 = rect.y2;
			x3 = rect.x3; y3 = rect.y3;
			x4 = rect.x4; y4 = rect.y4;
		}

		void set(real _x1, real _y1, real _x2, real _y2, real _x3, real _y3, real _x4, real _y4) {
			x1 = _x1; y1 = _y1;
			x2 = _x2; y2 = _y2;
			x3 = _x3; y3 = _y3;
			x4 = _x4; y4 = _y4;
		}
	};

	static SoraRect4v Rect4VByRect(const SoraRect& rect, real rot=0.f) {
		if(rot != 0.f) {
			real cx = rect.x1 + rect.x2/2;
			real cy = rect.y1 + rect.y2/2;

			SoraRect4v vRect;
			
			SoraVector vec(rect.x2/2, rect.y2/2);
			vec.rotate(rot);
			vRect.x1 = cx + vec.x; vRect.y1 = cy + vec.y;
			vec.x = -rect.x2/2; vec.y = rect.y2/2;
			vec.rotate(rot);
			vRect.x2 = cx + vec.x; vRect.y2 = cy + vec.y;
			vec.x = -rect.x2/2; vec.y = -rect.y2/2;
			vec.rotate(rot);
			vRect.x3 = cx + vec.x; vRect.y3 = cy + vec.y;
			vec.x = rect.x2/2; vec.y = -rect.y2/2;
			vec.rotate(rot);
			vRect.x4 = cx + vec.x; vRect.y4 = cy + vec.y;
	
			return vRect;
		} else {
			return SoraRect4v(rect.x1, rect.y1, rect.x1+rect.x2, rect.y1+rect.y2, rect.x1, rect.y1+rect.y2, rect.x1+rect.x2, rect.y2);
		}
	}
} // namespace sora

#endif