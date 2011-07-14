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

namespace sora {
	
	/**
	 *	Base class for prefabs
	 *  A prefab is a predefined a template of game world objects
     *  Must be able to load and save to file to save the state
	 **/
	
	class SoraPrefab: public SoraObject {
	public:
        virtual bool saveToFile(const SoraWString& file) = 0;
        virtual bool loadFromFile(const SoraWString& file) = 0;
	};
	
	
} // namespace sora

#endif // SORA_PREFAB_H_