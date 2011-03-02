/*
 *  geometrySet.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/17/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef RF_GEOMETRY_SET_H_
#define RF_GEOMETRY_SET_H_

#include "geometry.h"

#include <vector>

#define DISTANCE_INFINITY MAXFLOAT

namespace rfMath {
	
	class geometrySet: public geometry {
	public:
		geometrySet() {}
		virtual ~geometrySet() {}
		
		void add(geometry* geo) {
			geometries.push_back(geo);
		}
		void del(geometry* geo) {
			for(size_t i=0; i<geometries.size(); ++i) {
				if(geometries[i] == geo) {
					geometries.erase(geometries.begin()+i);
					break;
				}
			}
		}
		
		intersectResult intersect(ray& r) {
			float minDistance = DISTANCE_INFINITY;
			intersectResult minResult;
			for(int i=0; i<geometries.size(); ++i) {
				intersectResult result = geometries[i]->intersect(r);
				if(result.geo != NULL && result.distance < minDistance) {
					minDistance = result.distance;
					minResult = result;
				}
			}
			return minResult;
		}
		
	private:
		typedef std::vector<geometry*> GEO_CONT;
		GEO_CONT geometries;
	};
	
} // namespace rfMath

#endif
