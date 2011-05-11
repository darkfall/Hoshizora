/*
 *  rftdEnemyObject.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdEnemyObject.h"
#include "rftdSource.h"
#include "rftdSpawnPoint.h"

namespace rftd {

	rftdEnemyObject::rftdEnemyObject() {
		mSprite = NULL;
		mEndSource = NULL;
	}
	
	rftdEnemyObject::~rftdEnemyObject() {
		if(mSprite) {
			delete mSprite;
			mSprite = NULL;
		}
	}
	
	uint32 rftdEnemyObject::update(float32 dt) {
		if(mSprite)
			mSprite->update(dt);
		
		steer.update(dt);
		if(steer.isFinished()) {
			if(mEndSource)
				mEndSource->handleEvent(new EnemyReachEndEvent(getAttack()));
		}
		return 0;
	}
	
	void rftdEnemyObject::setPosition(float32 x, float32 y) {
		steer.setPosition(x, y);
	}
	
	float32 rftdEnemyObject::getPositionX() {
		return steer.getPosX();
	}
	
	float32 rftdEnemyObject::getPositionY() {
		return steer.getPosY();
	}
	
	void rftdEnemyObject::render() {
		if(mSprite) {
			mSprite->setRotation(steer.getDirection());
			mSprite->render(steer.getPosX(), steer.getPosY());
		}
	}

	bool rftdEnemyObject::loadConf(Json::Value& val) {
		if(val.isMember("enemy_tex")) {
			if(mSprite) {
				delete mSprite;
				mSprite = NULL;
			}
			mSprite = PRODUCE_SPRITE(val["enemy_tex"]);
		} else {
			mSprite = NULL;
			return false;
		}
		
		if(val.isMember("health"))
			mHealth = val["health"].asInt();
		else
			mHealth = 1;
		
		if(val.isMember("defense"))
			mDefense = val["defense"].asInt();
		else
			mDefense = 0;
		
		if(val.isMember("attack"))
			mAttack = val["attack"].asInt();
		else
			mAttack = 1;
		
		if(val.isMember("speed"))
			steer.setSpeed((float32)val["speed"].asDouble());
		else
			steer.setSpeed(10);
		
		if(val.isMember("type"))
			mType = val["type"].asInt();
		else
			mType = 0;
		
		return true;
	}
	
	void rftdEnemyObject::steerStart() {
		steer.start();
	}
	
	void rftdEnemyObject::addRoutePoint(float32 x, float32 y) {
		steer.addDestPoint(x, y);
	}
	
	bool rftdEnemyObject::isFinished() {
		return steer.isFinished();
	}
	
	int32 rftdEnemyObject::getHealth() const {
		return mHealth;
	}
	
	void rftdEnemyObject::setHealth(int32 health) {
		mHealth = health;
	}
	
	int32 rftdEnemyObject::getDefense() const {
		return mDefense;
	}
	
	void rftdEnemyObject::setDefense(int32 defense) {
		mDefense = defense;
	}
	
	int32 rftdEnemyObject::getType() const {
		return mType;
	}
	
	void rftdEnemyObject::setType(int32 type) {
		mType = type;
	}
	
	int32 rftdEnemyObject::getAttack() const {
		return mAttack;
	}
	
	void rftdEnemyObject::setAttack(int32 attack) {
		mAttack = attack;
	}
	
	void rftdEnemyObject::setEndSource(rftdSource* source) {
		mEndSource = source;
	}
	
	rftdSource* rftdEnemyObject::getEndSource() const {
		return mEndSource;
	}
} // namespace rftd