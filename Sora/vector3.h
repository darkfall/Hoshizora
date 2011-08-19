#ifndef RF_VECTOR3_H_
#define RF_VECTOR3_H_

#include <cmath>
#include <cassert>

namespace sora {

	class Vector3 {
	public:
		static const Vector3 ZERO;
		
		Vector3(): x(0.f), y(0.f), z(0.f) {}
		Vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
		Vector3(const Vector3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
		
		void set(float _x, float _y, float _z) {
			x = _x; y = _y; z = _z;
		}

		Vector3& operator=(const Vector3& rhs)  {
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		Vector3 operator-(const Vector3& rhs) const {
			return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
		}
		Vector3 operator+(const Vector3& rhs) const {
			return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
		}
		Vector3 operator*(float f) const {
			return Vector3(x*f, y*f, z*f);
		}
		Vector3 operator/(float f) const {
			assert(f != 0);
			return Vector3(x/f, y/f, z/f);
		}
		Vector3& operator+=(const Vector3& rhs) {
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& rhs) {
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}
	
		float dot(const Vector3& rhs) const {
			return x*rhs.x+y*rhs.y+z*rhs.z;
		}
		Vector3 cross(const Vector3& rhs) const {
			return Vector3(-z*rhs.y+y*rhs.z, z*rhs.x-x*rhs.z, -y*rhs.x+x*rhs.y);
		}
	
		float length() const {
			return sqrt(x*x+y*y+z*z);
		}
	
		float sqrLength() const {
			return x*x+y*y+z*z;
		}
	
		Vector3 normalize() const {
			float inv = 1/length();
			return Vector3(x*inv, y*inv, z*inv);
		}
	
		Vector3 negate() const {
			return Vector3(-x, -y, -z);
		}

		float x, y, z;
	};
    
} // namespace rfMath

#endif