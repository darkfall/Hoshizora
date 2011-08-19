/*
 *  PluginConcept.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraPluginManager.h"

namespace sora {

    SoraPluginManager::~SoraPluginManager() {
        PluginIterator itPlugin = mPluginList.begin();
        while(itPlugin != mPluginList.end()) {
            if(bIsInitialised) (*itPlugin)->shutdown();
            
            (*itPlugin)->unistall();
            
            ++itPlugin;
        }
        mPluginList.clear();
    }
    
    void SoraPluginManager::registerPlugin(SoraPlugin* pPlugin) {
        if(pPlugin) {
            pPlugin->install();
            mPluginList.push_back(pPlugin);
            
            if(bIsInitialised) pPlugin->initialise();
        }
    }
    
    void SoraPluginManager::unistallPlugin(const SoraString& sPluginName) {
        PluginIterator itPlugin = mPluginList.begin();
        while(itPlugin != mPluginList.end()) {
            if((*itPlugin)->getName().compare(sPluginName) == 0) {
                if(bIsInitialised) (*itPlugin)->shutdown();
                
                (*itPlugin)->unistall();
                
                itPlugin = mPluginList.erase(itPlugin);
            } else ++itPlugin;
        }
    }
    
    void SoraPluginManager::unistallPlugin(SoraPlugin* pPlugin) {
        PluginIterator itPlugin = mPluginList.begin();
        while(itPlugin != mPluginList.end()) {
            if(*itPlugin == pPlugin) {
                if(bIsInitialised) (*itPlugin)->shutdown();
                
                (*itPlugin)->unistall();
                
                itPlugin = mPluginList.erase(itPlugin);
            } else ++itPlugin;
        }
    }
    
    SoraPlugin* SoraPluginManager::getPlugin(const SoraString& sPluginName) {
        PluginIterator itPlugin = mPluginList.begin();
        while(itPlugin != mPluginList.end()) {
            if((*itPlugin)->getName().compare(sPluginName) == 0) {
                return (*itPlugin);
            }
            ++itPlugin;
        }
        return 0;
    }
    
    void SoraPluginManager::update(float32 dt) {
        PluginIterator itPlugin = mPluginList.begin();
        while(itPlugin != mPluginList.end()) {
            (*itPlugin)->update(dt);
            ++itPlugin;
        }
    }

} // namespace sora