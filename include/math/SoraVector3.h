#ifndef RF_VECTOR3_H_
#define RF_VECTOR3_H_

#include <cmath>
#include <cassert>

namespace sora {

	class SoraVector3 {
	public:
		static const SoraVector3 ZERO;
		
		SoraVector3(): x(0.f), y(0.f), z(0.f) {}
		SoraVector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
		SoraVector3(const SoraVector3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
		
		void set(float _x, float _y, float _z) {
			x = _x; y = _y; z = _z;
		}

		SoraVector3& operator=(const SoraVector3& rhs)  {
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		SoraVector3 operator-(const SoraVector3& rhs) const {
			return SoraVector3(x-rhs.x, y-rhs.y, z-rhs.z);
		}
		SoraVector3 operator+(const SoraVector3& rhs) const {
			return SoraVector3(x+rhs.x, y+rhs.y, z+rhs.z);
		}
		SoraVector3 operator*(float f) const {
			return SoraVector3(x*f, y*f, z*f);
		}
		SoraVector3 operator/(float f) const {
			assert(f != 0);
			return SoraVector3(x/f, y/f, z/f);
		}
		SoraVector3& operator+=(const SoraVector3& rhs) {
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}
		SoraVector3& operator-=(const SoraVector3& rhs) {
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}
	
		float dot(const SoraVector3& rhs) const {
			return x*rhs.x+y*rhs.y+z*rhs.z;
		}
		SoraVector3 cross(const SoraVector3& rhs) const {
			return SoraVector3(-z*rhs.y+y*rhs.z, z*rhs.x-x*rhs.z, -y*rhs.x+x*rhs.y);
		}
	
		float length() const {
			return sqrt(x*x+y*y+z*z);
		}
	
		float sqrLength() const {
			return x*x+y*y+z*z;
		}
	
		SoraVector3 normalize() const {
			float inv = 1/length();
			return SoraVector3(x*inv, y*inv, z*inv);
		}
	
		SoraVector3 negate() const {
			return SoraVector3(-x, -y, -z);
		}

		float x, y, z;
	};
    
} // namespace rfMath

#endif