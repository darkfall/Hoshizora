#include "SoraCGGLShader.h"
#include "SoraCore.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {

	SoraCGGLShaderContext::SoraCGGLShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);
        
        vertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
        fragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
        cgGLGetOptimalOptions(fragmentProfile);
        cgGLGetOptimalOptions(vertexProfile);
	}

	SoraCGGLShaderContext::~SoraCGGLShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
    }

    SoraShader* SoraCGGLShaderContext::createShader(const SoraWString& file, const SoraString& entry, int32 type) {
        SoraShader* shader;
        switch (type) {
			case VERTEX_SHADER: {
                shader = (SoraShader*)(new SoraCGGLShader(file, entry, type, context, vertexProfile));
                break;
            }
			case FRAGMENT_SHADER: {
                shader = (SoraShader*)(new SoraCGGLShader(file, entry, type, context, fragmentProfile));
                break;
            }
		}
		
		if(!shader || shader->getType() == 0) {
			if(shader) delete shader;
			return NULL;
		}
        return shader;
    }

	SoraCGGLShader::~SoraCGGLShader() {
        if(textureParam)
            cgDestroyParameter(textureParam);
		cgDestroyProgram(program);
	}
	
	void SoraCGGLShader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			DebugPtr->log(vamssg("SoraShader: %s\n", str),
					   LOG_LEVEL_ERROR);
			if(error == CG_COMPILER_ERROR) {
				DebugPtr->log(vamssg("SoraShaderContext: %s", cgGetLastListing(context)),
							  LOG_LEVEL_ERROR);
			}			
			setType(0);
		}
	}

	SoraCGGLShader::SoraCGGLShader(const SoraWString& file, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
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
	
	bool SoraCGGLShader::setParameteriv(const char* name, int32* fv, uint32 size) {
		if(getType() == 0) return false;
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
    
    bool SoraCGGLShader::getParameterfv(const char* name, float32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValuefc(param, size, val);
		return true;
    }
    
    bool SoraCGGLShader::getParameteriv(const char* name, int32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValueic(param, size, val);
		return true;
    }

} // namespace sora