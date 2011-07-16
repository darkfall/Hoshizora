#ifndef RF_VECTOR3_H_
#define RF_VECTOR3_H_

#include <cmath>
#include <cassert>
#include "SoraMath.h"

namespace sora {

	class vector3 {
	public:
		static const vector3 ZERO;
		
		vector3(): x(0.f), y(0.f), z(0.f) {}
		vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
		vector3(const vector3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
		
		void set(float _x, float _y, float _z) {
			x = _x; y = _y; z = _z;
		}

		vector3& operator=(const vector3& rhs)  {
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		vector3 operator-(const vector3& rhs) const {
			return vector3(x-rhs.x, y-rhs.y, z-rhs.z);
		}
		vector3 operator+(const vector3& rhs) const {
			return vector3(x+rhs.x, y+rhs.y, z+rhs.z);
		}
		vector3 operator*(float f) const {
			return vector3(x*f, y*f, z*f);
		}
		vector3 operator/(float f) const {
			assert(f != 0);
			return vector3(x/f, y/f, z/f);
		}
		vector3& operator+=(const vector3& rhs) {
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}
		vector3& operator-=(const vector3& rhs) {
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}
	
		float dot(const vector3& rhs) const {
			return x*rhs.x+y*rhs.y+z*rhs.z;
		}
		vector3 cross(const vector3& rhs) const {
			return vector3(-z*rhs.y+y*rhs.z, z*rhs.x-x*rhs.z, -y*rhs.x+x*rhs.y);
		}
	
		float length() const {
			return sqrt(x*x+y*y+z*z);
		}
	
		float sqrLength() const {
			return x*x+y*y+z*z;
		}
	
		vector3 normalize() const {
			float inv = 1/length();
			return vector3(x*inv, y*inv, z*inv);
		}
	
		vector3 negate() const {
			return vector3(-x, -y, -z);
		}

		float x, y, z;
	};

} // namespace rfMath

#endif