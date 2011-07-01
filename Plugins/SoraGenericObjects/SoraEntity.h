/*
 *  SoraGenericObject.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ENTITY_H_
#define SORA_ENTITY_H_

#include "SoraPlatform.h"
#include "SoraObject.h"

namespace sora {
	
	/** 
	 *	A Generic Object for project usage
	 *	Target:
	 *	Support png, jpeg, tga, bmp image format
	 *  Support intergration with other plugins, physical object etc
	 *	Support sub objects
	 *  Support Gif animation
	 *	Support Shader and ImageEffect
	 *	Support text
	 **/
	
	class SoraEntity: public SoraObject {
	public:
		SoraEntity();
		~SoraEntity();
		
	};
	
} // namespace sora

#endif // SORA_GENERIC_OBJECT_H_