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
		
		mHealthTextX = mHealthTextY = 0.f;
		mShowHealthText = false;
		mHealthTextAlign = sora::FONT_ALIGNMENT_LEFT;
		
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
		
		if(val.isMember("show_health_text")) {
			mShowHealthText = val["show_health_text"].asBool();
			Json::Value healthPropVal = val["health_text_prop"];
			if(healthPropVal.isObject()) {
				if(healthPropVal.isMember("x") && healthPropVal.isMember("y")) {
					mHealthTextX = (float32)healthPropVal["x"].asDouble();
					mHealthTextY = (float32)healthPropVal["y"].asDouble();
				} 
				if(healthPropVal.isMember("align")) {
					std::string alignStr = healthPropVal["align"].asString();
					if(strcmpnocase(alignStr.c_str(), "center") == 0)
						mHealthTextAlign = sora::FONT_ALIGNMENT_CENTER;
					else if(strcmpnocase(alignStr.c_str(), "left") == 0)
						mHealthTextAlign = sora::FONT_ALIGNMENT_LEFT;
					else
						mHealthTextAlign = sora::FONT_ALIGNMENT_RIGHT;
				}
			}
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
		if(mFont && mShowHealthText) {
			mFont->print(getPositionX()+mHealthTextX, 
						 getPositionY()+mHealthTextY, 
						 mHealthTextAlign, 
						 L"Health: %d",
						 mHealth);
		}
	}

} // namespace rftd