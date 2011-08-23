//
//  SoraShaderEnabled.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraShaderEnabled.h"
#include "SoraCore.h"

namespace sora {
    
    SoraShaderEnabledObject::SoraShaderEnabledObject() {
        mShaderContext = NULL;
    }
    
    SoraShaderEnabledObject::~SoraShaderEnabledObject() {
        if(mShaderContext) {
            delete mShaderContext;
        }
    }
    
    SoraShader* SoraShaderEnabledObject::attachShader(const SoraWString& file, const SoraString& entry, int32 type) {
        checkShaderContext();
        
        if(mShaderContext) {
            return mShaderContext->attachShader(file, entry, type);
        }
		return NULL;
    }
    
    SoraShader* SoraShaderEnabledObject::attachFragmentShader(const SoraWString& file, const SoraString& entry) {
        return attachShader(file, entry, FRAGMENT_SHADER);
    }
    
    SoraShader* SoraShaderEnabledObject::attachVertexShader(const SoraWString& file, const SoraString& entry) {
        return attachShader(file, entry, VERTEX_SHADER);
    }
    
    void SoraShaderEnabledObject::detachShader(SoraShader* shader) {
        if(mShaderContext) {
            mShaderContext->detachShader(shader);
        }
    }
    
    void SoraShaderEnabledObject::detachFragmentShader() {
        if(mShaderContext) {
            mShaderContext->detachFragmentShader();
        }
    }
    
    void SoraShaderEnabledObject::detachVertexShader() {
        if(mShaderContext) {
            mShaderContext->detachVertexShader();
        }
    }
    
    void SoraShaderEnabledObject::attachShader(SoraShader* shader) {
        checkShaderContext();
        
        if(mShaderContext) {
            mShaderContext->attachShader(shader);
        }
    }
    
    void SoraShaderEnabledObject::attachFragmentShader(SoraShader* shader) {
        checkShaderContext();
        
        if(mShaderContext) {
            mShaderContext->attachFragmentShader(shader);
        }
    }
    
    void SoraShaderEnabledObject::attachVertexShader(SoraShader* shader) {
        checkShaderContext();
        
        if(mShaderContext) {
            mShaderContext->attachVertexShader(shader);
        }
    }
    
    void SoraShaderEnabledObject::attachShaderToRender() {
        if(mShaderContext) {
            mShaderContext->attachToRender();
        }
    }
    
    void SoraShaderEnabledObject::detachShaderFromRender() {
        if(mShaderContext) {
            mShaderContext->detachFromRender();
        }
    }
    
    void SoraShaderEnabledObject::checkShaderContext() {
        if(!mShaderContext) {
            mShaderContext = SoraCore::Instance()->createShaderContext();
            if(!mShaderContext)
                log_error("error creating shader context");
        }
    }
    
    bool SoraShaderEnabledObject::hasShader() const {
        return mShaderContext && mShaderContext->isAvailable();
    }
    
    void SoraShaderEnabledObject::clearShader() {
        if(mShaderContext)
            mShaderContext->clear();
    }
} // namespace sora
