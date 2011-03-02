#ifndef RF_VECTOR3_H_
#define RF_VECTOR3_H_

#include <cmath>
#include <cassert>

namespace rfMath {
	
	const double D_PI   = 3.1415926535897932384626433832795;
	const double D_PI_2 = 1.5707963267948966192313216916398;
	const double D_PI_4 = 0.7853981633974483096156608458198;
	const double D_PI_8 = 0.3926990816987241548078304229099;
	
	class vector3 {
	public:
		vector3(): x(0.f), y(0.f), z(0.f) {}
		vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
		vector3(const vector3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}

		vector3& operator=(const vector3& rhs)  {
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		vector3 operator-(const vector3& rhs) {
			return vector3(x-rhs.x, y-rhs.y, z-rhs.z);
		}
		vector3 operator+(const vector3& rhs) {
			return vector3(x+rhs.x, y+rhs.y, z+rhs.z);
		}
		vector3 operator*(float f) {
			return vector3(x*f, y*f, z*f);
		}
		vector3 operator/(float f) {
			assert(f != 0);
			return vector3(x/f, y/f, z/f);
		}
	
		float dot(const vector3& rhs) {
			return x*rhs.x+y*rhs.y+z*rhs.z;
		}
		vector3 cross(const vector3& rhs) {
			return vector3(-z*rhs.y+y*rhs.z, z*rhs.x-x*rhs.z, -y*rhs.x+x*rhs.y);
		}
	
		float length() {
			return sqrt(x*x+y*y+z*z);
		}
	
		float sqrLength() {
			return x*x+y*y+z*z;
		}
	
		vector3 normalize() {
			float inv = 1/length();
			return vector3(x*inv, y*inv, z*inv);
		}
	
		vector3 negate() {
			return vector3(-x, -y, -z);
		}

		float x, y, z;
	};

} // namespace rfMath

#endif