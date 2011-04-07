#include "SoraCGGLShader.h"
#include "SoraCore.h"
#include "Debug/SoraInternalLogger.h"

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

    SoraShader* SoraCGGLShaderContext::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
        SoraShader* shader = (SoraShader*)(new SoraCGGLShader(file, entry, type, context));
		
		if(shader->getType() == 0) {
			delete shader;
			return NULL;
		}
        return shader;
    }

	SoraCGGLShader::~SoraCGGLShader() {
		cgDestroyProgram(program);
	}
	
	void SoraCGGLShader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			INT_LOG::debugPrintf("SoraShader: %s\n", str);
			if(error == CG_COMPILER_ERROR) {
				INT_LOG::debugPrintf("SoraShaderContext: %s", cgGetLastListing(context));
			}			
			setType(0);
		}
	}

	SoraCGGLShader::SoraCGGLShader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context) {
		setType(type);
		this->context = context;
		
		switch (type) {
			case VERTEX_SHADER: profile = cgGLGetLatestProfile(CG_GL_VERTEX); break;
			case FRAGMENT_SHADER: profile = cgGLGetLatestProfile(CG_GL_FRAGMENT); break;
			default:
				setType(0);
		}
		cgGLGetOptimalOptions(profile);
		checkError(context);
		
		ulong32 size;
		const char* data = (const char*)SoraCore::Instance()->getResourceFile(file, size);
		if(data) {
			program = cgCreateProgram(context,
									  CG_SOURCE,
									  data,
									  profile,
									  entry.c_str(),
									  NULL);
			SORA->freeResourceFile((void*)data);
            
			checkError(context);
			cgGLLoadProgram(program);
		} else {
			setType(0);
		}

		textureParam = 0;
	}

	void SoraCGGLShader::setTexture(const SoraString& decalName, ulong32 tex) {
		textureParam = cgGetNamedParameter(program, decalName.c_str());
        cgGLSetTextureParameter(textureParam, (GLuint)((SoraTexture*)tex)->mTextureID);
	}
	
	bool SoraCGGLShader::attach() {
		cgGLBindProgram(program);
		checkError(context);
		cgGLEnableProfile(profile);
		checkError(context);
		if(textureParam) {
			cgGLEnableTextureParameter(textureParam);
			checkError(context);
		}
		return (getType() != 0);
	}
	
	bool SoraCGGLShader::detach() {
		cgGLDisableProfile(profile);
		checkError(context);
		
		return (getType() != 0);
	}

	bool SoraCGGLShader::setParameterfv(const char* name, float32* fv, uint32 size) {
		if(getType() == 0) return false;
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
	
	bool SoraCGGLShader::setParameteriv(const char* name, int32* fv, uint32 size) {
		if(getType() == 0) return false;
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