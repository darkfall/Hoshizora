/*
 *  rftdCommon.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_COMMON_H_
#define RFTD_COMMON_H_

#include "SoraPlatform.h"
#include "SoraFileUtility.h"
#include "SoraColor.h"

#include "SoraLocalizer.h"

#include "rftdTowerConf.h"
#include "rftdObjColor.h"
#include "rftdEnemyConf.h"
#include "rftdTextureConf.h"
#include "rftdMapConf.h"
#include "rftdBulletConf.h"

#include "SoraEnvValues.h"
#include "SoraCore.h"
#include "SoraFont.h"

namespace rftd {
	
	static void rftdInitialize() {
		ulong32 size;
		void* data = sora::SORA->getResourceFile(L"rftdConf.json", size);
		if(data) {
			Json::Value root;
			Json::Reader reader;
			if(reader.parse((const char*)data, (const char*)data+size, root)) {
				if(root.isMember("locale_file_path")) {
					std::string localeFilePath = root["locale_file_path"].asString();
					std::vector<std::wstring> localeFiles;
					sora::SORA->enumFilesInFolder(localeFiles, sora::s2ws(localeFilePath));
					
					for(size_t i=0; i<localeFiles.size(); ++i)
						sora::SoraLocalizer::Instance()->addLocaleConf(localeFiles[i]);
				}
				
				if(root.isMember("locale"))
					sora::SoraLocalizer::Instance()->setCurrentLocale(root["locale"].asString());
				
				if(root.isMember("texture_conf_path")) {
					Json::Value textureConfArray = root["texture_conf_path"];
					if(textureConfArray.isArray()) {
						for(size_t i=0; i<textureConfArray.size(); ++i) {
							TextureConfLoader::Instance()->readTextureConf(sora::s2ws(textureConfArray[i].asString()));
						}
					}
				}
				
				if(root.isMember("color_conf_path")) {
					Json::Value colorConfArray = root["texture_conf_path"];
					if(colorConfArray.isArray()) {
						for(size_t i=0; i<colorConfArray.size(); ++i) {
							ObjColorLoader::Instance()->readObjColorConf(sora::s2ws(colorConfArray[i].asString()));
						}
					}
				}
				
				if(root.isMember("bullet_conf_path")) {
					Json::Value bulletConfArray = root["bullet_conf_path"];
					if(bulletConfArray.isArray()) {
						for(size_t i=0; i<bulletConfArray.size(); ++i) {
							BulletConfLoader::Instance()->readBulletConf(sora::s2ws(bulletConfArray[i].asString()));
						}
					}
				}
				
				if(root.isMember("tower_conf_path")) {
					Json::Value towerConfArray = root["tower_conf_path"];
					if(towerConfArray.isArray()) {
						for(size_t i=0; i<towerConfArray.size(); ++i) {
							std::vector<std::wstring> towerFiles;
							sora::SORA->enumFilesInFolder(towerFiles, sora::s2ws(towerConfArray[i].asString()));
							
							if(towerFiles.size() != 0) {
								for(size_t j=0; j<towerFiles.size(); ++j)
									TowerConfLoader::Instance()->readTowerConf(towerFiles[j]);
							} else
								TowerConfLoader::Instance()->readTowerConf(sora::s2ws(towerConfArray[i].asString()));
						}
					}
				}
				
				if(root.isMember("map_pack_path")) {
					Json::Value mapConfArray = root["map_pack_path"];
					if(mapConfArray.isArray()) {
						for(size_t i=0; i<mapConfArray.size(); ++i) {
							std::wstring mapPackPath = sora::s2ws(mapConfArray[i].asString());
							
							MapPackLoader::Instance()->readMapPack(mapPackPath);
						}
					}
				}
				
				if(root.isMember("global_font")) {
					Json::Value fontVal = root["global_font"];
					if(fontVal.isMember("name") && fontVal.isMember("size")) {
						std::wstring fontName = sora::s2ws(fontVal["name"].asString());
						sora::SoraFont* globalFont;
						try {
							globalFont = sora::SORA->createFont(fontName, fontVal["size"].asInt());
						} catch(sora::SoraException& exp) {
							sora::SORA->messageBoxW(L"Error creating global font: "+fontName, L"Error", MB_OK | MB_ICONERROR);
						}
						if(globalFont)
							sora::SET_ENV_DATA("rftd_global_font", (void*)globalFont);
					} 
				}
				
			} else {
				sora::SORA->messageBox(reader.getFormatedErrorMessages(), "Json Parser Error", MB_OK | MB_ICONERROR);
			}
		}
		
		sora::SET_ENV_INT("rftd_map_gridsize", 8);
	}
	
	
	static void rftdShutdown() {
		MapPackLoader::Destroy();
		EnemyConfLoader::Destroy();
		TowerConfLoader::Destroy();
		BulletConfLoader::Destroy();
		ObjColorLoader::Destroy();
		
		TextureConfLoader::Destroy();
	}
	
} // namespace rftd


#endif // RFTD_COMMON_H_