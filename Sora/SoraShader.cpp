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
		mType = 0;
        bInternal = true;
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
    
    void SoraShader::setType(int32 t) { 
        mType = t;
    };
    
    void SoraShader::setInternal(bool flag) { 
        bInternal = flag;
    }
    
    SoraShaderContext* SoraShader::getShaderContext() const {
        return mShaderContext;
    }
	
	SoraShaderContext::SoraShaderContext() {
		err = 0;
	}
	
	SoraShaderContext::~SoraShaderContext() {
		clear();
	}
	
	std::list<SoraShader*>::iterator SoraShaderContext::getShaderIterator(SoraShader* shader) {
		ShaderList::iterator itShader = mShaders.begin();
		while(itShader != mShaders.end()) {
			if(shader == (*itShader)) {
				return itShader;
			}
			++itShader;
		}
		return mShaders.end();
	}
	    
    SoraShader* SoraShaderContext::attachShader(const wchar_t* file, const char* entry, int32 type) {
        if(getError() != 0) return NULL;
		
		SoraShader* shader = createShader(file, entry, type);
        if(shader) {
            shader->setName(GetUniqueStringId(file));
            shader->bInternal = true;
            attachShader(shader);
            return shader;
        }
        return NULL;
    }
	
	void SoraShaderContext::attachToRender() {
		SoraCore::Instance()->attachShaderContext(this);
	}

	bool SoraShaderContext::attachShaderList() {
		if(getError() != 0) return false;

		ShaderList::const_iterator itShader = mShaders.begin();
		int32 er = 1;
		
		while(itShader != mShaders.end()) {
			if((*itShader) != NULL) {
				(*itShader)->attach();
				if((*itShader)->mType == 0) er = 0;
			}
			++itShader;
		}
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachFromRender() {
		SoraCore::Instance()->detachShaderContext();
	}
    
	bool SoraShaderContext::detachShaderList() {
		if(err != 0) return false;

		ShaderList::iterator itShader = mShaders.begin();
		int32 er = 1;
		
		while(itShader != mShaders.end()) {
			if((*itShader) != NULL) {
				(*itShader)->detach();
				if((*itShader)->mType == 0) er = 1;
			}
			++itShader;
		}	
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachShader(SoraShader* shader) {
		if(err != 0) return;
		
		ShaderList::iterator itShader = getShaderIterator(shader);
		if(itShader != mShaders.end()) {
            if((*itShader)->bInternal) {
                delete (*itShader);
                (*itShader) = 0;
            }
            mShaders.erase(itShader);
		}
	}
	
    const SoraShaderContext::ShaderList& SoraShaderContext::getShaders() const { 
        return mShaders;
    }
    
    void SoraShaderContext::attachShader(SoraShader* shader) { 
        assert(shader != NULL); 
        mShaders.push_back(shader);
    }
    
	void SoraShaderContext::clear() {
		ShaderList::iterator itShader = mShaders.begin();
		while(itShader != mShaders.end()) {
			if((*itShader) && (*itShader)->bInternal) {
				delete (*itShader);
				(*itShader) = 0;
			}
            ++itShader;
		}
		mShaders.clear();
	}
    
    void SoraShaderContext::setError(int32 error) { 
        err = error;
    }
    
    int32 SoraShaderContext::getError() const { 
        return err;
    }
    
    uint32 SoraShaderContext::size() const { 
        return mShaders.size();
    }
	
} // namespace sora