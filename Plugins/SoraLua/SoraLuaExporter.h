//
//  SoraLuaExporter.h
//  Sora
//
//  Created by Ruiwei Bu on 8/25/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraLuaExporter_h
#define Sora_SoraLuaExporter_h

#include "luaclass.h"
#include "LuaPlusHelper.h"
#include "SoraLuaConfig.h"
#include "signal/SoraSignal.h"

namespace sora {
    
    struct SoraLuaExporter {
        typedef SoraSignal<void(LuaPlus::LuaState*)> ExporterSignal;
        
        template<typename T>
        static SoraConnection ConnectExporterFunc(const T& func) {
            return mExporter.connect(func);
        }
        
        static void OnExport(LuaPlus::LuaState* state);
        
    private:
        static ExporterSignal mExporter;
    };
    
#define SORA_DEF_LUA_AUTO_EXPORT_FUNC(func) \
    sora::SoraLuaExporter::ConnectExporterFunc(func);
    
    /**
     * Helper macro that help check a symbol has been defined and register an export func
     **/
#define SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(Symbol) \
    SORA_LUA_AUTO_EXPORT_ALL || Symbol
    
#define SORA_LUA_AUTO_EXPORT_FUNC(fn) \
    SORA_STATIC_RUN_CODE_I(luaExport##fn, sora::SoraLuaExporter::ConnectExporterFunc(fn)) \
    
    
} // namespace sora


#endif
