/*
 *  meadBoss.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_BOSS_H_
#define MEAD_BOSS_H_

#include "SoraLua/SoraLuaObject.h"
#include "SoraSpriteAnimation/SoraSpriteAnimation.h"

#include "SoraImageEffect.h"

namespace mead {
	
	class meadBoss: public sora::SoraObject {
	public:
		meadBoss();
		~meadBoss();
		
		bool setScript(const std::wstring& script);
		void reloadScript();
		void setSprite(const std::wstring& animation);
		
		void loadScript(const std::wstring& script);
		
		void setPosition(float32 x, float32 y);
		float32 getPositionX();
		float32 getPositionY();
		
		void moveToPosition(float32 x, float32 y, uint32 inFrame);
		float32 getDestPositionX();
		float32 getDestPositionY();
		
		uint32 getCurrentHealth();
		uint32 getCurrentLifeNumber();
		
		void setHealth(uint32 health);
		void setLifeNumber(uint32 life);
		
		void setDied(bool flag);
		bool isDied();
		
		void playAnimation(const char* name, bool loop, bool queue);
		
		uint32 update(float32 dt);
		void render();
		
		
		// script callbacks
		
		void onDie();
		void onReceiveDamage(uint32 healthLeft);
		
		
	private:
		void exportSelf(LuaPlus::LuaState* state);
		
		sora::SoraSpriteAnimation* mBossSprite;
		sora::SoraLuaObject* mBossScript;
		
		std::wstring currentScript;
		
		float32 posx, posy;
		float32 destPosX, destPosY;
		float32 speedX, speedY;
		
		uint32 currMoveFrame;
		uint32 moveFrame;
		
		uint32 health;
		uint32 lives;
		
		bool bDied;
	};
	
	
} // namespace mead

#endif // MEAD_BOSS_H_