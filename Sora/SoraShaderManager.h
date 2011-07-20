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
    
    class SORA_API SoraShaderManager {
    protected:
        SoraShaderManager();
        ~SoraShaderManager();
        
    public:
		static SoraShaderManager* Instance();
		static void Destroy();

        // return the same shader if it have benn created by the manager
        SoraShader* createShader(const wchar_t* file, const char* entry, int32 type);
        
        // would create a new shader whether it exists in the manager or not
        SoraShader* createUniqueShader(const wchar_t* file, const char* entry, int32 type);
        
        void freeShader(SoraShader* shader);
        
    private:
		static SoraShaderManager* mInstance;

        typedef std::map<stringId, SoraShader*> ShaderMap;
        typedef std::map<SoraShader*, stringId> RevShaderMap;
        ShaderMap mShaders;
        RevShaderMap mRevShaders;
    };
    
    static SoraShader* CreateShader(const wchar_t* file, const char* entry, int32 type) {
        return SoraShaderManager::Instance()->createShader(file, entry, type);
    }
    
    static SoraShader* CreateUniqueShader(const wchar_t* file, const char* entry, int32 type) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, type);
    }
    
} // namespace sora



#endif
