/*
 *  PluginConcept.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_MANAGER_H
#define SORA_PLUGIN_MANAGER_H

#include "SoraPlugin.h"
#include <map>

namespace sora {

    class SORA_API SoraPluginManager {
        typedef std::map<SoraString, SoraPlugin*> PluginMap;
        typedef PluginMap::iterator PluginIterator;
        
    public:
        SoraPluginManager() {}
        virtual ~SoraPluginManager();
        
        virtual void registerPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* unistallPlugin(const SoraString& name);
        virtual SoraPlugin* unistallPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* getPlugin(const SoraString& pluginname);
                
    private:
        PluginMap mPluginMap;
    };

} // namespace sora

#endif // SORA_PLUGIN_MANAGER_H