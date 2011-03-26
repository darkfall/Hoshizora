/*
 *  PluginConcept.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_MANAGER_H
#define SORA_PLUGIN_MANAGER_H

#include "SoraPlugin.h"
#include <list>

namespace sora {

class SoraPluginManager {
	typedef std::list<SoraPlugin*>::iterator itPlugin;
	
public:
	SoraPluginManager() {}
	virtual ~SoraPluginManager();
	
	virtual void registerPlugin(SoraPlugin* pPlugin);
	
	virtual void unistallPlugin(const SoraString& sPluginName);
	virtual void unistallPlugin(SoraPlugin* pPlugin);
	
	virtual SoraPlugin* getPlugin(const SoraString& sPluginName);
    
    virtual void update();
	
private:
	std::list<SoraPlugin*> pPluginList;
	
	bool bIsInitialised;
};

} // namespace sora

#endif // SORA_PLUGIN_MANAGER_H