/*
 *  PluginConcept.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraPluginManager.h"

namespace sora {

SoraPluginManager::~SoraPluginManager() {
	itPlugin plugin = pPluginList.begin();
	while(plugin != pPluginList.end()) {
		if(bIsInitialised) (*plugin)->shutdown();
		
		(*plugin)->unistall();
		
		++plugin;
	}
	pPluginList.clear();
}

void SoraPluginManager::registerPlugin(SoraPlugin* pPlugin) {
	if(pPlugin) {
		pPlugin->install();
		pPluginList.push_back(pPlugin);
		
		if(bIsInitialised) pPlugin->initialise();
	}
}

void SoraPluginManager::unistallPlugin(const SoraString& sPluginName) {
	itPlugin plugin = pPluginList.begin();
	while(plugin != pPluginList.end()) {
		if((*plugin)->getName().compare(sPluginName) == 0) {
			if(bIsInitialised) (*plugin)->shutdown();
			
			(*plugin)->unistall();
			
			plugin = pPluginList.erase(plugin);
		} else ++plugin;
	}
}

void SoraPluginManager::unistallPlugin(SoraPlugin* pPlugin) {
	itPlugin plugin = pPluginList.begin();
	while(plugin != pPluginList.end()) {
		if(*plugin == pPlugin) {
			if(bIsInitialised) (*plugin)->shutdown();
			
			(*plugin)->unistall();
			
			plugin = pPluginList.erase(plugin);
		} else ++plugin;
	}
}

SoraPlugin* SoraPluginManager::getPlugin(const SoraString& sPluginName) {
	itPlugin plugin = pPluginList.begin();
	while(plugin != pPluginList.end()) {
		if((*plugin)->getName().compare(sPluginName) == 0) {
			return (*plugin);
		}
		++plugin;
	}
	return 0;
}

} // namespace sora