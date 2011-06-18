#include "SoraCGD3D9Shader.h"
#include "SoraCore.h"

#ifdef USE_SHADER

namespace sora {

	SoraCGD3D9ShaderContext::SoraCGD3D9ShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);

		cgD3D9SetDevice((IDirect3DDevice9*)(SoraCore::Instance()->getVideoDeviceHandle()));
        
        vertexProfile = CG_PROFILE_VS_2_X;
        fragmentProfile = CG_PROFILE_PS_2_X;
		cgD3D9GetOptimalOptions(vertexProfile);
		cgD3D9GetOptimalOptions(fragmentProfile);
	}

	SoraCGD3D9ShaderContext::~SoraCGD3D9ShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
	}

	SoraShader* SoraCGD3D9ShaderContext::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
		SoraShader* shader;
        switch(type) {
            case FRAGMENT_SHADER: {
                shader = new SoraCGD3D9Shader(file, entry, type, context, fragmentProfile);
                break;
            }
            case VERTEX_SHADER: {
                shader = new SoraCGD3D9Shader(file, entry, type, context, vertexProfile);
                break;
            }
        }
        
		if(!shader || shader->getType() == 0) {
			if(shader) delete shader;
			return 0;
		}
		return shader;
	}
	
	SoraCGD3D9Shader::~SoraCGD3D9Shader() {
		cgDestroyProgram(program);
		if(textureParam)
			cgDestroyParameter(textureParam);
	}

	void SoraCGD3D9Shader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			SoraCore::Instance()->log(std::string("SoraShader: ")+str, LOG_LEVEL_ERROR);
			if(error == CG_COMPILER_ERROR) {
				SoraCore::Instance()->log(std::string("SoraShaderContext: ")+cgGetLastListing(context), LOG_LEVEL_ERROR);
			}			
			setType(0);
		}
	}
	
	SoraCGD3D9Shader::SoraCGD3D9Shader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
		setType(type);
		
		this->context = context;
        this->profile = profile;

		ulong32 size;
		const char* data = (const char*)SoraCore::Instance()->getResourceFile(file, size);
		if(data) {
			program = cgCreateProgram(context,
									  CG_SOURCE,
									  data,
									  profile,
									  entry.c_str(),
									  NULL);
			sora::SORA->freeResourceFile((void*)data);
			
			checkError(context);
			cgD3D9LoadProgram(program, true, 0);
			checkError(context);
		} else {
			setType(0);
		}	

		textureParam = 0;
	}
	
	void SoraCGD3D9Shader::setTexture(const SoraString& decalName, ulong32 tex) {
		textureParam = cgGetNamedParameter(program, decalName.c_str());
		cgD3D9SetTexture(textureParam, (LPDIRECT3DTEXTURE9)(((SoraTexture*)tex)->mTextureID));
	}

	bool SoraCGD3D9Shader::attach() {
		
		cgD3D9BindProgram(program);
		checkError(context);
      //  if(textureParam) {
       //     cgD3D9SetTextureParameter(textureParam, (IDirect3DBaseTexture9*)(((SoraTexture*)textureHandle)->mTextureID));
       // }
		
		//cgDXEnableProfile(profile);
		
		return getType() != 0;
	}
	
	bool SoraCGD3D9Shader::detach() {
		//cgD3D9DisableProfile(profile);
		cgD3D9UnbindProgram(program);
		checkError(context);
		
		return getType() != 0;
	}

	bool SoraCGD3D9Shader::setParameterfv(const char* name, float32* fv, uint32 size) {
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
		
		switch (size) {
            case 0: break;
			case 1: cgSetParameter1fv(param, fv); break;
			case 2: cgSetParameter2fv(param, fv); break;
			case 3: cgSetParameter3fv(param, fv); break;
			case 4: cgSetParameter4fv(param, fv); break;
            default:
                cgSetParameterValuefc(param, size, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}
	
	bool SoraCGD3D9Shader::setParameteriv(const char* name, int32* fv, uint32 size) {
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
		
		switch (size) {
            case 0: break;
			case 1: cgSetParameter1iv(param, fv); break;
			case 2: cgSetParameter2iv(param, fv); break;
			case 3: cgSetParameter3iv(param, fv); break;
			case 4: cgSetParameter4iv(param, fv); break;
            default:
                cgSetParameterValueic(param, size, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}
    
    bool SoraCGD3D9Shader::getParameterfv(const char* name, float32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValuefc(param, size, val);
		return true;
    }
    
    bool SoraCGD3D9Shader::getParameteriv(const char* name, int32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValueic(param, size, val);
		return true;
    }

} // namespace sora

#endif // USE_SHADER