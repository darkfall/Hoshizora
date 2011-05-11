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
	
	class rftdSource;
	
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
		
		int32 getHealth() const;
		void setHealth(int32 health);
		
		int32 getDefense() const;
		void setDefense(int32 defense);
		
		int32 getType() const;
		void setType(int32 type);
		
		int32 getAttack() const;
		void setAttack(int32 attack);
		
		void setEndSource(rftdSource* endSource);
		rftdSource* getEndSource() const;
		
	private:
		sora::SoraSprite* mSprite;
		sora::SoraColorRGBA mColor;
		
		rftdSteerBehaviour steer;
		
		int32 mHealth;
		int32 mDefense;
		int32 mType;
		int32 mAttack;
		
		rftdSource* mEndSource;
	};
	
} // namespace rftd

#endif // RFTD_ENEMY_OBJECT_H_