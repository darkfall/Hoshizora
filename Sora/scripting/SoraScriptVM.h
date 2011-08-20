//
//  SoraScriptVM.h
//  Sora
//
//  Created by Ruiwei Bu on 8/20/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraScriptVM_h
#define Sora_SoraScriptVM_h

#include "../SoraPlatform.h"
#include "../SoraAutoPtr.h"
#include "../uncopyable.h"

namespace sora {
    
    class IScriptingSystem;
    class IScript;
    class IScriptVM;
    
    typedef SoraAutoPtr<IScript> ScriptPtr;
    typedef SoraAutoPtr<IScriptVM> ScriptVMPtr;
    
    /**
     * Interfaces for work with script
     **/
    
    class SORA_API SoraScript {
    public:
        virtual bool isLoaded() = 0;
        virtual void setData(const SoraString& data) = 0;
    };
    
    class SORA_API SoraScriptVM: uncopyable {
    public:
        virtual ~SoraScriptVM() {}
        
        virtual IScriptingSystem* getCreator() const = 0;
        virtual void execute(ScriptPtr script) = 0;
        virtual void callVoidFunc(const SoraString& funcName) = 0;
    };
    
    class SORA_API SoraScriptingSystem {
    public:
        enum Language {
            L_LUA = 0,
            L_LSCRIPT,
        };
        
        virtual int32 getLanguage() const = 0;
        virtual SoraString getName() const = 0;
        
        virtual ScriptVMPtr createVM() = 0;
        virtual ScriptPtr createScriptFromFile(const SoraString& file) = 0;
    };
    
} // namespace sora

#endif