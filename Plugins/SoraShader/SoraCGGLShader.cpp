#include "SoraCGGLShader.h"
#include "SoraCore.h"

namespace sora {

	SoraCGGLShaderContext::SoraCGGLShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);
	}

	SoraCGGLShaderContext::~SoraCGGLShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
	}
	
	SoraShader* SoraCGGLShaderContext::attachShader(const SoraString& file, const SoraString& entry, int32 type) {
		if(err != 0) return false;
		
		SoraShader* shader;
		shader = (SoraShader*)(new SoraCGGLShader(file, entry, type, context));
		
		if(shader->type != 0) {
			shaders.push_back(shader);
			return shader;
		} else {
			delete shader;
			return 0;
		}
	}

	SoraCGGLShader::~SoraCGGLShader() {
		cgDestroyProgram(program);
	}
	
	void SoraCGGLShader::checkError(CGcontext context) {
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
	

	SoraCGGLShader::SoraCGGLShader(const SoraString& file, const SoraString& entry, int32 type, CGcontext context) {
		this->type = type;
		this->context = context;
		
		switch (type) {
			case VERTEX_SHADER: profile = cgGLGetLatestProfile(CG_GL_VERTEX); break;
			case FRAGMENT_SHADER: profile = cgGLGetLatestProfile(CG_GL_FRAGMENT); break;
			default:
				this->type = 0;
		}
		cgGLGetOptimalOptions(profile);
		checkError(context);
		
		/*
		program = cgCreateProgramFromFile(context,
										  CG_SOURCE,
										  file.c_str(),
										  profile,
										  entry.c_str(),
										  NULL);
		 */
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
			cgGLLoadProgram(program);
		} else {
			type = 0;
		}
	}
	
	bool SoraCGGLShader::attach() {
		cgGLBindProgram(program);
		checkError(context);
		cgGLEnableProfile(profile);
		checkError(context);

		return (type != 0);
	}
	
	bool SoraCGGLShader::detach() {
		cgGLDisableProfile(profile);
		checkError(context);
		
		return (type != 0);
	}

	bool SoraCGGLShader::setParameterfv(const char* name, float32* fv, uint32 size) {
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
	
	bool SoraCGGLShader::setParameteriv(const char* name, int32* fv, uint32 size) {
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