/*
 *  meadPlayer.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_PLAYER_H_
#define MEAD_PLAYER_H_


#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

#include "SoraSpriteAnimation/SoraSpriteAnimation.h"
#include "SoraLua/SoraLuaObject.h"

#include "meadBulletManager.h"

namespace mead {
	
	class meadPlayer: public sora::SoraObject {
		friend class meadPlayerManager;
		
	public:
		meadPlayer();
		~meadPlayer();
		
		bool loadScript(const std::wstring& script);
		
		/*
		 load player settings
		 @param bulletScript, the bullet definition script for the player
		 @param anm, the animation definition script for the player
		 */
		bool loadSettings(const std::wstring& bulletScript, const std::wstring& anm);
		
		// inheritated from SoraObject
		uint32 update(float32 dt);
		void render();
		
		void playAnimation(const char* name, bool loop, bool queue);
		
		// internal player states
		enum {
			PLAYER_HEALTH = -1,
			PLAYER_LIFE = -2,
			PLAYER_IMMUNE_TIME = -3,
			PLAYER_GRAZE = -4,
			PLAYER_NEED_DESTROY = -5,
			PLAYER_SLOW_MOVE_SPEED = -6,
			PLAYER_MOVE_SPEED = -7,
			PLAYER_BOMB = -8,
			PLAYER_CAN_MOVE = -9,
			PLAYER_IS_IMMUNE = -10,
			PLAYER_CAN_SHOOT = -11,
			PLAYER_CAN_BOMB = -12,
			PLAYER_CAN_SLOW = -13,
			PLAYER_HIGH_SCORE = -14,
			PLAYER_SCORE = -15,
			PLAYER_CURR_IMMUNE_TIME = -16,
		};
		
		// states
		// you can also access states by SoraGlobalValues
		// states in SoraGlobalValues are began with "player_"
		// "player_life", "player_bomb" etc
		// which can be any id you like
		// but be sure id is larger than 0 , otherwise would overwrite the default data
		void setPlayerState(int32 which, int32 newVal);
		// the val would be added to the original number
		void setPlayerStateDiff(int32 which, int32 diffVal);
		int32 getPlayerState(int32 which);
		
		// items are controlled by item manager
		int32 getItemNumber(int32 which);
		
		// script export functions
		
		// on player create first time
		void onInit();
		// on player being destroyed
		void onDestroy();
		
		// player update
		void onUpdate();
		// player render
		void onRender();
		
		// default Z(shoot) key
		void onShoot();
		// default X(bomb) key
		void onBomb();
		// default on bullet graze (if it is on)
		void onGraze();
		// default on get item
		void onItem(int32 item);
		
		// default on die
		void onDie(int32 livesLeft);
		
		// rarely used functions
		// on lose health, not commonly used
		void onLoseHealth(int32 healthLeft);
		
		// default player keys
		// on special key pressed, registered by script
		// for special systems
		void registerCustomKey(uint32 whichKey);
		// on time that a custom key have pressed / up
		void onCustomKey(uint32 key, bool flag);
		
		// immule time
		// the time between player died and respawned until it will receive bullet damage
		void onImmuneEnter();
		void onImmuneExit();
		
		void setPosition(float32 x, float32 y);
		float32 getPositionX();
		float32 getPositionY();
		
		// basic stg keys
		enum {
			PLAYER_INPUT_UP = 0,
			PLAYER_INPUT_DOWN = 1,
			PLAYER_INPUT_LEFT = 2,
			PLAYER_INPUT_RIGHT = 3,
			PLAYER_INPUT_SHOOT = 4,
			PLAYER_INPUT_BOMB = 5,
			PLAYER_INPUT_SLOW = 6,
		};
		void setInputKey(int32 which, uint32 newKey);
		uint32 getInputKey(int32 which);
		
		void setScreenRect(float32 x, float32 y, float32 x1, float32 y1);
		// use to limit movement within the screen
		void setCollisionBox(int32 w, int32 h);
		// collision point radius
		void setCollisionPointRadius(float32 r);
		
		// these function are for replay system convinience
		// can be use to simulate player states
		void msgMovement(bool left, bool right, bool up, bool down, bool force=false);
		void msgShoot(bool shoot);
		void msgBomb(bool bomb);
		void msgCustomKey(uint32 key, bool flag);
		
	private:
		void exportSelf(LuaPlus::LuaState* state);
		void onKeyEvent(sora::SoraKeyEvent* kev);
		
		sora::SoraLuaObject mScriptObject;
		meadBulletManager* mBulletManager;
		
		sora::SoraSpriteAnimation* mSprite;
		float32 mSpriteWidth_2, mSpriteHeight_2;

		sora::SoraRect mScreenRect;
		float32 mCollisionPointRadius;

		typedef std::map<int32/*itemid*/, int32/*item number*/> ITEM_NUMBER_MAP;
		ITEM_NUMBER_MAP mItemNumbers;
		typedef ITEM_NUMBER_MAP PLAYER_STATE_MAP;
		PLAYER_STATE_MAP mPlayerStates;
		
		typedef std::map<int32, bool> CUSTOM_KEY_MAP;
		CUSTOM_KEY_MAP mCustomKeys;
		
		uint32 mBasicKeys[7];
		
		sora::SoraCore* sora;
	}; 
} // namespace mead

#endif // MEAD_PLAYER_H_