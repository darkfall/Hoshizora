//
//  SoraShaderManager.cpp
//  Sora
//
//  Created by Robert Bu on 7/14/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShaderManager.h"
#include "SoraShader.h"
#include "SoraException.h"
#include "SoraCore.h"

namespace sora {
    
	SoraShaderManager* SoraShaderManager::Instance() {
		static SoraShaderManager manager;
        return &manager;
	}

    SoraShaderManager::SoraShaderManager() {
        
    }
    
    SoraShaderManager::~SoraShaderManager() {
        
    }
    
    bool SoraShaderManager::init() {
        mShaderContext = SoraCore::Instance()->createShaderContext();
        if(!mShaderContext)
            return false;
        return true;
    }
    
    SoraShader* SoraShaderManager::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
        stringId sid = GetUniqueStringId(file);
        ShaderMap::iterator itShader = mShaders.find(sid);
        if(itShader != mShaders.end()) {
            return itShader->second;
        }
        
        SoraShader* shader = mShaderContext->createShader(file, entry, type);
        if(!shader) {
            THROW_SORA_EXCEPTION(RuntimeException, vamssg("Error creating shader %s", ws2s(file).c_str()));
            return NULL;
        }
        mShaders.insert(std::make_pair(sid, shader));
        mRevShaders.insert(std::make_pair(shader, sid));
        return shader;
    }
    
    SoraShader* SoraShaderManager::createUniqueShader(const SoraWString& file, const SoraString& entry, int32 type) {
        SoraShader* shader = mShaderContext->createShader(file, entry, type);
        if(!shader) {
            THROW_SORA_EXCEPTION(RuntimeException, vamssg("Error creating shader %s", ws2s(file).c_str()));
            return NULL;
        }
        return shader;
    }
    
    void SoraShaderManager::freeShader(SoraShader* shader) {
        RevShaderMap::iterator itShaderRev = mRevShaders.find(shader);
        if(itShaderRev != mRevShaders.end()) {
            ShaderMap::iterator itShader = mShaders.find(itShaderRev->second);
            if(itShader != mShaders.end())
                mShaders.erase(itShader);
            else
                THROW_SORA_EXCEPTION(RuntimeException, "Shader map does not match rev shader map, some error maybe happened");
            mRevShaders.erase(itShaderRev);
        }
        FreeShader(shader);
    }
    
} // namespace sora
 