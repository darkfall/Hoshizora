/*
 *  rftdSource.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/9/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdSource.h"

namespace rftd {
	
	rftdSource::rftdSource() {
		mSourceSprite = NULL;
		mLightPathSprite = NULL;
		
		mFont = (sora::SoraFont*)sora::GET_ENV_DATA("rftd_global_font");
		
		registerEventFunc(this, &rftdSource::onEnemyReachEndEvent);
	}
	
	rftdSource::~rftdSource() {
		if(mSourceSprite)
			delete mSourceSprite;
		if(mLightPathSprite)
			delete mLightPathSprite;
	}
	
	bool rftdSource::loadConf(Json::Value& val) {
		if(val.isMember("source_id"))
			mSourceId = val["source_id"].asInt();
		else
			return false;
		
		if(val.isMember("health"))
			mHealth = val["health"].asInt();
		else
			mHealth = 1;
		
		if(val.isMember("source_tex")) {
			if(mSourceSprite)
				delete mSourceSprite;
			mSourceSprite = PRODUCE_SPRITE(val["source_tex"]);
		}
		else {
			mSourceSprite = NULL;
			return false;
		}
		
		if(val.isMember("posx") && val.isMember("posy"))
			setPosition(val["posx"].asInt(), 
						val["posy"].asInt());
		else
			return false;
		
		if(val.isMember("lightpath_tex")) {
			if(mLightPathSprite)
				delete mLightPathSprite;
			mLightPathSprite = PRODUCE_SPRITE(val["lightpath_tex"]);
		}
		else {
			mLightPathSprite = NULL;
			return false;
		}
		return true;
	}
	
	int32 rftdSource::getHealth() const {
		return mHealth;
	}
	
	void rftdSource::setHealth(int32 health) {
		mHealth = health;
	}
	
	int32 rftdSource::getId() const {
		return mSourceId;
	}
	
	uint32 rftdSource::update(float32 dt) {
		if(mSourceSprite)
			mSourceSprite->update(dt);
		
		if(mLightPathSprite)
			mLightPathSprite->update(dt);
		
		return 0;
	}
	
	void rftdSource::onEnemyReachEndEvent(const EnemyReachEndEvent* event) {
		mHealth -= event->getAttack();
	}
	
	void rftdSource::render() {
		if(mSourceSprite)
			mSourceSprite->render(getPositionX(), getPositionY());
		if(mFont) {
			mFont->print(getPositionX(), 
						 getPositionY()+mSourceSprite->getSpriteHeight()-mSourceSprite->getCenterY(), 
						 sora::FONT_ALIGNMENT_LEFT, 
						 L"Health: %d",
						 mHealth);
		}
	}

} // namespace rftd