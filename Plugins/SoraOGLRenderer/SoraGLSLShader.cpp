/*
 *  SoraGLSLShader.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraPlatform.h"
#include "SoraGLSLShader.h"
#include "SoraException.h"
#include "SoraResourceFile.h"
#include "SoraTexture.h"

#include "Debug/SoraInternalLogger.h"

#ifdef OS_OSX
#include <OpenGL/gl.h> 
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#endif

namespace sora {
	
	void SoraGLSLShader::printProgramError() {
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infologLength);
		
		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(mProgram, infologLength, &charsWritten, infoLog);
			DebugPtr->log(vamssg("%s\n", infoLog), LOG_LEVEL_ERROR);
			free(infoLog);
		}
		
		mContext = NULL;
	}
	
	void SoraGLSLShader::printShaderError() {
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		
		glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infologLength);
		
		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(mShader, infologLength, &charsWritten, infoLog);
			DebugPtr->log(vamssg("%s\n", infoLog), LOG_LEVEL_ERROR);
			free(infoLog);
		}
	}

	SoraGLSLShader::SoraGLSLShader() {
		mType = 0;
		mShader = 0;
		mProgram = 0;
		
		mTexture1 = 0;
	}
	
	bool SoraGLSLShader::loadShader(const SoraWString& file, const SoraString& entry, int32 _type) {
		assert(mProgram != 0);
        mType = _type;

		if(mType == FRAGMENT_SHADER) 
			mShader = glCreateShader(GL_FRAGMENT_SHADER);
		else
			mShader = glCreateShader(GL_VERTEX_SHADER);
		
		
		if(mShader == 0) {			
			THROW_SORA_EXCEPTION("Error creating glsl shader");
			mType = 0;
			return false;
		}
		
		SoraResourceFileAuto data(file);
		if(data.isValid()) {
			const char* cdata = static_cast<const char*>(data);
			glShaderSource(mShader, 1, &cdata, NULL);
			glCompileShader(mShader);
			
			int success;
            glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
				printShaderError();
				mType = 0;
				
				return false;
			}
		}
		return true;
	}
	
	SoraGLSLShader::~SoraGLSLShader() {
		if(mShader != 0)
			glDeleteShader(mShader);
	}
		
	void SoraGLSLShader::setTexture(const SoraString& decalName, ulong32 tex) {
		mTexture1Name = decalName;
		mTexture1 = ((SoraTexture*)tex)->mTextureID;
	}
	
	bool SoraGLSLShader::setParameterfv(const char* name, float32* val, uint32 size) {
		if(mType == 0)
			return false;
		
		bool requestedShaderUse = false;
		if(!mContext->isAttached()) {
			mContext->attachShaderList();
			requestedShaderUse = true;
		}
		
		GLuint var = glGetUniformLocation(mProgram, name);
		if(var == 0) {
			printProgramError();
			return false;
		}
		
		switch(size) {
			case 1: glUniform1f(var, *val); break;
			case 2: glUniform2f(var, val[0], val[1]); break;
			case 3: glUniform3f(var, val[0], val[1], val[2]); break;
			case 4: glUniform4f(var, val[0], val[1], val[2], val[3]); break;
            case 9: glUniformMatrix3fv(var, 1, false, val); break;
            case 16: glUniformMatrix4fv(var, 1, false, val); break;
			default:
				glUniform1fv(var, static_cast<GLint>(size), val);
				break;
		}
		
		if(requestedShaderUse)
			mContext->detachShaderList();
		
		return true;
	}
	
	bool SoraGLSLShader::setParameteriv(const char* name, int32* val, uint32 size) {
		if(mType == 0)
			return false;
	
		bool requestedShaderUse = false;
		if(!mContext->isAttached()) {
			mContext->attachShaderList();
			requestedShaderUse = true;
		}
		
		GLuint var = glGetUniformLocation(mProgram, name);
		if(var == 0) {
			printProgramError();
			return false;
		}
		
		switch(size) {
			case 1: glUniform1i(var, *val); break;
			case 2: glUniform2i(var, val[0], val[1]); break;
			case 3: glUniform3i(var, val[0], val[1], val[2]); break;
			case 4: glUniform4i(var, val[0], val[1], val[2], val[3]); break;
			default:
				glUniform1iv(var, static_cast<GLint>(size), val);
				break;
		}
		
		if(requestedShaderUse)
			mContext->detachShaderList();
		
		return true;
	}
	
	bool SoraGLSLShader::getParameterfv(const char* name, float32* val, uint32 size) {
		if(mType == 0)
			return false;
		
		bool requestedShaderUse = false;
		if(!mContext->isAttached()) {
			mContext->attachShaderList();
			requestedShaderUse = true;
		}
		
		GLuint var = glGetUniformLocation(mProgram, name);
		if(var == 0) {
			printProgramError();
			return false;
		}
		
		glGetUniformfv(mProgram, var, val);
		
		if(requestedShaderUse)
			mContext->detachShaderList();
        return glGetError() == GL_NO_ERROR;
	}
	
	bool SoraGLSLShader::getParameteriv(const char* name, int32* val, uint32 size) {
		if(mType == 0)
			return false;
		
		bool requestedShaderUse = false;
		if(!mContext->isAttached()) {
			mContext->attachShaderList();
			requestedShaderUse = true;
		}
		
		GLuint var = glGetUniformLocation(mProgram, name);
		if(var == 0) {
			printProgramError();
			return false;
		}
		
		glGetUniformiv(mProgram, var, val);
		
		if(requestedShaderUse)
			mContext->detachShaderList();
        return glGetError() == GL_NO_ERROR;
	}
	
	bool SoraGLSLShader::attach() {
		if(mTexture1 != 0) {
			GLuint tex = glGetUniformLocation(mProgram, mTexture1Name.c_str());
			if(tex) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, mTexture1);
				glUniform1i(tex, 1);
			}
		}
		glAttachShader(mProgram, mShader);
        return glGetError() == GL_NO_ERROR;
	}
	
	bool SoraGLSLShader::detach() {
		glDetachShader(mProgram, mShader);	
        return glGetError() == GL_NO_ERROR;
	}
	
	SoraGLSLShaderContext::SoraGLSLShaderContext() {
		mProgram = glCreateProgram();
	}
	
	SoraGLSLShaderContext::~SoraGLSLShaderContext() {
		if(mProgram != 0)
			glDeleteProgram(mProgram);
	}

	SoraShader* SoraGLSLShaderContext::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
		if(!mProgram)
			return NULL;
		
		SoraGLSLShader* shader = new SoraGLSLShader();
		shader->mProgram = mProgram;
		shader->mContext = this;
        shader->loadShader(file, entry, type);
        return shader;
	}
	
	bool SoraGLSLShaderContext::attachShaderList() {
        if(!mProgram)
            return false;
        
		SoraShaderContext::attachShaderList();
		
		glLinkProgram(mProgram);
		glUseProgram(mProgram);
        
        int success;
        glGetShaderiv(mProgram, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            int infologLength = 0;
            int charsWritten  = 0;
            char *infoLog;
            
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infologLength);
            
            if (infologLength > 0) {
                infoLog = (char *)malloc(infologLength);
                glGetProgramInfoLog(mProgram, infologLength, &charsWritten, infoLog);
                DebugPtr->log(vamssg("%s\n", infoLog), LOG_LEVEL_ERROR);
                free(infoLog);
            }
        }
        
		mAttached = true;
        return glGetError() == GL_NO_ERROR;
	}
	
	bool SoraGLSLShaderContext::detachShaderList() {
        if(!mProgram)
            return false;
        
		SoraShaderContext::detachShaderList();
		
		glUseProgram(0);
		
		mAttached = false;
         return glGetError() == GL_NO_ERROR;
	}
	
	bool SoraGLSLShaderContext::isAttached() const {
		return mAttached;
	}
} // namespace sora