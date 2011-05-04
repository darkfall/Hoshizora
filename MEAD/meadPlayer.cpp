/*
 *  meadPlayer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadPlayer.h"
#include "SoraLua/luaclass.h"
#include "SoraLua/LuaPlusHelper.h"
#include "SoraCore.h"

#include "SoraEventManager.h"
#include "bulletLuaHelper.h"

namespace mead {	
	
	meadPlayer::meadPlayer() {
		mBulletManager = new meadBulletManager;
		mSprite = NULL;
		
		registerEventFunc(this, &meadPlayer::onKeyEvent);
		sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
		
		mBasicKeys[PLAYER_INPUT_UP] = SORA_KEY_UP;
		mBasicKeys[PLAYER_INPUT_DOWN] = SORA_KEY_DOWN;
		mBasicKeys[PLAYER_INPUT_LEFT] = SORA_KEY_LEFT;
		mBasicKeys[PLAYER_INPUT_RIGHT] = SORA_KEY_RIGHT;
		mBasicKeys[PLAYER_INPUT_SHOOT] = SORA_KEY_Z;
		mBasicKeys[PLAYER_INPUT_BOMB] = SORA_KEY_X;
		mBasicKeys[PLAYER_INPUT_SLOW] = SORA_KEY_SHIFT;
		
		mPlayerStates[PLAYER_MOVE_SPEED]		= 5;
		mPlayerStates[PLAYER_SLOW_MOVE_SPEED]	= 3;
		mPlayerStates[PLAYER_LIFE]				= 3;
		mPlayerStates[PLAYER_BOMB]				= 3;
		mPlayerStates[PLAYER_GRAZE]				= 0;
		mPlayerStates[PLAYER_IMMUNE_TIME]		= 30;
		mPlayerStates[PLAYER_NEED_DESTROY]		= false;
		mPlayerStates[PLAYER_CAN_MOVE]			= true;
		mPlayerStates[PLAYER_CAN_SHOOT]			= true;
		mPlayerStates[PLAYER_CAN_BOMB]			= true;
		mPlayerStates[PLAYER_CAN_SLOW]			= true;
		mPlayerStates[PLAYER_SCORE]				= 0;
		mPlayerStates[PLAYER_HIGH_SCORE]		= 0;
		mPlayerStates[PLAYER_IS_IMMUNE]			= false;
		
		mScreenRect.Set(0.f, 0.f, sora::SORA->getScreenWidth(), sora::SORA->getScreenHeight());
		mCollisionPointRadius = 1.f;
		
		sora = sora::SoraCore::Instance();
	}
	
	meadPlayer::~meadPlayer() {
		onDestroy();
		if(mBulletManager) {
			delete mBulletManager;
			mBulletManager = NULL;
		}
		// deregister self from input event handler list
		sora::SORA_EVENT_MANAGER->unregisterInputEventHandler(this);
	}
	
	bool meadPlayer::loadScript(const std::wstring& script) {
		if(mScriptObject.doScript(script) == 0) {
			exportSelf(mScriptObject.getState());
			
			onInit();
			return true;
		} else {
			sora::SORA->messageBoxW(L"Failed to load player script: "+script, L"error", MB_OK | MB_ICONERROR);
		}
		return false;
	}
	
	bool meadPlayer::loadSettings(const std::wstring& bulletScript, const std::wstring& anm) {
		if(mBulletManager) 
			mBulletManager->loadBulletConfig(bulletScript);
		mSprite = sora::UNPACK_ANIMATION(anm);
		if(mSprite == NULL) {
			sora::SORA->messageBoxW(L"Failed to load player animation file: "+anm, L"error", MB_OK | MB_ICONERROR);
			return false;
		}
		mSprite->setAnchor(sora::ANCHOR_MIDDLE);
		mSpriteWidth_2 = mSprite->getSprite()->getSpriteWidth() >> 2;
		mSpriteHeight_2 = mSprite->getSprite()->getSpriteHeight() >> 2;
		
		return true;
	}
	
	uint32 meadPlayer::update(float32 dt) {
		if(mSprite) {
			mSprite->update(dt);
		}
		
		msgMovement(sora->keyDown(mBasicKeys[PLAYER_INPUT_UP]),
				    sora->keyDown(mBasicKeys[PLAYER_INPUT_DOWN]),
				    sora->keyDown(mBasicKeys[PLAYER_INPUT_LEFT]),
				    sora->keyDown(mBasicKeys[PLAYER_INPUT_RIGHT]));
				
		msgShoot(sora->keyDown(mBasicKeys[PLAYER_INPUT_SHOOT]));
		msgBomb(sora->keyDown(mBasicKeys[PLAYER_INPUT_BOMB]));
		
		onUpdate();
		return 0;
	}
	
	void meadPlayer::msgShoot(bool shoot) {
		if(mPlayerStates[PLAYER_CAN_SHOOT]) {
			if(shoot) {
				onShoot();
			}
		}
	}
	
	void meadPlayer::msgBomb(bool bomb) {
		if(mPlayerStates[PLAYER_CAN_BOMB]) {
			if(bomb) {
				onBomb();
			}
		}
	}
	
	void meadPlayer::msgCustomKey(uint32 key, bool flag) {
		onCustomKey(key, flag);
	}
	
	void meadPlayer::msgMovement(bool left, bool right, bool up, bool down, bool force) {
		if(mPlayerStates[PLAYER_CAN_MOVE] || force) {
			bool isSlow = sora::SORA->keyDown(mBasicKeys[PLAYER_INPUT_SLOW]);
			if(!mPlayerStates[PLAYER_CAN_SLOW])
				isSlow = false;
			
			float32 moveSpeed = isSlow?mPlayerStates[PLAYER_SLOW_MOVE_SPEED]:mPlayerStates[PLAYER_MOVE_SPEED];
		
			// check if the player is within the screen
			if(posx-mSpriteWidth_2 <= mScreenRect.x1) left = false;
			else if(posx+mSpriteWidth_2 >= mScreenRect.x2) right = false;
			if(posy-mSpriteHeight_2 <= mScreenRect.y1) up = false;
			else if(posy+mSpriteHeight_2 >= mScreenRect.y2) down = false;
			
			if(left)
				playAnimation("moveleft", true, false);
			else if(right)
				playAnimation("moveright", true, false);
			
			if((up || down) && (left || right)) {
				if(up) posy -= moveSpeed * sinf(sora::F_PI_4);
				else posy += moveSpeed * sinf(sora::F_PI_4);
				
				if(left) posx -= moveSpeed * cosf(sora::F_PI_4);
				else posx += moveSpeed * cosf(sora::F_PI_4);
			} else if(down) {
				posy += moveSpeed;
			} else if(up) {
				posy -= moveSpeed;
			} else if(right) {
				posx += moveSpeed;
			} else if(left) {
				posx -= moveSpeed;
			} else 
				playAnimation("stand", true, false);
		}	
	}
	
	void meadPlayer::render() {
		
		if(mBulletManager) {
			mBulletManager->update();
		}
		
		if(mSprite) {
			mSprite->setPosition(getPositionX(), getPositionY());
			mSprite->render();
		}
		
		onRender();
	}
	
	void meadPlayer::playAnimation(const char* name, bool loop, bool queue) {
		if(mSprite)
			mSprite->playEx(name, loop, queue);
	}
	
	void meadPlayer::setPlayerState(int32 which, int32 newVal) {
		mPlayerStates[which] = newVal;
	}
	
	void meadPlayer::setPlayerStateDiff(int32 which, int32 diffVal) {
		PLAYER_STATE_MAP::iterator itState = mPlayerStates.find(which);
		if(itState != mPlayerStates.end()) {
			itState->second += diffVal;
		} else 
			mPlayerStates[which] = diffVal;
	}
	
	int32 meadPlayer::getPlayerState(int32 which) {
		PLAYER_STATE_MAP::iterator itState = mPlayerStates.find(which);
		if(itState != mPlayerStates.end()) {
			return itState->second;
		}
		return 0;
	}
	
	int32 meadPlayer::getItemNumber(int32 which) {
		ITEM_NUMBER_MAP::iterator itNumber = mItemNumbers.find(which);
		if(itNumber != mItemNumbers.end()) {
			return itNumber->second;
		}
		return 0;
	}
	
	void meadPlayer::onInit() {
		mScriptObject.callFuncVoid("onInit");
	}
	
	void meadPlayer::onDestroy() {
		mScriptObject.callFuncVoid("onDestroy");
	}
	
	void meadPlayer::onUpdate() {
		mScriptObject.callFuncVoid("onUpdate");
	}
	
	void meadPlayer::onRender() {
		mScriptObject.callFuncVoid("onRender");
	}
	
	void meadPlayer::onShoot() {
		mScriptObject.callFuncVoid("onShoot");
	}
	
	void meadPlayer::onBomb() {
		mScriptObject.callFuncVoid("onBomb");
	}
	
	void meadPlayer::onItem(int32 which) {
		sora::callLuaFunc(mScriptObject.getState(), "onItem", which);
	}
	
	void meadPlayer::onDie(int32 livesLeft) {
		sora::callLuaFunc(mScriptObject.getState(), "onDie", livesLeft);
	}
	
	void meadPlayer::onLoseHealth(int32 healthLeft) {
		sora::callLuaFunc(mScriptObject.getState(), "onLoseHealth", healthLeft);
	}
	
	void meadPlayer::registerCustomKey(uint32 whichKey) {
		mCustomKeys[whichKey] = false;
	}
	
	void meadPlayer::onCustomKey(uint32 key, bool flag) {
		sora::callLuaFunc(mScriptObject.getState(), "onCustomKey", key, flag);
	}
	
	void meadPlayer::onImmuneEnter() {
		mScriptObject.callFuncVoid("onImmuneEnter");
	}
	
	void meadPlayer::onImmuneExit() {
		mScriptObject.callFuncVoid("onImmuneEnter");
	}

	void meadPlayer::onKeyEvent(const sora::SoraKeyEvent* kev) {
		CUSTOM_KEY_MAP::iterator itKey = mCustomKeys.begin();
		while(itKey != mCustomKeys.end()) {
			if(kev->key == itKey->first) {
				if(kev->type == SORA_INPUT_KEYDOWN)
					itKey->second = true;
				else
					itKey->second = false;
				onCustomKey(itKey->first, itKey->second);
			}
			++itKey;
		}
	}

	void meadPlayer::setPosition(float32 x, float32 y) {
		SoraObject::setPosition(x, y);
	}
	
	float32 meadPlayer::getPositionX() {
		return SoraObject::getPositionX();
	}
	
	float32 meadPlayer::getPositionY() {
		return SoraObject::getPositionY();
	}
	
	void meadPlayer::setInputKey(int32 which, uint32 key) {
		if(which >= 0 && which <= PLAYER_INPUT_SLOW)
			mBasicKeys[which] = key;
	}
	
	uint32 meadPlayer::getInputKey(int32 which) {
		if(which >= 0 && which <= PLAYER_INPUT_SLOW)
			return mBasicKeys[which];
		return 0;
	}
	
	void meadPlayer::setCollisionBox(int32 w, int32 h) {
		mSpriteWidth_2 = (int32)w >> 1;
		mSpriteHeight_2 = (int32)h >> 1;
	}
	
	void meadPlayer::setScreenRect(float32 x, float32 y, float32 x1, float32 y1) {
		mScreenRect.Set(x, y, x1, y1);
	}
	
	void meadPlayer::setCollisionPointRadius(float32 r) {
		mCollisionPointRadius = r;
	}
	
	void meadPlayer::exportSelf(LuaPlus::LuaState* state) {
		gge::LuaClass<meadPlayer>(state, "player", this) 
		.def("setPosition", &meadPlayer::setPosition)
		.def("getPositionX", &meadPlayer::getPositionX)
		.def("getPositionY", &meadPlayer::getPositionY)
		.def("loadSettings", &meadPlayer::loadSettings)
		.def("getPlayerState", &meadPlayer::getPlayerState)
		.def("getItemNumber", &meadPlayer::getItemNumber)
		.def("setPlayerState", &meadPlayer::setPlayerState)
		.def("playAnimation", &meadPlayer::playAnimation)
		.def("update", &meadPlayer::onUpdate)
		.def("render", &meadPlayer::onRender)
		.def("setInputKey", &meadPlayer::setInputKey)
		.def("getInputKey", &meadPlayer::getInputKey)
		.def("setScreenRect", &meadPlayer::setScreenRect)
		.def("setCollisionBox", &meadPlayer::setCollisionBox)
		.def("registerCustomKey", &meadPlayer::registerCustomKey);
		
		LuaPlus::LuaObject globals = state->GetGlobals();
		globals.SetInteger("PLAYER_MOVE_SPEED",			PLAYER_MOVE_SPEED);
		globals.SetInteger("PLAYER_SLOW_MOVE_SPEED",	PLAYER_SLOW_MOVE_SPEED);
		globals.SetInteger("PLAYER_LIFE",				PLAYER_LIFE);
		globals.SetInteger("PLAYER_BOMB",				PLAYER_BOMB	);
		globals.SetInteger("PLAYER_GRAZE",				PLAYER_GRAZE);
		globals.SetInteger("PLAYER_IMMUNE_TIME",		PLAYER_IMMUNE_TIME);
		globals.SetInteger("PLAYER_CAN_MOVE",			PLAYER_CAN_MOVE);
		globals.SetInteger("PLAYER_CAN_SLOW",			PLAYER_CAN_SLOW);
		globals.SetInteger("PLAYER_CAN_SHOOT",			PLAYER_CAN_SHOOT);
		globals.SetInteger("PLAYER_CAN_BOMB",			PLAYER_CAN_BOMB);
		globals.SetInteger("PLAYER_IS_IMMUNE",			PLAYER_IS_IMMUNE);
		globals.SetInteger("PLAYER_CURR_IMMUNE_TIME",	PLAYER_CURR_IMMUNE_TIME);
		globals.SetInteger("PLAYER_SCORE",				PLAYER_SCORE);
		globals.SetInteger("PLAYER_HIGH_SCORE",			PLAYER_HIGH_SCORE);
		globals.SetInteger("PLAYER_NEED_DESTROY",		PLAYER_NEED_DESTROY);
		
		
		globals.SetInteger("PLAYER_INPUT_UP",			PLAYER_INPUT_UP);
		globals.SetInteger("PLAYER_INPUT_DOWN",			PLAYER_INPUT_DOWN);
		globals.SetInteger("PLAYER_INPUT_LEFT",			PLAYER_INPUT_LEFT);
		globals.SetInteger("PLAYER_INPUT_RIGHT",		PLAYER_INPUT_RIGHT);
		globals.SetInteger("PLAYER_INPUT_SHOOT",		PLAYER_INPUT_SHOOT);
		globals.SetInteger("PLAYER_INPUT_BOMB",			PLAYER_INPUT_BOMB);
		globals.SetInteger("PLAYER_INPUT_SLOW",			PLAYER_INPUT_SLOW);
		
		gge::LuaClass<meadBulletManager>(state, "bullet", mBulletManager)
			.def("shootBullet", &meadBulletManager::shootBullet)
			.def("shootBulletEx", &meadBulletManager::shootBulletEx)
			.def("shootLaser", &meadBulletManager::shootLaser)
			.def("shootLaserEx", &meadBulletManager::shootLaserEx)
			.def("shootBulletDirect", &meadBulletManager::shootBulletDirect)
			.def("loadBulletConfig", &meadBulletManager::loadBulletConfig)
			.def("loadBulletSprite", &meadBulletManager::loadBulletSprite)
			.def("update", &meadBulletManager::update)
			.def("clearBulletList", &meadBulletManager::clearBulletList)
			.def("setScreenRect", &meadBulletManager::setScreenRectAABB)
			.def("getSize", &meadBulletManager::getSize)
			.def("isBulletValid", &meadBulletManager::isBulletValid)
			.def("setBulletId", &meadBulletManager::setBulletId)
			.def("getBullet", &meadBulletManager::getBulletDirect);
		LuaModule(state)
			.def("setBulletPos", &setBulletPosition)
			.def("setBulletSpeed", &setBulletSpeed)
			.def("setBulletOriginPoint", &setBulletOriginPoint)
			.def("setBulletIncrementalSpeed", &setBulletIncrementalSpeed)
			.def("setBulletIncrementalSpeedEx", &setBulletIncrementalSpeedEx)
			.def("setBulletIncrementalDirection", &setBulletIncrementalDirection)
			.def("setBulletCustomValue1", &setBulletCustomValue1)
			.def("setBulletCustomValue2", &setBulletCustomValue2)
			.def("setBulletReflectionTimes", &setBulletReflectionTimes)
			.def("setBulletDirection", &setBulletDirection)
			.def("getBulletSpeed", &getBulletSpeed)
			.def("getBulletSpeedX", &getBulletSpeedX)
			.def("getBulletSpeedY", &getBulletSpeedY)
			.def("getBulletDirection", &getBulletDirection)
			.def("getBulletOriginPointX", &getBulletOriginPointX)
			.def("getBulletOriginPointY", &getBulletOriginPointY)
			.def("getBulletReflectionTimes", &getBulletReflectionTimes)
			.def("getBulletIncrementalSpeed", &getBulletIncrementalSpeed)
			.def("getBulletIncrementalSpeedX", &getBulletIncrementalSpeedX)
			.def("getBulletIncrementalSpeedY", &getBulletIncrementalSpeedY)
			.def("getBulletCustomValue1", &getBulletCustomValue1)
			.def("getBulletCustomValue2", &getBulletCustomValue2)
			.def("getBulletPosX", &getBulletPositionX)
			.def("getBulletPosY", &getBulletPositionY)
			.def("removeBullet", &removeBullet)
			.def("getBulletId", &getBulletId);
	}
} // namespace mead
