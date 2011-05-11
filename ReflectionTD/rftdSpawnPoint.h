/*
 *  rftdSpawnPoint.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_SPAWN_POINT_H_
#define RFTD_SPAWN_POINT_H_

#include "gcnExtend/gcnDraggableIcon.h"
#include "hgevector.h"

#include "SoraEvent.h"
#include "SoraTimerEvent.h"
#include "SoraSprite.h"

#include "json/json.h"

namespace rftd {
	
	class rftdMapEnemyCont;
	class rftdSource;
	
	class EnemySpawnEvent: public sora::SoraTimerEvent {
	public:
		EnemySpawnEvent(int32 eid): enemyId(eid) {}
		void setType(int32 eid) { enemyId = eid; }
		int32 getType() const { return enemyId; }
		
	private:
		int32 enemyId;
	};
	
	class rftdSpawnPoint;
	
	class SpawnFinishEvent: public sora::SoraEvent {
	public:
		SpawnFinishEvent(rftdSpawnPoint* sp): which(sp) {}

		rftdSpawnPoint* getSpawnPoint() const { return which; }
		
	private:
		rftdSpawnPoint* which;
	};
	
	class rftdSpawnPoint: public gcn::DraggableIcon, public sora::SoraEventHandler {
	public:
		rftdSpawnPoint();
		virtual ~rftdSpawnPoint();
		
		void start();
		void pause();
		void resume();
		
		void setEnemyType(int32 eid, int32 maxsize);
		int32 getEnemyType() const;
		
		void setMovePosQueue(const std::vector<hgeVector>& queue);
		
		void setSpawnInterval(float32 time);
		float32 getSpawnInternval() const;
		
		void setContainer(rftdMapEnemyCont* map);
		rftdMapEnemyCont* getContainer() const;
		
		void onEnemySpawn(const EnemySpawnEvent* event);
		
		bool isFinished() const;
		bool loadConf(Json::Value& val);
		
		int32 getLevelListId() const;
		int32 getEndSourceId() const;
	
		void update(float32 dt);
		void render();
		
		void setEndSource(rftdSource* source);
		rftdSource* getEndSource() const;
		
	private:
		rftdMapEnemyCont* parentMap;
		rftdSource* endSource;
		
		int32 enemyType;
		int32 enemySize;
		int32 currEnemySize;
		
		int32 levelListId;
		int32 endSourceId;
		
		float32 spawnInterval;
		std::vector<hgeVector> moveQueue;
		
		EnemySpawnEvent* spawnEvent;
		
		sora::SoraSprite* mPointSprite;
		sora::SoraSprite* mRouteSprite;
		
		bool bFinished;
		bool bPaused;
	};
}


#endif // RFTD_SPAWN_POINT_H