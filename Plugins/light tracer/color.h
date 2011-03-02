/*
 *  color.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_COLOR_H_
#define RF_COLOR_H_

#include <cmath>

namespace rfMath {
	
	class color {
	public:
		color(): r(0.f), g(0.f), b(0.f), a(1.f) {}
		color(float _r, float _g, float _b, float _a=1.0f): r(_r), g(_g), b(_b), a(_a) {}
		
		color& operator=(const color& rhs) {
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			a = rhs.a;
			return *this;
		}
		
		color operator+(const color& rhs) {
			return color(r+rhs.r, g+rhs.g, b+rhs.b, a+rhs.a);
		}
		color operator*(float s) {
			return color(r*s, g*s, b*s, a);
		}
		color modulate(const color& rhs) {
			return color(r*rhs.r, g*rhs.g, b*rhs.b, a);
		}
		
		float r, g, b, a;
	};
	
	static color BLACK_COLOR(0.f, 0.f, 0.f);
	static color WHITE_COLOR(1.f, 1.f, 1.f);
	static color RED_COLOR(1.f, 0.f, 0.f);
	static color GREEN_COLOR(0.f, 1.f, 0.f);
	static color BLUE_COLOR(0.f, 0.f, 1.f);
} // namespace rfMath;

#endif