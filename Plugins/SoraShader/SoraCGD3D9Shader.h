#ifndef SORA_CGD3D9_SHADER_H_
#define SORA_CGD3D9_SHADER_H_

#include "SoraShader.h"
#include "Cg/cg.h"
#include "Cg/cgD3D9.h"

#ifdef WIN32
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgd3d9.lib")
#endif


namespace sora {

	/*
		SoraShader for D3D9 renderer
		use when USE_OGL_RENDERER is not defined
		see SoraPlatform.h for USE_OGL_RENDERER definition
	 */
	class SoraCGD3D9Shader: public SoraShader {
		friend class SoraShaderContext;
		friend class SoraCGD3D9ShaderContext;

		SoraCGD3D9Shader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context);
		~SoraCGD3D9Shader();
		
	public:
	
		void setTexture(const SoraString& decalName, ulong32 tex);
		bool setParameterfv(const char* name, float32* val, uint32 size);
		bool setParameteriv(const char* name, int32* val, uint32 size);

	private:
		bool attach();
		bool detach();
	
		enum {
		/* DirectX 9 Vertex Shader */
			DX_VERTEX_SHADER	= CG_PROFILE_VS_2_X,
			/* DirectX 9 Pixel Shader */
			DX_PIXEL_SHADER		= CG_PROFILE_PS_2_X,
		};

		void checkError(CGcontext context);

		CGprofile profile;
		CGprogram program;
		CGcontext context;
		CGparameter textureParam;
		ulong32 textureHandle;
	};

	class SoraCGD3D9ShaderContext: public SoraShaderContext {
	public:
		SoraCGD3D9ShaderContext();
		~SoraCGD3D9ShaderContext();

		SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type);

		CGcontext context;
	};
	

} // namespace sora


#endif // SORA_DX_SHADER_H_