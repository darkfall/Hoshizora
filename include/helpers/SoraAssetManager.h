/*
 *  SoraAssetManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ASSET_MANAGER_H_
#define SORA_ASSET_MANAGER_H_

#include "../SoraPlatform.h"
#include "../SoraSingleton.h"

namespace sora {
	
	class SORA_API SoraAssetManager: public SoraSingleton<SoraAssetManager> {
	protected:
		friend class SoraSingleton<SoraAssetManager>;
		
		SoraAssetManager() {}
		~SoraAssetManager() {}
		
	public:
		
	};
	
} // namespace sora

#endif // SORA_ASSET_MANAGER_H_