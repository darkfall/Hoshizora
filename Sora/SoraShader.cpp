/*
 *  SoraShader.cpp
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraShader.h"

#include "SoraCore.h"

namespace sora {

	SoraShader::SoraShader() {
		this->type = 0;
	}
	
	SoraShader::~SoraShader() {
		
	}
	
	SoraShaderContext::SoraShaderContext() {
		err = 0;
	}
	
	SoraShaderContext::~SoraShaderContext() {
		ShaderList::iterator itShader = shaders.begin();
		while(itShader != shaders.end()) {
			if((*itShader)) {
				delete (*itShader);
				(*itShader) = 0;
				++itShader;
			}
		}
	}
	
	std::list<SoraShader*>::iterator SoraShaderContext::getShaderIterator(SoraShader* shader) {
		ShaderList::iterator itShader = shaders.begin();
		while(itShader != shaders.end()) {
			if(shader == (*itShader)) {
				return itShader;
			}
			++itShader;
		}
		return shaders.end();
	}
    
    SoraShader* SoraShaderContext::attachShader(const SoraWString& file, const SoraString& entry, int32 type) {
        if(getError() != 0) return false;
		
		SoraShader* shader = createShader(file, entry, type);
        if(shader) {
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

		ShaderList::const_iterator itShader = getShaderList().begin();
		int32 er = 1;
		
		while(itShader != getShaderList().end()) {
			(*itShader)->attach();
			if((*itShader)->type == 0) er = 0;
			
			++itShader;
		}
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachFromRender() {
		SoraCore::Instance()->detachShaderContext();
	}

	bool SoraShaderContext::detachShaderList() {
		if(err != 0) return false;

		ShaderList::iterator itShader = shaders.begin();
		int32 er = 1;
		
		while(itShader != shaders.end()) {
			(*itShader)->detach();
			if((*itShader)->type == 0) er = 1;
			
			++itShader;
		}	
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachShader(SoraShader* shader) {
		if(err != 0) return;
		
		ShaderList::iterator itShader = getShaderIterator(shader);
		if(itShader != shaders.end()) {
			delete (*itShader);
			(*itShader) = 0;
			shaders.erase(itShader);
		}
	}
	
	
} // namespace sora