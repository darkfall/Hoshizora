#ifndef RF_INTERSECT_RESULT_H_
#define RF_INTERSECT_RESULT_H_

#include "geometry.h"
#include "vector3.h"

namespace rfMath {
	
	class intersectResult {
	public:
		intersectResult(): geo(NULL), distance(0) {}
		
		intersectResult& operator=(const intersectResult& rhs) {
			geo = rhs.geo;
			distance = rhs.distance;
			position = rhs.position;
			normal = rhs.normal;
			return *this;
		}
		
		geometry* geo;
		float distance;
		vector3 position;
		vector3 normal;
	};
	
}

#endif