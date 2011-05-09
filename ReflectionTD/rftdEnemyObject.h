/*
 *  rftdEnemyObject.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_ENEMY_OBJECT_H_
#define RFTD_ENEMY_OBJECT_H_

#include "rftdCommon.h"
#include "rftdSteer.h"
#include "SoraSprite.h"
#include "SoraColor.h"

#include "AutoContainer.h"

namespace rftd {

	class rftdEnemyObject: public sora::SoraObject, public sora::AutoListElement<rftdEnemyObject> {
	public:
		rftdEnemyObject();
		virtual ~rftdEnemyObject();
		
		virtual uint32 update(float32 dt);
		virtual void render();
		
		void steerStart();
		void addRoutePoint(float32 x, float32 y);
		
		bool isFinished();
		
		void setPosition(float32 x, float32 y);
		float32 getPositionX();
		float32 getPositionY();
		
		bool loadConf(Json::Value& val);
		
	private:
		sora::SoraSprite* mSprite;
		sora::SoraColorRGBA mColor;
		
		rftdSteerBehaviour steer;
		
		int32 mHealth;
		int32 mDefense;
		int32 mType;
		
		//EnemyConf mProperties;
	};
	
} // namespace rftd

#endif // RFTD_ENEMY_OBJECT_H_