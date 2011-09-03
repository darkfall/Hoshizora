/*
 *  SoraQuaternion.h
 *  lParticle
 *
 *  Created by GriffinBu on 12/21/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef RF_QUATERNION_H_
#define RF_QUATERNION_H_

#include "SoraVector3.h"

namespace sora {
	
	class SoraQuaternion {
	public:
		SoraQuaternion(float _x, float _y, float _z, float _w):
			w(_w), x(_x), y(_y), z(_z) {}
		SoraQuaternion(const SoraQuaternion& quad):
			w(quad.w), x(quad.x), y(quad.y), z(quad.z) {}
		SoraQuaternion():
			w(0.), x(0.), y(0.), z(0.) {}
		
		void set(float _x, float _y, float _z, float _w) {
			x = _x; y = _y; z = _z; w = _w;
		}
		
		void set(const SoraQuaternion& quad) {
			x = quad.x; y = quad.y; z = quad.z; w = quad.w;
		}
		
		SoraVector3 get() {
			return SoraVector3(x, y, z);
		}
		
		inline SoraQuaternion& operator=(const SoraQuaternion& rhs) {
			x=rhs.x; y=rhs.y; z = rhs.z; w=rhs.w;
			return *this;
		}
		
		inline const SoraQuaternion operator*(const SoraQuaternion& rhs) {
			SoraQuaternion ans;
			float d1, d2, d3, d4;
			d1 = w*rhs.w;
			d2 = -x*rhs.x;
			d3 = -y*rhs.y;
			d4 = -z*rhs.z;
			ans.w = d1+d2+d3+d4;
			
			d1 = w*rhs.x;
			d2 = x*rhs.w;
			d3 = y*rhs.z;
			d4 = -z*rhs.y;
			ans.x = d1+d2+d3+d4;
			
			d1 = w*rhs.y;
			d2 = y*rhs.w;
			d3 = z*rhs.x;
			d4 = -x*rhs.z;
			ans.y = d1+d2+d3+d4;
			
			d1 = w*rhs.z;
			d2 = z*rhs.w;
			d3 = x*rhs.y;
			d4 = -y*rhs.x;
			ans.z = d1+d2+d3+d4;
			
			*this = ans;
			return *this;
		}
		
		void makeRotate(float angle, float _x, float _y, float _z) {
			float inversenorm = (float)1.0f/sqrt(x*x + y*y + z*z);
			float coshalfangle = cosf(0.5f*angle);
			float sinhalfangle = sinf(0.5f*angle);
			
			x = _x * sinhalfangle * inversenorm;
			y = _y * sinhalfangle * inversenorm;
			z = _z * sinhalfangle * inversenorm;
			w = coshalfangle;
		}
		
		void makeRotate(float angle, const SoraVector3& vec) {
			makeRotate(angle, vec.x, vec.y, vec.z);
		}
		
		void makeRotate(float roll, float pitch, float yaw) {
			float sh0 = sin(0.5f*roll);
			float ch0 = cos(0.5f*roll);
			float sh1 = sin(0.5f*pitch);
			float ch1 = cos(0.5f*pitch);
			float sh2 = sin(0.5f*yaw);
			float ch2 = cos(0.5f*yaw);
			
			x = sh0*ch1*ch2 + ch0*sh1*sh2;
			y = ch0*sh2*ch1 + sh0*ch2*sh1;
			z = ch0*ch2*sh1 - sh0*sh2*ch1;
			w = ch0*ch1*ch2 - sh0*sh1*sh2;
		}
		
		SoraVector3 rotate(const SoraVector3& v) const {
			float ux = w*v.x + y*v.z - z*v.y;
			float uy = w*v.y + z*v.x - x*v.z;
			float uz = w*v.z + x*v.y - y*v.x;
			float uw = -x*v.x - y*v.y - z*v.z;
			float vx = -uw*x + ux*w - uy*z + uz*y;
			float vy = -uw*y + uy*w - uz*x + ux*z;
			float vz = -uw*z + uz*w - ux*y + uz*x;
			return SoraVector3(vx, vy, vz);
		}
		
		void getRotate(float& angle, float& _x, float& _y, float& _z) const {
			float sinhalfangle = sqrt(x*x + y*y + z*z);
			angle = 2.f * atan2f(sinhalfangle, w);
			if(sinhalfangle) {
				_x = x / sinhalfangle;
				_y = y / sinhalfangle;
				_z = z / sinhalfangle;
			} else {
				_x = 0.0f;
				_y = 0.0f;
				_z = 0.0f;
			}
		}
		
		void getRotate(float& angle, SoraVector3& vec) const {
			getRotate(angle, vec.x, vec.y, vec.z);
		}
		
		void slerp(float t, const SoraQuaternion& from, const SoraQuaternion& to) {
			const float epsilon = 0.00001f;
			float cosomega, scale_from, scale_to;
			
			SoraQuaternion quatTo(to);
			
			cosomega = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;
			if(cosomega < 0.0f) {
				cosomega = -cosomega;
				quatTo.set(-to.x, -to.y, -to.z, -to.w);
			}
			
			if((1.0 - cosomega) > epsilon) {
				float omega = acos(cosomega);
				float invsinomega = 1.0f/sin(omega);
				scale_from = sin((1.0f-t)*omega)*invsinomega;
				scale_to = sin(t*omega)*invsinomega;
			} else {
				scale_from = 1.0f - t;
				scale_to = t;
			}
			
			x = from.x*scale_from + quatTo.x*scale_to;
			y = from.y*scale_from + quatTo.y*scale_to;
			z = from.z*scale_from + quatTo.z*scale_to;
			w = from.w*scale_from + quatTo.w*scale_to;
		}
		
	//private:
		float w, x, y, z;
	};
		
}	// namespace sora


#endif