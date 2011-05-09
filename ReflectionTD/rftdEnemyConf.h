/*
 *  rftdEnemyConf.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_ENEMY_CONF_H_
#define RFTD_ENEMY_CONF_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "hgerect.h"

#include "json/json.h"

namespace rftd {
	
	typedef struct {
		hgeRect mTextureRect;
		
		// ref to obj color
		uint32 mColorId;
		uint32 mEnemyId;
		uint32 mTextureId;
		
		uint32 mType;
		uint32 mEffector;
		
		uint32 mHealth;
		uint32 mDefense;
		float32 mMoveSpeed;
	} EnemyConf;
	
	class EnemyConfLoader: public sora::SoraSingleton<EnemyConfLoader> {
		friend class sora::SoraSingleton<EnemyConfLoader>;
		
		std::map<uint32/*id*/, Json::Value> ENEMY_CONF_MAP;

	public:
		void clear() {
			ENEMY_CONF_MAP.clear();
		}
		
		Json::Value operator[](uint32 id) {
			std::map<uint32, Json::Value>::iterator itConf = ENEMY_CONF_MAP.find(id);
			if(itConf != ENEMY_CONF_MAP.end())
				return itConf->second;
			else
				sora::SORA->messageBox("Error getting enemy conf", "Error", MB_OK | MB_ICONERROR);
			return Json::Value();
		}
		
		Json::Value get(uint32 id) {
			std::map<uint32, Json::Value>::iterator itConf = ENEMY_CONF_MAP.find(id);
			if(itConf != ENEMY_CONF_MAP.end())
				return itConf->second;
			else
				sora::SORA->messageBox("Error getting enemy conf", "Error", MB_OK | MB_ICONERROR);
			return Json::Value();
		}
		
		bool exists(uint32 id) {
			std::map<uint32, Json::Value>::iterator itConf = ENEMY_CONF_MAP.find(id);
			if(itConf != ENEMY_CONF_MAP.end())
				return true;
			
			return false;
		}
		
		void readEnemyConf(const std::wstring& confPath) {
			ulong32 size;
			void* data = sora::SORA->getResourceFile(confPath, size);
			if(data) {
				Json::Reader reader;
				Json::Value root;
				if(reader.parse((const char*)data, (const char*)data+size, root)) {
					Json::Value enemyArray = root["enemies"];
					if(enemyArray.isArray()) {
						for(size_t i=0; i<enemyArray.size(); ++i) {
							int32 eid;
							if(enemyArray[i].isMember("id"))
								eid = enemyArray[i]["id"].asInt();
							else
								continue;
							
							ENEMY_CONF_MAP[eid] = enemyArray[i];
						}
					}
				} else {
					sora::SORA->messageBox(reader.getFormatedErrorMessages(), "Json Error", MB_OK | MB_ICONERROR);
				}
				
				
				sora::SORA->freeResourceFile(data);
			}
		}			
	};
	
#define GET_ENEMY(id) EnemyConfLoader::Instance()->get(id)


} // namespace rftd

#endif // RFTD_ENEMY_CONF_H_