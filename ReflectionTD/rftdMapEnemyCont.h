/*
 *  rftdMapEnemyCont.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/11/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_MAP_ENEMY_CONT_H_
#define RFTD_MAP_ENEMY_CONT_H_

#include "rftdCommon.h"
#include "rftdSpawnPoint.h"
#include "rftdSource.h"

namespace rftd {
	
	enum {
		MAP_STATE_SPAWNING = 1,
		MAP_STATE_PAUSED,
		MAP_STATE_WAITING_FOR_NEXT_LEVEL,
		MAP_STATE_WAITING_FOR_START,
		MAP_STATE_SPAWN_FINISHED,
		MAP_STATE_MAP_END,
	};
	
	class rftdMap;
	
	class rftdMapEnemyCont: public sora::SoraObject {
	public:
		rftdMapEnemyCont(rftdMap* parent);
		virtual ~rftdMapEnemyCont();
		
		bool readLevelConf(Json::Value& val);
		
		void start();
		void pause();
		void reset();
		
		virtual uint32 update(float32 dt);
		virtual void render();
		
		int32 getEnemySize() const;
		int32 getCurrLevel() const;
		float32 getCurrLevelInterval() const;
		float32 getLevelInterval() const;
		std::wstring getCurrLevelName() const;
		bool isMapFinished() const;
		
		rftdSource* getSourceById(int32 sid) const ;
		
	private:
		void startSpawn();
		
		inline void parseSpawnPoint(Json::Value& val);
		inline void parseLevelList(Json::Value& val);
		inline void parseLevelInterval(Json::Value& val);
		inline void parseLevelName(Json::Value& val);
		
		void onSpawnFinishEvent(const SpawnFinishEvent* event);
		
		typedef struct tagSpawnPointConf {
			rftdSpawnPoint* spawnPoint;
			int32 spawnListId;
			int32 endSourceId;
			
			int32 currLevel;
			int32 totalLevel;
			
			tagSpawnPointConf(): currLevel(0), totalLevel(0), spawnListId(0), spawnPoint(NULL) {}
		} SpawnPointConf;
		
		typedef std::vector<SpawnPointConf> SPAWN_POINT_LIST;
		SPAWN_POINT_LIST spawnPoints;
		
		int32 spawnFinishedNum;
		int32 spawnPointFinishedNum;
		
		typedef struct {
			int32 enemyId;
			int32 totalEnemy;
			float32 spawnInterval;
		} LevelConf;
		
		typedef struct {
			int32 listId;
			int32 totalLevel;
			
			std::vector<LevelConf> levelEnemyList;
		} LevelListConf;
		typedef std::map<int32, LevelListConf> LEVEL_CONF_MAP;
		LEVEL_CONF_MAP levelConfs;
		
		typedef std::vector<float32> LEVEL_INTERVAL_CONT;
		LEVEL_INTERVAL_CONT levelIntervals;
		
		typedef std::vector<std::wstring> LEVEL_NAME_CONT;
		LEVEL_NAME_CONT levelNames;
		
		typedef std::list<rftdSource*> SOURCE_LIST;
		SOURCE_LIST mSources;
		
		int32 currLevel;
		float32 currLevelInternval;
		
		int32 state;
		rftdMap* parentMap;
	};
	
} // namespace rftd;

#endif // RFTD_MAP_ENEMY_CONT_H_