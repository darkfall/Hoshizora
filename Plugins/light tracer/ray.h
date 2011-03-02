
#ifndef RF_RAY_H_
#define RF_RAY_H_

#include "vector3.h"

namespace rfMath {
	
	class ray {
	public:
		ray(const vector3& _origin, const vector3& _dir): origin(_origin), direction(_dir) {}
		
		vector3 getPoint(float t) {
			return origin+direction*t;
		}
		
		vector3 origin;
		vector3 direction;
	};
	
}

#endif