/*
 *  SoraShader.cpp
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/21/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraShader.h"

#include "SoraCore.h"

namespace sora {

	SoraShader::SoraShader() {
		mType = UNKNOWN_SHADEr;
        mErrorCode = ShaderNoError;
        mShaderContext = NULL;
	}
	
	SoraShader::~SoraShader() {
		
	}
	
	bool SoraShader::setParameter1f(const char* name, float32 v1) {
		float32 arr[1];
		arr[0] = v1;
		return setParameterfv(name, arr, 1);
	}
	
	bool SoraShader::setParameter2f(const char* name, float32 v1, float32 v2) {
		float32 arr[2];
		arr[0] = v1; arr[1] = v2;
		return setParameterfv(name, arr, 2);
	}
	
	bool SoraShader::setParameter3f(const char* name, float32 v1, float32 v2, float32 v3) {
		float32 arr[3];
		arr[0] = v1; arr[1] = v2; arr[2] = v3;
		return setParameterfv(name, arr, 3);
	}
	
	bool SoraShader::setParameter4f(const char* name, float32 v1, float32 v2, float32 v3, float32 v4) {
		float32 arr[4];
		arr[0] = v1; arr[1] = v2; arr[2] = v3; arr[3] = v4;
		return setParameterfv(name, arr, 4);
	}
	
	bool SoraShader::setParameter1i(const char* name, int32 v1) {
		int32 arr[1];
		arr[0] = v1;
		return setParameteriv(name, arr, 1);
	}
	
	bool SoraShader::setParameter2i(const char* name, int32 v1, int32 v2) {
		int32 arr[2];
		arr[0] = v1; arr[1] = v2;
		return setParameteriv(name, arr, 2);
	}
	
	bool SoraShader::setParameter3i(const char* name, int32 v1, int32 v2, int32 v3) {
		int32 arr[3];
		arr[0] = v1; arr[1] = v2; arr[2] = v3;
		return setParameteriv(name, arr, 3);
	}
	
	bool SoraShader::setParameter4i(const char* name, int32 v1, int32 v2, int32 v3, int32 v4) {
		int32 arr[4];
		arr[0] = v1; arr[1] = v2; arr[2] = v3; arr[3] = v4;
		return setParameteriv(name, arr, 4);
	}
	
    int32 SoraShader::getType() const {
        return mType;
    }
    
    SoraShaderContext* SoraShader::getShaderContext() const {
        return mShaderContext;
    }
    
    void SoraShader::setError(int32 error) {
        mErrorCode = error;
    }
    
    int32 SoraShader::getError() const {
        return mErrorCode;
    }
	
	SoraShaderContext::SoraShaderContext() {
	}
	
	SoraShaderContext::~SoraShaderContext() {
		clear();
	}
	
    SoraShader* SoraShaderContext::attachShader(const SoraWString& file, const SoraString& entry, int32 type) {
        if(type == FRAGMENT_SHADER)
            return attachFragmentShader(file, entry);
        else if(type == VERTEX_SHADER)
            return attachVertexShader(file, entry);
        else 
            THROW_SORA_EXCEPTION(RuntimeException, "Unknown shader type");
        return NULL;
    }
	
	void SoraShaderContext::attachToRender() {
		SoraCore::Instance()->attachShaderContext(this);
	}

	bool SoraShaderContext::attachShaderList() {
        bool result = true;
        if(mVertexShader.get())
            result = mVertexShader->attach();
        if(mFragmentShader.get())
            result = mFragmentShader->attach();
        return result;
	}
    
    bool SoraShaderContext::detachShaderList() {
        bool result = true;
        if(mVertexShader.get())
            result = mVertexShader->detach();
        if(mFragmentShader.get())
            result = mFragmentShader->detach();
        return result;
    }
	
	void SoraShaderContext::detachFromRender() {
		SoraCore::Instance()->detachShaderContext();
	}
    
	void SoraShaderContext::detachFragmentShader() {
        mFragmentShader.reset();
    }
    
    void SoraShaderContext::detachVertexShader() {
        mVertexShader.reset();
    }
    
	void SoraShaderContext::clear() {
		mFragmentShader.reset();
        mVertexShader.reset();
	}
    
    SoraShader* SoraShaderContext::attachFragmentShader(const SoraWString& file, const SoraString& entry) {
        SoraShader* shader = createShader(file, entry, FRAGMENT_SHADER);
        if(!shader)
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating shader");
        if(shader->getError() != ShaderNoError) {
            shader->release();
            THROW_SORA_EXCEPTION(RuntimeException, vamssg("Shader returned with error code %d", shader->getError()));
            return NULL;
        }
        mFragmentShader.assign(shader);
        return shader;
    }
    
    SoraShader* SoraShaderContext::attachVertexShader(const SoraWString& file, const SoraString& entry) {
        SoraShader* shader = createShader(file, entry, VERTEX_SHADER);
        if(!shader)
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating shader");
        if(shader->getError() != ShaderNoError) {
            shader->release();
            THROW_SORA_EXCEPTION(RuntimeException, vamssg("Shader returned with error code %d", shader->getError()));
            return NULL;
        }
        mVertexShader.assign(shader);
        return shader;
    }
    
    SoraShader* SoraShaderContext::getFragmentShader() const {
        return mFragmentShader.get();
    }
    
    SoraShader* SoraShaderContext::getVertexShader() const {
        return mVertexShader.get();
    }
	
    void SoraShaderContext::attachShader(SoraShader* shader) {
        sora_assert(shader);
        if(shader->getType() == FRAGMENT_SHADER)
            attachFragmentShader(shader);
        else if(shader->getType() == VERTEX_SHADER)
            attachVertexShader(shader);
        else 
            THROW_SORA_EXCEPTION(RuntimeException, "Unknown shader type");
    }
    
    void SoraShaderContext::attachFragmentShader(SoraShader* shader) {
        sora_assert(shader);
        
        shader->duplicate();
        mFragmentShader.assign(shader);
    }
    
    void SoraShaderContext::attachVertexShader(SoraShader* shader) {
        sora_assert(shader);
        
        shader->duplicate();
        mVertexShader.assign(shader);
    }
    
    void SoraShaderContext::detachShader(SoraShader* shader) {
        sora_assert(shader);
        
        if(shader->getType() == FRAGMENT_SHADER && shader == mFragmentShader.get())
            detachFragmentShader();
        else if(shader->getType() == VERTEX_SHADER && shader == mVertexShader.get())
            detachVertexShader();
    }
    
    bool SoraShaderContext::isAvailable() {
        return (mFragmentShader.get() != NULL || mVertexShader.get() != NULL);
    }
    
} // namespace sora