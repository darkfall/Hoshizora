//
//  SoraShaderManager.h
//  Sora
//
//  Created by Ruiwei Bu on 7/14/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraShaderManager_h
#define Sora_SoraShaderManager_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include "stringId.h"
#include <map>

namespace sora {
    
    class SoraShader;
    
    class SoraShaderManager: public SoraSingleton<SoraShaderManager> {
    protected:
        friend class SoraSingleton<SoraShaderManager>;
        SoraShaderManager();
        ~SoraShaderManager();
        
    public:
        // return the same shader if it have benn created by the manager
        SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type);
        
        // would create a new shader whether it exists in the manager or not
        SoraShader* createUniqueShader(const SoraWString& file, const SoraString& entry, int32 type);
        
        void freeShader(SoraShader* shader);
        
    private:
        typedef std::map<stringId, SoraShader*> ShaderMap;
        typedef std::map<SoraShader*, stringId> RevShaderMap;
        ShaderMap mShaders;
        RevShaderMap mRevShaders;
    };
    
    static SoraShader* CreateShader(const SoraWString& file, const SoraString& entry, int32 type) {
        return SoraShaderManager::Instance()->createShader(file, entry, type);
    }
    
    static SoraShader* CreateUniqueShader(const SoraWString& file, const SoraString& entry, int32 type) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, type);
    }
    
} // namespace sora



#endif
