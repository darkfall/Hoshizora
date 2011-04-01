#include "SoraCGD3D9Shader.h"
#include "SoraCore.h"

namespace sora {

	SoraCGD3D9ShaderContext::SoraCGD3D9ShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);

		cgD3D9SetDevice((IDirect3DDevice9*)(SoraCore::Instance()->getVideoDeviceHandle()));
	}

	SoraCGD3D9ShaderContext::~SoraCGD3D9ShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
	}

	SoraShader* SoraCGD3D9ShaderContext::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
		SoraShader* shader = (SoraShader*)(new SoraCGD3D9Shader(file, entry, type, context));
		
		if(shader->getType() == 0) {
			delete shader;
			return 0;
		}
		return shader;
	}
	
	SoraCGD3D9Shader::~SoraCGD3D9Shader() {
		cgDestroyProgram(program);
	}

	void SoraCGD3D9Shader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			SoraCore::Instance()->log(std::string("SoraShader: ")+str);
			if(error == CG_COMPILER_ERROR) {
				SoraCore::Instance()->log(std::string("SoraShaderContext: ")+cgGetLastListing(context));
			}			
			setType(0);
		}
	}
	
	SoraCGD3D9Shader::SoraCGD3D9Shader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context) {
		setType(type);
		switch (type) {
			case VERTEX_SHADER:		profile = cgD3D9GetLatestVertexProfile(); break;
			case FRAGMENT_SHADER:	profile = cgD3D9GetLatestPixelProfile(); break;
			default:
				setType(0);
		}
		cgD3D9GetOptimalOptions(profile);
		checkError(context);	

		ulong32 size;
		const char* data = (const char*)SoraCore::Instance()->getResourceFile(file, size);
		if(data) {
			char* sdata = (char*)malloc(size+1);
			memcpy(sdata, data, size);
			sdata[size] = '\0';
			program = cgCreateProgram(context,
									  CG_SOURCE,
									  sdata,
									  profile,
									  entry.c_str(),
									  NULL);
			sora::SORA->freeResourceFile((void*)data);
			free(sdata);
			
			checkError(context);
			cgD3D9LoadProgram(program, false, 0);
			checkError(context);
		} else {
			setType(0);
		}	

		textureParam = 0;
		textureHandle = 0;
	}
	
	void SoraCGD3D9Shader::setTexture(const SoraString& decalName, ulong32 tex) {
		textureParam = cgGetNamedParameter(program, decalName.c_str());
		textureHandle = tex;
		checkError(context);
	}

	bool SoraCGD3D9Shader::attach() {
		cgD3D9BindProgram(program);
		checkError(context);
		
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
			case 1: cgSetParameter1fv(param, fv); break;
			case 2: cgSetParameter2fv(param, fv); break;
			case 3: cgSetParameter3fv(param, fv); break;
			case 4: cgSetParameter4fv(param, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}
	
	bool SoraCGD3D9Shader::setParameteriv(const char* name, int32* fv, uint32 size) {
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
		
		switch (size) {
			case 1: cgSetParameter1iv(param, fv); break;
			case 2: cgSetParameter2iv(param, fv); break;
			case 3: cgSetParameter3iv(param, fv); break;
			case 4: cgSetParameter4iv(param, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}

} // namespace sora