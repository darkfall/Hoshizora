/*
 *  SoraPrefab.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_PREFAB_H_
#define SORA_PREFAB_H_

#include "SoraObject.h"
#include "SoraStream.h"

namespace sora {
	
	/**
	 *	Base class for prefabs
	 *  A prefab is a predefined a template of game world objects
     *  Must be able to load and save to file to save the state
	 **/
	
	class SORA_API SoraPrefab: public SoraObject {
	public:
        virtual bool        loadFromData(void* data, uint32 size) = 0;
        virtual void        saveToStream(SoraStream& stream) = 0;
	};
	
	
} // namespace sora

#endif // SORA_PREFAB_H_