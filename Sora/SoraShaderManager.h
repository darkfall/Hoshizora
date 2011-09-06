//
//  SoraShaderManager.h
//  Sora
//
//  Created by Robert Bu on 7/14/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderManager_h
#define Sora_SoraShaderManager_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraShader.h"
#include "SoraStringId.h"
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
        SoraShader* createShader(const util::String& file, const util::String& entry, int32 type);
        
        // would create a new shader whether it exists in the manager or not
        SoraShader* createUniqueShader(const util::String& file, const util::String& entry, int32 type);
        
        void freeShader(SoraShader* shader);
        
    private:
        SoraShaderContext* mShaderContext;
        
        typedef std::map<SoraStringId, SoraShader*> ShaderMap;
        typedef std::map<SoraShader*, SoraStringId> RevShaderMap;
        ShaderMap mShaders;
        RevShaderMap mRevShaders;
    };
    
    static SoraShader* CreateShader(const util::String& file, const util::String& entry, int32 type) {
        return SoraShaderManager::Instance()->createShader(file, entry, type);
    }
    
    static SoraShader* CreateFragmentShader(const util::String& file, const util::String& entry) {
        return SoraShaderManager::Instance()->createShader(file, entry, FRAGMENT_SHADER);
    }
    
    static SoraShader* CreateVertexShader(const util::String& file, const util::String& entry) {
        return SoraShaderManager::Instance()->createShader(file, entry, VERTEX_SHADER);
    }
    
    static SoraShader* CreateUniqueShader(const util::String& file, const util::String& entry, int32 type) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, type);
    }
    
    static SoraShader* CreateUniqueFragmentShader(const util::String& file, const util::String& entry) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, FRAGMENT_SHADER);
    }
    
    static SoraShader* CreateUniqueVertexShader(const util::String& file, const util::String& entry) {
        return SoraShaderManager::Instance()->createUniqueShader(file, entry, VERTEX_SHADER);
    }
    
} // namespace sora



#endif
