/*
 *  camera.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/16/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_CAMERA_H_
#define RF_CAMERA_H_

namespace rfMath {
	
	class ray;
	
	class camera {
	public:
		camera(const vector3& _eye, const vector3& _front, const vector3& _refUp, float _fov):
			eye(_eye), front(_front), refUp(_refUp), fov(_fov) {
			right = front.cross(refUp);
			up = right.cross(front);
			fovScale = tan(fov*0.5f*D_PI/180)*2;
		}
		
		virtual ~camera() {}
		
		virtual ray generateRay(float x, float y) = 0;
		
		
		void rotate(float x, float y, float z) {
			eye.x += x;
			eye.y += y;
			eye.z += z;
		}
		void setEye(const vector3& _eye) {
			eye = _eye;
		}
		void setFront(const vector3& _front) {
			front = _front;
			up = right.cross(front);
		}
		void setRefUp(const vector3& _refUp) {
			refUp = _refUp;
		}
		void setFov(float _fov) {
			fov = _fov;
		}
		
		vector3 getFront() {
			return front;
		}
		
		vector3 getEye() {
			return eye;
		}
		
		
		vector3 eye;
		vector3 front;
		vector3 refUp;
		vector3 right;
		vector3 up;
		float fov;
		float fovScale;
	};
	
} // namespace rfMath

#endif