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
	 *
	 **/
	
	class SoraPrefab: public SoraObject {
	public:
		SoraPrefab();
		~SoraPrefab();
		
		bool serializable() { return true; }
	};
	
	
} // namespace sora

#endif // SORA_PREFAB_H_