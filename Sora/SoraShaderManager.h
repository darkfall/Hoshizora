//
//  SoraShaderManager.h
//  Sora
//
//  Created by Ruiwei Bu on 7/14/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderManager_h
#define Sora_SoraShaderManager_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraShader.h"
#include "stringId.h"
#include <map>

namespace sora {
    
    class SORA_API SoraShaderManager {
    protected:
        SoraShaderManager();
        ~SoraShaderManager();
        
    public:
		static SoraShaderManager* Instance();
        
        bool init();

        // return the same shader if it have been created by the manager
        SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type);
        
        // would create a new shader whether it exists in the manager or not
        SoraShader* createUniqueShader(const SoraWString& file, const SoraString& entry, int32 type);
        
        void freeShader(SoraShader* shader);
        
    private:
        SoraShaderContext* mShaderContext;
        
        typedef std::map<stringId, SoraShader*> ShaderMap;
        typedef std::map<SoraShader*, stringId> RevShaderMap;
        ShaderMap mShaders;
        RevShaderMap mRevShaders;
    };
    
    static SoraShader* CreateShader(const SoraWString& file, const SoraString& entry, int32 type) {
        return SoraShaderManager::Instance()->createShader(file, entry, type);
    }
    
    static SoraShader* CreateFragmentShader(const SoraWString& file, const SoraString& entry) {
        return SoraShaderManager::Instance()->createShader(file, entry, FRAGMENT_SHADER);
    }
    
    static SoraShader* CreateVertexShader(const SoraWString& file, const SoraString& entry) {
        return SoraShaderManager::Instance()->createShader(file, entry, VERTEX_SHADER);
    }
    
    static SoraShader* CreateUniqueShader(const SoraWString& file, const SoraString& entry, int32 type) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, type);
    }
    
    static SoraShader* CreateUniqueFragmentShader(const SoraWString& file, const SoraString& entry) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, FRAGMENT_SHADER);
    }
    
    static SoraShader* CreateUniqueVertexShader(const SoraWString& file, const SoraString& entry) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, VERTEX_SHADER);
    }
    
} // namespace sora



#endif
