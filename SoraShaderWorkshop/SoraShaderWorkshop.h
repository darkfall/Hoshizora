/*
 *  SoraShaderWorkshop.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SHADER_WORKSHOP_H_
#define SORA_SHADER_WORKSHOP_H_

#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraFileChangeEvent.h"

#include "json/json.h"

namespace sora {
	
	// to do
	// soraluacanvas
	
	class SoraShaderWorkshopLoader: public SoraSingleton<SoraShaderWorkshopLoader> {
		friend class SoraShaderWorkshopMainWindow;
		friend class SoraSingleton<SoraShaderWorkshopLoader>;
		
		SoraShaderWorkshopLoader() {}
		~SoraShaderWorkshopLoader() {}
		
	public:
		bool loadConfig(const SoraWString& configPath);
		
	private:
		void readShaders(const Json::Value& val);
		void readShaderSprite(const Json::Value& val);
		void readWindowInfo(const Json::Value& val);
		
		typedef struct {
			SoraString paramName;
			SoraString paramType;
			int32 paramSize;
		} ShaderParamInfo;
		
		typedef struct {
			SoraWString shaderPath;
			int32 shaderType;
			SoraString shaderEntry;
		
			typedef std::vector<ShaderParamInfo> SHADER_PARAM_CONT;
			SHADER_PARAM_CONT shaderParams;
		} ShaderInfo;
		
		typedef std::vector<ShaderInfo> SHADER_LIST;
		SHADER_LIST mShaders;
		
		SoraWString mSprite;
		
		int32 mScreenWidth, mScreenHeight;
	};
	
	class SoraShaderWorkshopMainWindow: public SoraWindowInfoBase {
	public:
		SoraShaderWorkshopMainWindow();
		~SoraShaderWorkshopMainWindow();
		
		void onFileChangeEvent(SoraFileChangeEvent* event);
		void onConsoleEvent(SoraConsoleEvent* event);
		void onMenuBarEvent(SoraMenuBarClickEvent* event);
		
		bool updateFunc();
		bool renderFunc();
		
		void init();
		
		void reloadConfig();
		void reloadShader();
		
		int32 getWindowWidth() { return SoraShaderWorkshopLoader::Instance()->mScreenWidth; }
		int32 getWindowHeight() { return SoraShaderWorkshopLoader::Instance()->mScreenHeight; }
		
		int32 getWindowPosX() { return 0; }
		int32 getWindowPosY() { return 0; }
		
		SoraString getWindowName()  { return "SoraShaderWorkshop"; }
		SoraString getWindowId()  { return "MainWindow"; }
		
		bool isWindowed() { return true; }
		bool hideMouse() { return false; }
		
	private:
		SoraShaderContext* mShaderContext;
		SoraSprite* mShaderSprite;
	};
	
} // namespace sora

#endif // SORA_SHADER_WORKSHOP_H_