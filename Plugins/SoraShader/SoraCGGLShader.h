#ifndef SORA_CGGL_SHADER_H_
#define SORA_CGGL_SHADER_H_

#include "SoraShader.h"

#ifdef SORA_USE_SHADER

#include "Cg/cg.h"
#include "Cg/cgGL.h"

#ifdef WIN32
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cggl.lib")
#endif

namespace sora {

	/*
		SoraShader for OpenGL renderer
	 */
	class SoraCGGLShader: public SoraShader {
		friend class SoraShaderContext;
        friend class SoraCGGLShaderContext;
        
        SoraCGGLShader(const util::String& file, const util::String& entry, int32 type, CGcontext context, CGprofile profile);
		~SoraCGGLShader();
        
	public:
		
		void setTexture(const util::String& decalName, ulong32 tex);
		bool setParameterfv(const char* name, float* val, uint32 size);
		bool setParameteriv(const char* name, int32* val, uint32 size);
        bool getParameterfv(const char* name, float* val, uint32 size);
        bool getParameteriv(const char* name, int32* val, uint32 size);
        
	private:
		bool attach();
		bool detach();
		
		/* Shader profiles */
		enum {
			/* OpenGL ARB Extension Vertex Shader */
			OGL_VERTEX_SHADER	= CG_PROFILE_ARBVP1,
			/* OpenGL ARB Extension Fragment Shader */
			OGL_FRAGMENT_SHADER = CG_PROFILE_ARBFP1,
		};

		/*
		 Check for Cg Error
		 Log to core
		*/
		void checkError(CGcontext context);
	
		CGprogram program;
		CGcontext context;
        CGprofile profile;
		CGparameter textureParam;
	};
	
	class SoraCGGLShaderContext: public SoraShaderContext {
	public:
		SoraCGGLShaderContext();
		~SoraCGGLShaderContext();

        SoraShader* createShader(const util::String& file, const util::String& entry, int32 type);
		
		CGcontext context;
        CGprofile fragmentProfile;
        CGprofile vertexProfile;
	};

} // namespace sora

#endif // SORA_USE_SHADER

#endif // SORA_OGL_SHADER_H_