/*
 *  meadBoss.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadBoss.h"
#include "SoraCore.h"
#include "bulletLuaExport.h"

#include "SoraSoundManager/SoundManagerLuaExport.h"
#include "SoraSpriteAnimation/AnimationLuaExport.h"
#include "SoraGlobalValues/GlobalValuesLuaExport.h"

namespace mead {

	meadBoss::meadBoss(): posx(0.f), posy(0.f), destPosX(0.f), destPosY(0.f), speedX(0.f), speedY(0.f),
							currMoveFrame(0), moveFrame(0),
							health(0), lives(0),
							mBossScript(NULL), mBossSprite(NULL), bDied(false) {
							}
	
	meadBoss::~meadBoss() {
		if(mBossScript) {
			mBossScript->callFuncVoid("finish");
			delete mBossScript;
		}
		if(mBossSprite)
			delete mBossSprite;
	}
	
	bool meadBoss::setScript(const std::wstring& script) {
		mBossScript = new sora::SoraLuaObject;
		exportSelf(mBossScript->getState());

		if(mBossScript->doScript(script) == 0) {			
			mBossScript->callFuncVoid("init");
			
			currentScript = script;
			return true;
		}
		return false;
	}
	
	void meadBoss::reloadScript() {
		if(mBossScript->doScript(currentScript) == 0) {			
			mBossScript->callFuncVoid("init");
		}
	}
	
	void meadBoss::setSprite(const std::string& sprite) {
		mBossSprite = sora::SoraSpriteAnimationPacker::unpack(sprite.c_str());
		if(mBossSprite)
			mBossSprite->setAnchor(sora::ANCHOR_MIDDLE);
	}
	
	void meadBoss::exportSelf(LuaPlus::LuaState* state) {
		gge::LuaClass<meadBoss>(state, "boss", this)
			.def("setSprite", &meadBoss::setSprite)
			.def("setPosition", &meadBoss::setPosition)
			.def("getPositionX", &meadBoss::getPositionX)
			.def("getPositionY", &meadBoss::getPositionY)
			.def("moveToPosition", &meadBoss::moveToPosition)
			.def("getDestPositionX", &meadBoss::getDestPositionX)
			.def("getDestPositionY", &meadBoss::getDestPositionY)
			.def("getCurrentHealth", &meadBoss::getCurrentHealth)
			.def("getCurrentLifeNumber", &meadBoss::getCurrentLifeNumber)
			.def("setHealth", &meadBoss::setHealth)
			.def("setLifeNumber", &meadBoss::setLifeNumber)
			.def("setDied", &meadBoss::setDied)
			.def("isDied", &meadBoss::isDied)
			.def("playAnimation", &meadBoss::playAnimation)
			.def("loadScript", &meadBoss::loadScript);
		
		exportBulletManager(state);
		sora::exportSoundManager(state);
		sora::exportSpriteAnimation(state);
		sora::exportGlobalValuesManager(state);
		sora::GlobalValuesExporter::exportValues(state);
	}
	
	void meadBoss::setPosition(float32 x, float32 y) {
		posx = x; posy = y;
	}
	
	float32 meadBoss::getPositionX() {
		return posx;
	}
	
	float32 meadBoss::getPositionY() {
		return posy;
	}
	
	void meadBoss::moveToPosition(float32 x, float32 y, uint32 frame) {
		speedX = (x-posx)/frame;
		speedY = (y-posy)/frame;
		
		if(speedX > 0)
			playAnimation("moveright", true, false);
		else if(speedX < 0)
			playAnimation("moveleft", true, false);
		
		currMoveFrame = 0;
		moveFrame = frame;
	}
	
	void meadBoss::loadScript(const std::wstring& script) {
		if(mBossScript) {
			if(mBossScript->doScript(script) != 0)
				sora::SORA->messageBox("fail to load dependent script", "error", MB_OK | MB_ICONERROR);
		}
	}
	
	float32 meadBoss::getDestPositionX() {
		return destPosX;
	}
	
	float32 meadBoss::getDestPositionY() {
		return destPosY;
	}
	
	uint32 meadBoss::getCurrentHealth() {
		return health;
	}
	
	uint32 meadBoss::getCurrentLifeNumber() {
		return lives;
	}
	
	void meadBoss::playAnimation(const char* name, bool loop, bool queue) {
		if(mBossSprite)
			mBossSprite->playEx(name, loop, queue);
	}
	
	void meadBoss::setHealth(uint32 h) {
		health = h;
	}
	
	void meadBoss::setLifeNumber(uint32 n) {
		lives = n;
	}
	
	void meadBoss::update() {
		float32 dt = sora::SORA->getDelta();
		if(mBossScript)
			mBossScript->update(dt);
		if(mBossSprite)
			mBossSprite->update(dt);
		
		if(moveFrame != 0) {
			++currMoveFrame;
			if(currMoveFrame >= moveFrame)
				moveFrame = 0;
			
			posx += speedX;
			posy += speedY;
		}
	}
	
	void meadBoss::render() {
		if(mBossSprite) {
			mBossSprite->setPosition(posx, posy);
			mBossSprite->render();
		}
		if(mBossScript)
			mBossScript->render();
	}
	
	void meadBoss::setDied(bool flag) {
		bDied = flag;
	}
	
	bool meadBoss::isDied() {
		return bDied;
	}
	
} // namespace mead