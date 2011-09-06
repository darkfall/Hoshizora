/*
 *  PluginConcept.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_MANAGER_H
#define SORA_PLUGIN_MANAGER_H

#include "SoraPlugin.h"
#include <map>

namespace sora {

    class SORA_API SoraPluginManager {
        typedef std::map<util::String, SoraPlugin*> PluginMap;
        typedef PluginMap::iterator PluginIterator;
        
    public:
        SoraPluginManager() {}
        virtual ~SoraPluginManager();
        
        virtual void registerPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* unistallPlugin(const util::String& name);
        virtual SoraPlugin* unistallPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* getPlugin(const util::String& pluginname);
                
    private:
        PluginMap mPluginMap;
    };

} // namespace sora

#endif // SORA_PLUGIN_MANAGER_H