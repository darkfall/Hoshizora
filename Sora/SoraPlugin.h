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
#include "uncopyable.h"

namespace sora {

	class SORA_API SoraPlugin: public uncopyable {
	public:	
        SoraPlugin() {}
        virtual ~SoraPlugin() {}
        
		// calls when install a plugin
		virtual void install() {}
	
		// calls when initialise a plugin
		virtual void initialise() {}
	
		// calls when shutdown a plugin
		virtual void shutdown() {}

		// calls when unistsall a plugin
		virtual void unistall() {}
		
		// calls every frame
		virtual void update() {}
        
        // calls when reinitialise a plugin under some special situations
        virtual void reinitialise() {}
	
		virtual const SoraString getName() const = 0;
	};

} // namespace sora

#endif // sora_plugin_h