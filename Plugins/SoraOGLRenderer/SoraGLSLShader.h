/*
 *  SoraGLSLShader.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GLSL_SHADER_H_
#define SORA_GLSL_SHADER_H_

#include "SoraShader.h"

#include "SoraPlatform.h"

#ifdef WIN32

#include <GL/GL.h>
#include <GL/GLU.h>

#elif defined(OS_OSX)

#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>

#elif defined(OS_LINUX)

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#else defined(OS_IOS)

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

namespace sora {
	
	/**
	 *	Implemention for GLSL Shader support
	 *  Note that glsl not use entry function but main
	 */
	
	class SoraGLSLShaderContext: public SoraShaderContext {
	public:
		SoraGLSLShaderContext();
		~SoraGLSLShaderContext();
		
        SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type);
		
		virtual bool attachShaderList();
		virtual bool detachShaderList();
		
		bool isAttached() const;
        GLuint getProgram() const { return mProgram; }
				
	private:
		GLuint mProgram;
		
		bool mAttached;
	};
	
	class SoraGLSLShader: public SoraShader {
	protected:
		friend class SoraGLSLShaderContext;
		
		SoraGLSLShader();
		~SoraGLSLShader();
		
		bool loadShader(const SoraWString& file, const SoraString& entry, int32 type);
		
	public:
		
		void setTexture(const SoraString& decalName, ulong32 tex);
		bool setParameterfv(const char* name, float32* val, uint32 size);
		bool setParameteriv(const char* name, int32* val, uint32 size);
        bool getParameterfv(const char* name, float32* val, uint32 size);
        bool getParameteriv(const char* name, int32* val, uint32 size);             
        
        
	private:
		bool attach();
		bool detach();
		
		GLuint mShader;
		GLuint mProgram;
		
		std::string mTexture1Name;
		ulong32 mTexture1;
		
		void printProgramError();
		void printShaderError();
		
		SoraGLSLShaderContext* mContext;
	};
	
} // namespace sora


#endif // SORA_GLSL_SHADER_H_