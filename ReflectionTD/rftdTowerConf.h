/*
 *  rftdTowerConf.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_TOWER_CONF_H_
#define RFTD_TOWER_CONF_H_

#include "SoraCore.h"
#include "stringId.h"
#include "rftdBulletEffector.h"

#include "json/json.h"

namespace rftd {
	
	const int BULLET_TARGET_TYPE_HOMING = 1;
	const int BULLET_TARGET_TYPE_ABSOLUTE = 2;
	const int BULLET_TARGET_TYPE_INSTANT = 3; // laser
	
	typedef struct {
		float32 attack_interval;
		
		float32 bullet_dgr_var;
		float32 bullet_speed;
		
		int32 bullet_target_type;
		int32 bullet_number;
		
		int32 bullet_color;
		int32 bullet_max_reflection;
		
		int32 bullet_id;
		int32 bullet_max_target;
		
		int32 bullet_damage;
		
		std::vector<BulletEffectorConf> bullet_effector_list;
	} TowerLevelConf;
	
	typedef struct {
		std::string name;
		std::string anmPath;
		
		int32 tower_id;
		
		std::vector<TowerLevelConf> tower_level_conf;
	} TowerConf;
	
	class TowerConfLoader: public sora::SoraSingleton<TowerConfLoader> {
		friend class sora::SoraSingleton<TowerConfLoader>;
		
		sora::hash_map<int32, TowerConf> TOWER_CONF_MAP;
		
	public:
		TowerConf get(int32 id) {
			sora::hash_map<int32, TowerConf>::iterator itConf = TOWER_CONF_MAP.find(id);
			if(itConf != TOWER_CONF_MAP.end())
				return itConf->second;
			else {
				sora::SORA->messageBox("Error getting tower conf", "Error", MB_OK | MB_ICONERROR);
			}
			return TowerConf();
		}
		
		BulletEffectorConf readEffectorConf(const std::string& effString) {		
			std::vector<std::string> effParams;
			sora::deliStr(effParams, effString, ',');
			BulletEffectorConf effector;
			
			if(effParams.size() != 0) {
				effector.name = effParams[0];
				
				for(size_t i=1; i<effParams.size(); ++i)
					effector.param_list.push_back((float32)atof(effParams[i].c_str()));
			}
			
			return effector;
		}
		
		TowerLevelConf readTowerLevelConf(Json::Value& val) {
			TowerLevelConf conf;
			if(val.isMember("attack_interval"))
				conf.attack_interval = val["attack_interval"].asDouble();
			
			if(val.isMember("bullet_target_type")) {
				std::string type = val["bullet_target_type"].asString();
				if(strcmpnocase("homing", type.c_str()) == 0)
					conf.bullet_target_type = BULLET_TARGET_TYPE_HOMING;
				else if(strcmpnocase("absolute", type.c_str()) == 0)
					conf.bullet_target_type = BULLET_TARGET_TYPE_ABSOLUTE;
				else if(strcmpnocase("instant", type.c_str()) == 0)
					conf.bullet_target_type = BULLET_TARGET_TYPE_INSTANT;
				else
					conf.bullet_target_type = -1;
			}
			
			if(val.isMember("bullet_number"))
				conf.bullet_number = val["bullet_number"].asInt();
			
			if(val.isMember("bulet_dgr_var"))
				conf.bullet_number = val["bullet_dgr_var"].asDouble();
			
			if(val.isMember("bullet_color"))
				conf.bullet_number = val["bullet_color"].asInt();
			
			if(val.isMember("bullet_max_reflection"))
				conf.bullet_max_reflection = val["bullet_max_reflection"].asInt();
			
			if(val.isMember("bullet_speed"))
				conf.bullet_speed = val["bullet_speed"].asDouble();
			
			if(val.isMember("bullet_effector")) {
				Json::Value effectorArray = val["bullet_effector"];
				if(effectorArray.isString()) {
					conf.bullet_effector_list.push_back(readEffectorConf(effectorArray.asString()));
				} else {
					for(size_t i=0; i<effectorArray.size(); ++i)
						conf.bullet_effector_list.push_back(readEffectorConf(effectorArray[i].asString()));
				}
			}
			
			if(val.isMember("bullet_damage"))
				conf.bullet_damage = val["bullet_damage"].asInt();
			
			if(val.isMember("bullet_id")) 
				conf.bullet_id = val["bullet_id"].asInt();
			
			if(val.isMember("bullet_max_target"))
				conf.bullet_max_target = val["bullet_max_target"].asInt();
			
			return conf;
		}
		
		TowerConf readTowerConf(Json::Value& val) {
			TowerConf conf;
			if(val.isMember("name")) 
				conf.name = val["name"].asString();
			
			if(val.isMember("tower_animation"))
				conf.anmPath = val["tower_animation"].asString();
			
			if(val.isMember("id"))
				conf.tower_id = val["id"].asInt();
			
			if(val.isMember("levels")) {
				Json::Value levelArray = val["levels"];
				if(!levelArray.isNull() && levelArray.isArray()) {
					for(size_t i=0; i<levelArray.size(); ++i) {
						conf.tower_level_conf.push_back(readTowerLevelConf(levelArray[i]));
					}
				}
			}
			return conf;
		}
		
		void readTowerConf(const std::wstring confPath) {
			Json::Reader reader;
			
			ulong32 size;
			const char* data = (const char*)sora::SORA->getResourceFile(confPath, size);
			if(data) {
				Json::Value root;
				if(reader.parse(data, data+size, root)) {
					Json::Value towerArray = root["towers"];
					if(!towerArray.isNull() && towerArray.isArray()) {
						for(size_t i=0; i<towerArray.size(); ++i) {
							Json::Value tower = towerArray[i];
							if(tower.isMember("id")) {
								int32 tid = tower["id"].asInt();
								
								TOWER_CONF_MAP[tid] = readTowerConf(tower);
							}
						}
					}
				} else {
					sora::SORA->messageBox(reader.getFormatedErrorMessages(), "Json Error", MB_OK | MB_ICONERROR);
				}
				
				
				sora::SORA->freeResourceFile((void*)data);
			}
		}
		
		void readTowerConfInFolder(const std::wstring confPath) {
			std::vector<std::wstring> files;
			sora::SORA->enumFilesInFolder(files, confPath);
			for(size_t i=0; i<files.size(); ++i) {
				printf("reading :%s\n", sora::ws2s(files[i]).c_str());
				
				readTowerConf(files[i]);
			}
		}
		
	};
	
#define GET_TOWER(id) TowerConfLoader::Instance()->get(id)

} // namespace rftd

#endif // RFTD_TOWER_CONF_H_