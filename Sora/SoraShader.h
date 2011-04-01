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
#include <list>

namespace sora {
	
	typedef enum {
		/* vertex shader */
		VERTEX_SHADER = 1,
		/* fragment shader */
		FRAGMENT_SHADER = 2,
	} SORA_SHADER_TYPE;
	
	/* 
		Class to hold a shader
		Automatically created and managed by SoraShaderContext
	*/
	class SoraShader {
		friend class SoraShaderContext;
		
	public:
        SoraShader();
		virtual ~SoraShader();
        
		/*
			Set a parameterf
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array, from 1..4
			@retval, succeed or not
		 */
		virtual bool setParameterfv(const char* name, float32* val, uint32 size) = 0;
		/*
			Set a parameteri
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array, from 1..4
			@retval, succeed or not
		 */
		virtual bool setParameteriv(const char* name, int32* val, uint32 size) = 0;
		
		virtual void setTexture(const SoraString& decalName, ulong32 tex) = 0;

		/*
			Get the type of the shader
			See SORA_SHADER_TYPE
		 */
		int32 getType() const { return type; }
        void setType(int32 t) { type = t; };

	private:
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
        
        uint32 type;
	};
	
	class SoraShaderContext {
	public:
        typedef std::list<SoraShader*> ShaderList;

		SoraShaderContext();
		virtual ~SoraShaderContext();
		
        /*
         create a shader from context, not attach
         @param file, the path of the shader file to attach
         @param entry, entry function of the shader
         @param type, the type of the shader, see SORA_SHADER_TYPE
         @retval, the handle to the attached shader, is 0 if attach failed
		 */
        virtual SoraShader* createShader(const SoraWString& file, const SoraString& entry, int32 type) = 0;
        
		/*
			attach a shader to context
			@param file, the path of the shader file to attach
			@param entry, entry function of the shader
			@param type, the type of the shader, see SORA_SHADER_TYPE
			@retval, the handle to the attached shader, is 0 if attach failed
		 */
        SoraShader* attachShader(const SoraWString& file, const SoraString& entry, int32 type);
        
        
        void attachShader(SoraShader* shader) { assert(shader != NULL); shaders.push_back(shader); }
		/*
			detach a shader from context
			@param shader, the handle to the shader, the retval of attachShader
		 */
		void	detachShader(SoraShader* shader);

		/*
			attach all shaders in context to render
			@retval, succeed or not
		 */
		void	attachToRender();
		/*
			detach all shaders in context from render
			@retval, succeed or not
		 */
		void	detachFromRender();


		/*
		called by render system
		DO NOT CALL THIS
		*/
		bool	attachShaderList();
		bool	detachShaderList();
        
        void    setError(int32 error) { err = error; }
        int32   getError() const      { return err; }
        
        uint32   getShaderSize() const { return shaders.size(); }
        
        const ShaderList& getShaderList()  { return shaders; }
		
	private:
		inline ShaderList::iterator getShaderIterator(SoraShader* shader);
		
		ShaderList shaders;
		
		int32 err;
	};

} // namespace sora


#endif