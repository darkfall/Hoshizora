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

	SoraShader* SoraCGD3D9ShaderContext::attachShader(const SoraString& file, const SoraString& entry, int32 type) {
		if(err != 0) return false;
		
		SoraShader* shader;
		shader = (SoraShader*)(new SoraCGD3D9Shader(file, entry, type, context));
		
		if(shader->type != 0) {
			shaders.push_back(shader);
			return shader;
		} else {
			delete shader;
			return 0;
		}
	}
	
	SoraCGD3D9Shader::~SoraCGD3D9Shader() {
		cgDestroyProgram(program);
	}

	void SoraCGD3D9Shader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			SoraCore::Instance()->log(std::string("SoraShader: ")+str);
			printf("SoraShader: %s\n", str);
			if(error == CG_COMPILER_ERROR) {
				SoraCore::Instance()->log(std::string("SoraShaderContext: ")+cgGetLastListing(context));
				printf("SoraShaderContext: %s", cgGetLastListing(context));
			}			
			type = 0;
		}
	}
	
	SoraCGD3D9Shader::SoraCGD3D9Shader(const SoraString& file, const SoraString& entry, int32 type, CGcontext context) {
		this->type = type;
		switch (type) {
			case VERTEX_SHADER:		profile = cgD3D9GetLatestVertexProfile(); break;
			case FRAGMENT_SHADER:	profile = cgD3D9GetLatestPixelProfile(); break;
		}
		cgD3D9GetOptimalOptions(profile);
		checkError(context);	
		ulong32 size;
		const char* data = (const char*)SoraCore::Instance()->getResourceFile(s2ws(file), size);
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
			free(sdata);
			free((void*)data);

			checkError(context);
			cgD3D9LoadProgram(program, false, 0);
			checkError(context);
		} else {
			type = 0;
		}	
	}
	
	bool SoraCGD3D9Shader::attach() {
		if(type == 0) return false;
		
		cgD3D9BindProgram(program);
		checkError(context);
		//cgDXEnableProfile(profile);
		
		return type != 0;
	}
	
	bool SoraCGD3D9Shader::detach() {
		if(type == 0) return false;
		
		//cgD3D9DisableProfile(profile);
		cgD3D9UnbindProgram(program);
		checkError(context);
		
		return type != 0;
	}

	bool SoraCGD3D9Shader::setParameterfv(const char* name, float32* fv, uint32 size) {
		if(type == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(type == 0) return false;
		
		switch (size) {
			case 1: cgSetParameter1fv(param, fv); break;
			case 2: cgSetParameter2fv(param, fv); break;
			case 3: cgSetParameter3fv(param, fv); break;
			case 4: cgSetParameter4fv(param, fv); break;
		}
		checkError(context);
		return (type != 0);
	}
	
	bool SoraCGD3D9Shader::setParameteriv(const char* name, int32* fv, uint32 size) {
		if(type == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(type == 0) return false;
		
		switch (size) {
			case 1: cgSetParameter1iv(param, fv); break;
			case 2: cgSetParameter2iv(param, fv); break;
			case 3: cgSetParameter3iv(param, fv); break;
			case 4: cgSetParameter4iv(param, fv); break;
		}
		checkError(context);
		return (type != 0);
	}

} // namespace sora