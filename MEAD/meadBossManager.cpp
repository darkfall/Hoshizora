/*
 *  meadBossManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadBossManager.h"
#include "SoraCore.h"

namespace mead {

	meadBossManager::meadBossManager() {
	}
	
	meadBossManager::~meadBossManager() {
		clearAllBosses();
	}
	
	ulong32 meadBossManager::createBoss(const std::wstring& script, const std::wstring& animation) {
		meadBoss* boss = new meadBoss;
		boss->setSprite(animation);
		if(!boss->setScript(script)) {
			sora::SORA->logf("meadBossManager: error load boss script %s", sora::ws2s(script).c_str());
			delete boss;
			return 0;
		}
		mBosses[(ulong32)boss] = boss;
		return (ulong32)boss;
	}
	
	bool meadBossManager::isBossDied(ulong32 handle) {
		BOSS_LIST::iterator itBoss = mBosses.find(handle);
		if(itBoss != mBosses.end()) {
			return itBoss->second->isDied();
		}
		return true;
	}
	
	void meadBossManager::reloadScripts() {
		BOSS_LIST::iterator itBoss = mBosses.begin();
		while(itBoss != mBosses.end()) {
			itBoss->second->reloadScript();
			++itBoss;
		}
	}
	
	void meadBossManager::update() {
		BOSS_LIST::iterator itBoss = mBosses.begin();
		while(itBoss != mBosses.end()) {
			itBoss->second->update(0.f);
			if(itBoss->second->isDied()) {
				delete itBoss->second;
				itBoss->second = NULL;
				mBosses.erase(itBoss);
			}
			++itBoss;
		}
	}
	
	void meadBossManager::render() {
		BOSS_LIST::iterator itBoss = mBosses.begin();
		while(itBoss != mBosses.end()) {
			itBoss->second->render();
			++itBoss;
		}
	}

	void meadBossManager::clearAllBosses() {
		BOSS_LIST::iterator itBoss = mBosses.begin();
		while(itBoss != mBosses.end()) {
			delete itBoss->second;
			itBoss->second = NULL;
			
			++itBoss;
		}
		mBosses.clear();
	}

} // namespace mead