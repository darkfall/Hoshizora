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
		ShaderList::iterator liShader = shaders.begin();
		while(liShader != shaders.end()) {
			if((*liShader)) {
				delete (*liShader);
				(*liShader) = 0;
				++liShader;
			}
		}
	}
	
	std::list<SoraShader*>::iterator SoraShaderContext::getShader(ulong32 shader) {
		ShaderList::iterator liShader = shaders.begin();
		while(liShader != shaders.end()) {
			if(shader == (ulong32)(*liShader)) {
				return liShader;
			}
			++liShader;
		}
		return shaders.end();
	}
	
	void SoraShaderContext::attachToRender() {
		SoraCore::Instance()->attachShaderContext(this);
	}

	bool SoraShaderContext::attachShaderList() {
		if(err != 0) return false;

		ShaderList::iterator liShader = shaders.begin();
		int32 er = 1;
		
		while(liShader != shaders.end()) {
			(*liShader)->attach();
			if((*liShader)->type == 0) er = 0;
			
			++liShader;
		}
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachFromRender() {
		SoraCore::Instance()->detachShaderContext();
	}

	bool SoraShaderContext::detachShaderList() {
		if(err != 0) return false;

		ShaderList::iterator liShader = shaders.begin();
		int32 er = 1;
		
		while(liShader != shaders.end()) {
			(*liShader)->detach();
			if((*liShader)->type == 0) er = 1;
			
			++liShader;
		}	
		return er==1?true:false;
	}
	
	void SoraShaderContext::detachShader(ulong32 shader) {
		if(err != 0) return;
		
		ShaderList::iterator itShader = getShader(shader);
		if(itShader != shaders.end()) {
			delete (*itShader);
			(*itShader) = 0;
			shaders.erase(itShader);
		}
	}
	
	
} // namespace sora