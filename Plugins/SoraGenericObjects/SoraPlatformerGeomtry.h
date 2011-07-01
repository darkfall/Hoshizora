/*
 *  SoraPlatformerGeomtry.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_PLATFORMER_GEOMETRY_H_
#define SORA_PLATFORMER_GEOMETRY_H_

#include "SoraGeometry.h"

namespace sora {
	
	class SoraPlatformerGeometry: public SoraGeometry {
	public:
		SoraPlatformerGeometry();
		~SoraPlatformerGeometry();
		
		void addVertex(float32 x, float32 y);
		
		void render();
		uint32 update(float32 dt);
	};
	
	
} // namespace sora


#endif // SORA_PLATFORMER_GEOMETRY_H_