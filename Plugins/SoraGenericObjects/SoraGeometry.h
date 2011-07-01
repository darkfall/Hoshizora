/*
 *  SoraGeometry.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GEOMETRY_H_
#define SORA_GEOMETRY_H_

#include "SoraObject.h"

namespace sora {
	
	/**
	 *  Base class for geometry
	 **/
	
	class SoraGeometry: public SoraObject {
	public:
		SoraGeometry();
		~SoraGeometry();

	};
	
} // namespace sora

#endif // SORA_GEOMETRY_H_