//
//  SoraShaderEnabled.cpp
//  Sora
//
//  Created by Robert Bu on 8/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShaderEnabled.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "SoraShader.h"
#include "SoraFileSystem.h"

namespace sora {
    
    SoraShaderEnabledObject::SoraShaderEnabledObject():
    mInternalContext(false),
    mShaderContext(0) {
        
    }
    
    SoraShaderEnabledObject::~SoraShaderEnabledObject() {
        if(mInternalContext && mShaderContext) {
            delete mShaderContext;
        }
    }

    void SoraShaderEnabledObject::attachShaderContext(const StringType& tag) {
        SoraShaderContext* context = SoraCore::Ptr->getFileSystem()->getShaderResource(tag);
        if(context)
            attachShaderContext(context);
    }
    
    void SoraShaderEnabledObject::attachShaderContext(SoraShaderContext* context) {
        sora_assert(context);
        
        if(mInternalContext && mShaderContext)
            delete mShaderContext;
        mShaderContext = context;
        mInternalContext = false;
    }
    
    SoraShaderContext* SoraShaderEnabledObject::getShaderContext() const {
        return mShaderContext;
    }
    
    SoraShader* SoraShaderEnabledObject::attachShader(const StringType& file, const SoraString& entry, int32 type) {
        checkShaderContext();
        
        if(mShaderContext) {
            return mShaderContext->attachShader(file, entry, type);
        }
        return NULL;
    }
    
    SoraShader* SoraShaderEnabledObject::attachFragmentShader(const StringType& file, const SoraString& entry) {
        return attachShader(file, entry, SoraShader::FragmentShader);
    }
    
    SoraShader* SoraShaderEnabledObject::attachVertexShader(const StringType& file, const SoraString& entry) {
        return attachShader(file, entry, SoraShader::VertexShader);
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
                log_error("SoraShaderEnabledObject: error creating shader context");
            mInternalContext = true;
        }
    }
    
    bool SoraShaderEnabledObject::hasShader() const {
        return mShaderContext && mShaderContext->isAvailable();
    }
    
    void SoraShaderEnabledObject::clearShader() {
        if(mShaderContext)
            mShaderContext->clear();
    }
    
    SoraShader* SoraShaderEnabledObject::getFragmentShader() const {
        if(mShaderContext)
            return mShaderContext->getFragmentShader();
        return 0;
    }
    
    SoraShader* SoraShaderEnabledObject::getVertexShader() const {
        if(mShaderContext)
            return mShaderContext->getVertexShader();
        return 0;
    }
    
} // namespace sora
