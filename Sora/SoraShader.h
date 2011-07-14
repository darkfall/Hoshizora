/*
 *  SoraShader.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/21/10.
 *  Copyright 2010 GameMaster&. All rights reserved.
 *
 */


#ifndef SORA_SHADER_H__
#define SORA_SHADER_H__

#include "SoraPlatform.h"
#include "AutoContainer.h"
#include "SoraNamedObject.h"

#include <list>

namespace sora {
	
	typedef enum {
		/* vertex shader */
		VERTEX_SHADER = 1,
		/* fragment shader */
		FRAGMENT_SHADER = 2,
	} SORA_SHADER_TYPE;
    
    class SoraShaderContext;
	
	/* 
		Class to hold a shader
		Automatically created and managed by SoraShaderContext
	*/
	class SORA_API SoraShader: public SoraNamedObject, public AutoListElement<SoraShader> {
		friend class SoraShaderContext;
		
	public:
        SoraShader();
		virtual ~SoraShader();
		
		
		/*
			Set a parameterf
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array
			@retval, succeed or not
		 */
		virtual bool setParameterfv(const char* name, float32* val, uint32 size) = 0;
		/*
			Set a parameteri
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array
			@retval, succeed or not
		 */
		virtual bool setParameteriv(const char* name, int32* val, uint32 size) = 0;
        
        virtual bool getParameterfv(const char* name, float32* val, uint32 size) = 0;
        virtual bool getParameteriv(const char* name, int32* val, uint32 size) = 0;
		
		bool setParameter1f(const char* name, float32 v1);
		bool setParameter2f(const char* name, float32 v1, float32 v2);
		bool setParameter3f(const char* name, float32 v1, float32 v2, float32 v3);
		bool setParameter4f(const char* name, float32 v1, float32 v2, float32 v3, float32 v4);

		bool setParameter1i(const char* name, int32 v1);
		bool setParameter2i(const char* name, int32 v1, int32 v2);
		bool setParameter3i(const char* name, int32 v1, int32 v2, int32 v3);
		bool setParameter4i(const char* name, int32 v1, int32 v2, int32 v3, int32 v4);
		
        /*
         add a sample texture
         may use as TEX1 in shader
         */
		virtual void setTexture(const SoraString& decalName, ulong32 tex) = 0;

		/*
			Get the type of the shader
			See SORA_SHADER_TYPE
		 */
		int32 getType() const;
        void setType(int32 t);
		
		void setInternal(bool flag);
        
        SoraShaderContext* getShaderContext() const;

	protected:
        bool bInternal;
		/* 
		 Attach the shader to render
		 Applies to the rendering after attach
		 Would clear when endFrame (if not detach before endFrame)
		 */
		virtual bool attach() = 0;
		/*
		 Detach the shader from render
		 */
		virtual bool detach() = 0;
        
        uint32 mType;
        SoraShaderContext* mShaderContext;
	};
	
	class SORA_API SoraShaderContext {
	public:
        typedef std::list<SoraShader*> ShaderList;

		SoraShaderContext();
		virtual ~SoraShaderContext();
		
		void clear();
        
        /**
         * create a shader from context, not attach
         * @param file, the path of the shader file to attach
         * @param entry, entry function of the shader
         * @param type, the type of the shader, see SORA_SHADER_TYPE
         * @retval, the handle to the attached shader, is 0 if attach failed
		 */
        virtual SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type) = 0;
        
		/**
         * attach a shader to context
         * @param file, the path of the shader file to attach
         * @param entry, entry function of the shader
         * @param type, the type of the shader, see SORA_SHADER_TYPE
         * @retval, the handle to the attached shader, is 0 if attach failed
		 */
        SoraShader* attachShader(const SoraWString& file, const SoraString& entry, int32 type);
                
        /**
         *   get shader list
         **/
        const ShaderList& getShaders() const;
        
        void attachShader(SoraShader* shader);
		/**
		 *	detach a shader from context
		 *	@param shader, the handle to the shader, the retval of attachShader
		 **/
		void detachShader(SoraShader* shader);

		/**
         * attach all shaders in context to render
         * @retval, succeed or not
		 */
		void attachToRender();
		/**
         * detach all shaders in context from render
         * @retval, succeed or not
		 */
		void detachFromRender();
        
        void	setError(int32 error);
        int32	getError() const;
        
        uint32 size() const;
        
		/*
		called by render system
		DO NOT CALL THESE unless you know what you are doing
		*/
		virtual bool attachShaderList();
		virtual bool detachShaderList();
		
	protected:
		inline ShaderList::iterator getShaderIterator(SoraShader* shader);
		
		ShaderList mShaders;
		
		int32 err;
	};
    
    static void FreeShader(SoraShader* shader) {
        SoraShaderContext* context = shader->getShaderContext();
        if(!context)
            THROW_SORA_EXCEPTION("Caught wild shader without ShaderContext, possible bug");
        else context->detachShader(shader);
    }

} // namespace sora


#endif