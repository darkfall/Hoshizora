/*
 *  rftdMapEnemyCont.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/11/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdMapEnemyCont.h"

#include "rftdEnemyObject.h"
#include "rftdMap.h"

namespace rftd {
	
	rftdMapEnemyCont::rftdMapEnemyCont(rftdMap* parent): parentMap(parent) {
		registerEventFunc(this, &rftdMapEnemyCont::onSpawnFinishEvent);
		
		currLevel = 0;
		currLevelInternval = 0.f;
		
		state = MAP_STATE_WAITING_FOR_START;
	}
	
	rftdMapEnemyCont::~rftdMapEnemyCont() {
		SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
		while(itSpawnPoint != spawnPoints.end()) {
			if(parentMap)
				parentMap->remove((*itSpawnPoint).spawnPoint);
			
			delete (*itSpawnPoint).spawnPoint;
			(*itSpawnPoint).spawnPoint = NULL;
			++itSpawnPoint;
		}
		
		spawnPoints.clear();
		mSources.clear();
	}
	
	bool rftdMapEnemyCont::readLevelConf(Json::Value& root) {
		Json::Value levelConfList = root["level_list_conf"];
		if(!levelConfList.isNull() && levelConfList.isArray()) {
			for(size_t i=0; i<levelConfList.size(); ++i) {
				parseLevelList(levelConfList[i]);
			}
		}
		
		mSources.clear();
		Json::Value sourceArray = root["sources"];
		if(sourceArray.isArray()) {
			for(size_t i=0; i<sourceArray.size(); ++i) {
				rftdSource* source = new rftdSource;
				if(source->loadConf(sourceArray[i]))
					mSources.push_back(source);
				else
					delete source;
			}
		}
		if(mSources.size() == 0)
			return false;
		
		
		Json::Value spawnPointList = root["spawn_points"];
		if(!spawnPointList.isNull() && spawnPointList.isArray()) {
			for(size_t i=0; i<spawnPointList.size(); ++i) {
				parseSpawnPoint(spawnPointList[i]);
			}
		} else 
			return false;
		
		Json::Value levelIntervals = root["level_interval"];
		if(levelIntervals.isArray())
			parseLevelInterval(levelIntervals);
		
		Json::Value levelNames = root["level_name"];
		if(levelNames.isArray())
			parseLevelName(levelNames);
		
		return true;
	}
	
	void rftdMapEnemyCont::parseSpawnPoint(Json::Value& val) {
		rftdSpawnPoint* spawnPoint = new rftdSpawnPoint;
		if(spawnPoint) {
			spawnPoint->setContainer(this);
			if(!spawnPoint->loadConf(val)) {
				sora::SORA->messageBox("Failed to load spawn point", "Error", MB_OK | MB_ICONERROR);
				delete spawnPoint;
				return;
			}
			
			int32 listId = spawnPoint->getLevelListId();
			if(listId != -1) {
				LEVEL_CONF_MAP::iterator itConf = levelConfs.find(listId);
				if(itConf != levelConfs.end()) {
					SpawnPointConf conf;
					
					conf.spawnPoint = spawnPoint;
					conf.spawnListId = listId;
					conf.totalLevel = itConf->second.totalLevel;
					conf.currLevel = 0;
					conf.endSourceId = spawnPoint->getEndSourceId();
					
					if(parentMap)
						parentMap->add(conf.spawnPoint);
					
					spawnPoints.push_back(conf);
					
					return;
				} 
			}
			
			delete spawnPoint;
		}
	}
	
	void rftdMapEnemyCont::parseLevelList(Json::Value& val) {
		LevelListConf conf;
		if(val.isMember("id")) 
			conf.listId = val["id"].asInt();
		if(val.isMember("total"))
			conf.totalLevel = val["total"].asInt();
		
		Json::Value levels = val["levels"];
		if(!levels.isNull() && levels.isArray()) {
			for(size_t i=0; i<levels.size(); ++i) {
				Json::Value levelVal = levels[i];
				
				LevelConf lconf;
				if(levelVal.isMember("enemyid"))
					lconf.enemyId = levelVal["enemyid"].asInt();
				else
					continue;
				if(levelVal.isMember("amount"))
					lconf.totalEnemy = levelVal["amount"].asInt();
				else
					continue;
				if(levelVal.isMember("interval"))
					lconf.spawnInterval = (float32)levelVal["interval"].asDouble();
				else
					lconf.spawnInterval = 1.0;
				
				conf.levelEnemyList.push_back(lconf);
			}
		}
		
		if(conf.totalLevel != conf.levelEnemyList.size()) {
			for(size_t i=conf.levelEnemyList.size(); i<conf.totalLevel; ++i) {
				LevelConf lconf;
				lconf.enemyId = -1;
				lconf.totalEnemy = -1;
				conf.levelEnemyList.push_back(lconf);
			}
		}
		
		if(conf.listId != -1)
			levelConfs[conf.listId] = conf;
	}
	
	void rftdMapEnemyCont::parseLevelInterval(Json::Value& val) {
		for(size_t i=0; i<val.size(); ++i)
			levelIntervals.push_back((float32)val[i].asDouble());
	}
	
	void rftdMapEnemyCont::parseLevelName(Json::Value& val) {
		for(size_t i=0; i<val.size(); ++i) {
			levelNames.push_back(sora::GET_LOCAL_STR(val[i].asString()));
		}
	}
	
	uint32 rftdMapEnemyCont::update(float32 dt) {
		if(state == MAP_STATE_SPAWN_FINISHED) {
			SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
			spawnPointFinishedNum = 0;
			while(itSpawnPoint != spawnPoints.end()) {
				if((*itSpawnPoint).currLevel < (*itSpawnPoint).totalLevel) {
					++(*itSpawnPoint).currLevel;
				} else {
					++spawnPointFinishedNum;
				}
				++itSpawnPoint;
			}
			
			if(spawnPointFinishedNum == spawnPoints.size()) {
				state = MAP_STATE_MAP_END;
			} else {
				spawnFinishedNum = 0;
				++currLevel;
				start();
			}
			return 0;
		}
		
		
		if(state != MAP_STATE_PAUSED &&
		   state != MAP_STATE_WAITING_FOR_START &&
		   state != MAP_STATE_MAP_END) {
			sora::SoraObject::update(dt);
			SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
			while(itSpawnPoint != spawnPoints.end()) {
				(*itSpawnPoint).spawnPoint->update(dt);
				++itSpawnPoint;
			}
			
			SOURCE_LIST::iterator itSource = mSources.begin();
			while(itSource != mSources.end()) {
				(*itSource)->update(dt);
				++itSource;
			}
		}
		
		if(state == MAP_STATE_SPAWNING) {
			SoraObject* objects = getObjList();
			while(objects != NULL) {
				rftdEnemyObject* enemyObject = static_cast<rftdEnemyObject*>(objects);
				if(enemyObject != NULL && enemyObject->isFinished()) {
					del(objects);
				}
				objects = objects->next();
			}
		} else if(state == MAP_STATE_WAITING_FOR_NEXT_LEVEL) {
			currLevelInternval += dt;
			if(currLevelInternval >= levelIntervals[currLevel]) {
				state = MAP_STATE_SPAWNING;
				startSpawn();
				
				currLevelInternval = levelIntervals[currLevel];
			}
		}
		
		return 0;
	}
	
	void rftdMapEnemyCont::render() {
		sora::SoraObject::render();
		SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
		while(itSpawnPoint != spawnPoints.end()) {
			(*itSpawnPoint).spawnPoint->render();
			++itSpawnPoint;
		}
		SOURCE_LIST::iterator itSource = mSources.begin();
		while(itSource != mSources.end()) {
			(*itSource)->render();
			++itSource;
		}
	}
	
	float32 rftdMapEnemyCont::getCurrLevelInterval() const {
		return currLevelInternval;
	}
	
	float32 rftdMapEnemyCont::getLevelInterval() const {
		if(currLevel < levelIntervals.size())
			return levelIntervals[currLevel];
		return 0.f;
	}
	
	int32 rftdMapEnemyCont::getCurrLevel() const {
		return currLevel;
	}
	
	int32 rftdMapEnemyCont::getEnemySize() const {
		return getObjSize();
	}
	
	std::wstring rftdMapEnemyCont::getCurrLevelName() const {
		if(currLevel < levelNames.size())
			return levelNames[currLevel];
		return L"\0";
	}
	
	bool rftdMapEnemyCont::isMapFinished() const {
		return state == MAP_STATE_MAP_END;
	}
	
	void rftdMapEnemyCont::onSpawnFinishEvent(const SpawnFinishEvent* event) {
		if(state != MAP_STATE_SPAWNING)
			return;
		
		++spawnFinishedNum;
		if(spawnFinishedNum >= spawnPoints.size()) {
			state = MAP_STATE_SPAWN_FINISHED;
		}
	}
	
	void rftdMapEnemyCont::start() {
		if(state == MAP_STATE_PAUSED) {
			SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
			while(itSpawnPoint != spawnPoints.end()) {
				(*itSpawnPoint).spawnPoint->resume();
			}
		}
		
		state = MAP_STATE_WAITING_FOR_NEXT_LEVEL;
		currLevelInternval = 0.f;
		if(levelIntervals.size() == 0 || currLevel >= levelIntervals.size()) {
			state = MAP_STATE_SPAWNING;
			startSpawn();
		}
		spawnFinishedNum = 0;
		spawnPointFinishedNum = 0;
	}
	
	void rftdMapEnemyCont::startSpawn() {
		SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
		while(itSpawnPoint != spawnPoints.end()) {
			if((*itSpawnPoint).currLevel < (*itSpawnPoint).totalLevel) {
				LevelConf conf = levelConfs[(*itSpawnPoint).spawnListId].levelEnemyList[(*itSpawnPoint).currLevel];
				(*itSpawnPoint).spawnPoint->setEnemyType(conf.enemyId, conf.totalEnemy);
				(*itSpawnPoint).spawnPoint->setSpawnInterval(conf.spawnInterval);
				(*itSpawnPoint).spawnPoint->start();
			}
			++itSpawnPoint;
		}
		
	}
	
	void rftdMapEnemyCont::pause() {
		SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
		while(itSpawnPoint != spawnPoints.end()) {
			(*itSpawnPoint).spawnPoint->pause();
			
			++itSpawnPoint;
		}
		
		state = MAP_STATE_PAUSED;
	}
	
	void rftdMapEnemyCont::reset() {
		SPAWN_POINT_LIST::iterator itSpawnPoint = spawnPoints.begin();
		while(itSpawnPoint != spawnPoints.end()) {
			(*itSpawnPoint).currLevel = 0;
			(*itSpawnPoint).spawnPoint->pause();
		}
		
		state = MAP_STATE_WAITING_FOR_START;
	}

	
	rftdSource* rftdMapEnemyCont::getSourceById(int32 sid) const {
		SOURCE_LIST::const_iterator itSource = mSources.begin();
		for(itSource; itSource != mSources.end(); ++itSource) {
			if((*itSource)->getId() == sid)
				return (*itSource);
		}
		return NULL;
	}
	
} // namespace rftd