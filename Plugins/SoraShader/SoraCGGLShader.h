#ifndef SORA_CGGL_SHADER_H_
#define SORA_CGGL_SHADER_H_

#include "SoraShader.h"
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
        
        SoraCGGLShader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context);
		~SoraCGGLShader();
        
	public:
		
		bool setParameterfv(const char* name, float32* val, uint32 size);
		bool setParameteriv(const char* name, int32* val, uint32 size);
		
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
	
		CGprofile profile;
		CGprogram program;
		CGcontext context;
	};
	
	class SoraCGGLShaderContext: public SoraShaderContext {
	public:
		SoraCGGLShaderContext();
		~SoraCGGLShaderContext();

        SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type);
		
		CGcontext context;
	};

} // namespace sora


#endif // SORA_OGL_SHADER_H_