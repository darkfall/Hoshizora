/*
 *  perspectiveCamera.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/16/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#include "ray.h"
#include "camera.h"


namespace rfMath {

#ifndef RF_PERSPECTIVE_CAMERA_H_
#define RF_PERSPECTIVE_CAMERA_H_
		
	class perspectiveCamera: public camera {
	public:
		perspectiveCamera(const vector3& _eye, const vector3& _front, const vector3& _refUp, float _fov):
			camera(_eye, _front, _refUp, _fov) {
				printf("%f, %f, %f\n", eye.x, eye.y, eye.z);
		}
		
		ray generateRay(float x, float y) {
			vector3 r = right*((x-0.5f)*fovScale);
			vector3 u = up*((y-0.5f)*fovScale);
			vector3 dir = (front+r+u).normalize();
			return ray(eye, dir);
		}
		
	private:
		
	};
	
	
#endif

} // namespace rfMath