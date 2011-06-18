/*
 *  rftdSource.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/9/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_SOURCE_H_
#define RFTD_SOURCE_H_

#include "rftdCommon.h"
#include "rftdEvents.h"

namespace rftd {
	
	class EnemyReachEndEvent;
	
	// the source of all contructable sources
	// also a PCB style electrical path renderer
	// configured in rftdmap files
	class rftdSource: public sora::SoraObject {
	public:
		rftdSource();
		~rftdSource();
		
		bool loadConf(Json::Value& val);
		
		virtual uint32 update(float32 dt);
		virtual void render();
		
		int32 getHealth() const;
		void setHealth(int32 health);
		
		int32 getId() const;
		
		void onEnemyReachEndEvent(const EnemyReachEndEvent* event);
		
	private:
		int32 mSourceId;
		int32 mHealth;
		
		sora::SoraSprite* mSourceSprite;
		sora::SoraSprite* mLightPathSprite;
		
		float32 mHealthTextX, mHealthTextY;
		int32 mHealthTextAlign;
		bool mShowHealthText;
		
		typedef std::vector<hgeVector> LIGHT_ROUTE;
		typedef std::list<LIGHT_ROUTE > LIGHT_ROUTE_CONT;
		
		LIGHT_ROUTE_CONT mRoutes;
		
		sora::SoraFont* mFont;
	};
	
} // namespace rftd

#endif // RFTD_SOURCE_H_