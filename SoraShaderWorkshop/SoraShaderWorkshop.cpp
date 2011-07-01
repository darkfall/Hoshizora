/*
 *  SoraShaderWorkshop.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraShaderWorkshop.h"

namespace sora {

	bool SoraShaderWorkshopLoader::loadConfig(const SoraWString& path) {
		SoraResourceFileAuto data(path);
		
		if(data.isValid()) {
			Json::Reader reader;
			Json::Value root;
			
			char* cdata = (char*)data;
			if(reader.parse(cdata, cdata+data.getSize(), root)) {
				readShaders(root);
				readShaderSprite(root);
				readWindowInfo(root);
			}
			return true;
		}
		return false;
	}
	
	void SoraShaderWorkshopLoader::readShaders(const Json::Value& val) {
		mShaders.clear();
		
		Json::Value shaderArray = val["Shaders"];
		if(shaderArray.isArray()) {
			for(int i=0; i<shaderArray.size(); ++i) {
				Json::Value shader = shaderArray[i];
				if(shader.isObject()) {
					ShaderInfo info;
					
					if(shader.isMember("path")) {
						info.shaderPath = s2ws(shader["path"].asString());
					}
					
					if(shader.isMember("type")) {
						int type = shader["type"].asInt();
						if(type == i)
							info.shaderType = FRAGMENT_SHADER;
						else
							info.shaderPath = VERTEX_SHADER;
					}
					
					if(shader.isMember("entry")) {
						info.shaderEntry = shader["entry"].asString();
					}
					
					if(shader.isMember("params")) {
						Json::Value paramArray = shader["params"];
						if(paramArray.isArray()) {
							for(int i=0; i<paramArray.size(); ++i) {
								Json::Value param = paramArray[i];
								if(param.isObject()) {
									ShaderParamInfo paramInfo;
									
									if(param.isMember("name")) 
										paramInfo.paramName = param["name"].asString();
									
									if(param.isMember("type"))
										paramInfo.paramType = param["type"].asString();
									
									if(param.isMember("size"))
										paramInfo.paramSize = param["size"].asInt();
									
									info.shaderParams.push_back(paramInfo);
								}
							}
						}
					}
					
					mShaders.push_back(info);
				}
			}
		}
	}
	
	void SoraShaderWorkshopLoader::readShaderSprite(const Json::Value& val) {
		mSprite.clear();
		if(val.isMember("sprite"))
			mSprite = s2ws(val["sprite"].asString());
	}
	
	void SoraShaderWorkshopLoader::readWindowInfo(const Json::Value& val) {
		if(val.isMember("screen_width"))
			mScreenWidth = val["screen_width"].asInt();
		else
			mScreenWidth = 800;
		
		if(val.isMember("screen_height"))
			mScreenHeight = val["screen_height"].asInt();
		else
			mScreenHeight = 600;
	}

	SoraShaderWorkshopMainWindow::SoraShaderWorkshopMainWindow() {
		registerEventFunc(this, &SoraShaderWorkshopMainWindow::onFileChangeEvent);
		registerEventFunc(this, &SoraShaderWorkshopMainWindow::onConsoleEvent);
		registerEventFunc(this, &SoraShaderWorkshopMainWindow::onMenuBarEvent);
		
		//SORA->getConsole()->registerCmdHandler(); 
		
		mShaderContext = SORA->createShaderContext();
	}
	
	SoraShaderWorkshopMainWindow::~SoraShaderWorkshopMainWindow() {
		if(mShaderContext)
			delete mShaderContext;
		
	}
	
	void SoraShaderWorkshopMainWindow::init() {
		reloadConfig();
	}
	
	void SoraShaderWorkshopMainWindow::onFileChangeEvent(SoraFileChangeEvent* event) {
	}
	
	void SoraShaderWorkshopMainWindow::onConsoleEvent(SoraConsoleEvent* event) {
		
	}
	
	void SoraShaderWorkshopMainWindow::onMenuBarEvent(SoraMenuBarClickEvent* event) {
	}
	
	bool SoraShaderWorkshopMainWindow::updateFunc() {
		
		return false;
	}
	
	bool SoraShaderWorkshopMainWindow::renderFunc() {
		SORA->attachShaderContext(mShaderContext);
		mShaderSprite->render();
		SORA->detachShaderContext();
		return false;
	}
	
	void SoraShaderWorkshopMainWindow::reloadConfig() {
		if(SoraShaderWorkshopLoader::Instance()->loadConfig(L"config.json"))
			reloadShader();
	}
	
	void SoraShaderWorkshopMainWindow::reloadShader() {
		SoraShaderWorkshopLoader* loader = SoraShaderWorkshopLoader::Instance();
		
		mShaderContext->clear();
		SoraShaderWorkshopLoader::SHADER_LIST shaders = loader->mShaders;
		SoraShaderWorkshopLoader::SHADER_LIST::iterator itShader = shaders.begin();
		while(itShader != shaders.end()) {
			mShaderContext->attachShader(itShader->shaderPath, itShader->shaderEntry, itShader->shaderType);
			++itShader;
		}
		
		if(mShaderSprite) {
			delete mShaderSprite;
		}
		
		if(loader->mSprite.size() == 0) {
			mShaderSprite = SORA->createSprite(loader->mSprite);
		} else {
			mShaderSprite = new SoraSprite(SORA->createTextureWH(SORA->getScreenWidth(), SORA->getScreenHeight()));
		}
		mShaderSprite->setColor(mShaderSprite->getSpriteWidth()/2, 
								mShaderSprite->getSpriteHeight()/2);
		mShaderSprite->setPosition(SORA->getScreenWidth()/2, 
								   SORA->getScreenHeight()/2);
	}

} // namespace sora