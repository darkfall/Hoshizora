/*
 *  rftdEnemyObject.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdEnemyObject.h"

namespace rftd {

	rftdEnemyObject::rftdEnemyObject() {
		mSprite = NULL;
	}
	
	rftdEnemyObject::~rftdEnemyObject() {
		if(mSprite) {
			delete mSprite;
			mSprite = NULL;
		}
	}
	
	uint32 rftdEnemyObject::update(float32 dt) {
		steer.update(dt);
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
			mSprite = PRODUCE_SPRITE(val["enemy_tex"]);
		}
		
		if(val.isMember("health"))
			mHealth = val["health"].asInt();
		
		if(val.isMember("defense"))
			mDefense = val["defense"].asInt();
		
		if(val.isMember("speed"))
			steer.setSpeed((float32)val["speed"].asDouble());
		
		if(val.isMember("type"))
			mType = val["type"].asInt();
		
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
} // namespace rftd