/*
 *  rftdSpawnPoint.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "rftdSpawnPoint.h"

#include "rftdMap.h"
#include "SoraEventManager.h"

#include "rftdEnemyObject.h"

namespace rftd {

	
	rftdSpawnPoint::rftdSpawnPoint() {
		bPaused = true;
		bFinished = false;
		
		parentMap = NULL;
		spawnInterval = 0.f;
		
		levelListId = -1;
		endSourceId = -1;
		
		mPointSprite = NULL;
		mRouteSprite = NULL;
		
		endSource = NULL;
		
		spawnEvent = new EnemySpawnEvent(-1);
		
		registerEventFunc(this, &rftdSpawnPoint::onEnemySpawn);
	}
	
	rftdSpawnPoint::~rftdSpawnPoint() {
		sora::SORA_EVENT_MANAGER->unregisterTimerEvent(this);

		if(spawnEvent) {
			delete spawnEvent;
			spawnEvent = NULL;
		}
		if(mPointSprite) {
			delete mPointSprite;
			mPointSprite = NULL;
		}
		if(mRouteSprite) {
			delete mRouteSprite;
			mRouteSprite = NULL;
		}
	}
	
	void rftdSpawnPoint::start() {
		if(parentMap != NULL) {
			bPaused = false;
			bFinished = false;
			
			currEnemySize = 0;
			
			sora::SORA_EVENT_MANAGER->unregisterTimerEvent(this);
			sora::SORA_EVENT_MANAGER->registerTimerEvent(this, spawnEvent, spawnInterval, true);
		}
	}
	
	void rftdSpawnPoint::resume() {
		if(bPaused) {
			bPaused = false;
		}
	}
	
	void rftdSpawnPoint::pause() {
		bPaused = true;
	}
	
	bool rftdSpawnPoint::isFinished() const {
		return bFinished;
	}
	
	void rftdSpawnPoint::setSpawnInterval(float32 time) {
		spawnInterval = time;
	}
	
	float32 rftdSpawnPoint::getSpawnInternval() const {
		return spawnInterval;
	}
	
	void rftdSpawnPoint::setEnemyType(int32 type, int32 maxSize) {
		enemyType = type;
		enemySize = maxSize;
		currEnemySize = 0;
		
		spawnEvent->setType(type);
	}
	
	int32 rftdSpawnPoint::getEnemyType() const {
		return enemyType;
	}
	
	void rftdSpawnPoint::setMovePosQueue(const std::vector<hgeVector>& queue) {
		moveQueue = queue;
	}
	
	void rftdSpawnPoint::setContainer(rftdMapEnemyCont* map) {
		parentMap = map;
	}
	
	rftdMapEnemyCont* rftdSpawnPoint::getContainer() const {
		return parentMap;
	}
	
	void rftdSpawnPoint::onEnemySpawn(const EnemySpawnEvent* event) {
		if(bFinished)
			return;
		
		if(currEnemySize >= enemySize) {
			bFinished = true;
			SpawnFinishEvent finishEvent(this);
			parentMap->handleEvent(&finishEvent);

			return;
		}
		
		if(parentMap != NULL && !bPaused) {
			if(EnemyConfLoader::Instance()->exists(event->getType())) {
				rftdEnemyObject* pNewObject = new rftdEnemyObject;
				if(pNewObject) {
					Json::Value enemyConf = GET_ENEMY(enemyType);
					if(!pNewObject->loadConf(enemyConf)) {
						delete pNewObject;
						return;
					}
					pNewObject->setPosition(getX(), getY());
					for(size_t i=0; i<moveQueue.size(); ++i)
						pNewObject->addRoutePoint(moveQueue[i].x, moveQueue[i].y);
					pNewObject->steerStart();
					pNewObject->setEndSource(endSource);
					parentMap->add(pNewObject);
					
					++currEnemySize;
				}
			}
		}
	}

	bool rftdSpawnPoint::loadConf(Json::Value& val) {
		if(val.isMember("posx") && val.isMember("posy")) {
			setPosition(val["posx"].asInt(), val["posy"].asInt());
		} else
			return false;
		
		if(val.isMember("spawn_point_tex")) {
			mPointSprite = PRODUCE_SPRITE(val["spawn_point_tex"]);
		}
		
		if(val.isMember("spawn_route_tex")) {
			mRouteSprite = PRODUCE_SPRITE(val["spawn_route_tex"]);
		}
	
		Json::Value spawnRoute = val["route"];
		if(spawnRoute.isArray()) {
			moveQueue.clear();

			for(size_t i=0; i<spawnRoute.size(); ++i) {
				std::string info = spawnRoute[i].asString();
				std::vector<std::string> params;
				sora::deliStr(params, info, ',');
				if(params.size() == 2) {
					moveQueue.push_back(hgeVector((float32)atof(params[0].c_str()), (float32)atof(params[1].c_str())));
				}
			}
		} else
			return false;
		
		if(val.isMember("level_list_id"))
			levelListId = val["level_list_id"].asInt();
		else
			return false;
		
		if(val.isMember("end_source_id")) {
			endSourceId = val["end_source_id"].asInt();
			setEndSource(parentMap->getSourceById(endSourceId));
		}
		else
			return false;
		
		return true;
	}
	
	int32 rftdSpawnPoint::getLevelListId() const {
		return levelListId;
	}
	
	int32 rftdSpawnPoint::getEndSourceId() const {
		return endSourceId;
	}
	
	void rftdSpawnPoint::update(float32 dt) {
		if(mPointSprite)
			mPointSprite->update(dt);
	}
	
	void rftdSpawnPoint::render() {
		if(mPointSprite) {
			mPointSprite->render(getX(), getY());
		}
	}
	
	void rftdSpawnPoint::setEndSource(rftdSource* source) {
		endSource = source;
	}
	
	rftdSource* rftdSpawnPoint::getEndSource() const {
		return endSource;
	}
} // namespace rftd