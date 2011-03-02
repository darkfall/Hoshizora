/*
 *  SoraPlugin.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_H
#define SORA_PLUGIN_H

#include "SoraPlatform.h"

namespace sora {

	class SoraPlugin {
	public:	
		// calls when install a plugin
		virtual void install() = 0;
	
		// calls when initialise a plugin
		virtual void initialise() = 0;
	
		// calls when shutdown a plugin
		virtual void shutdown() = 0;

		// calls when unistsall a plugin
		virtual void unistall() = 0;
		
		// calls every frame
		virtual void update() = 0;
	
		virtual const SoraString getName() const = 0;
	};

} // namespace sora

#endif // sora_plugin_h