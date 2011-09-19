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
    
    SoraShader* SoraShaderManager::createShader(const StringType& file, const SoraString& entry, int32 type) {
        SoraShader* shader = mShaderContext->createShader(file, entry, type);
        if(!shader) {
            THROW_SORA_EXCEPTION(RuntimeException, vamssg("Error creating shader %s", file.c_str()));
            return NULL;
        }
        return shader;
    }
    
    SoraShader* SoraShaderManager::createShaderFromMem(const char* data, const SoraString& entry, int32 type) {
        SoraShader* shader = mShaderContext->createShaderFromMem(data, entry, type);
        if(!shader) {
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating shader from mem");
            return NULL;
        }
        return shader;
    }
    

} // namespace sora
 