/*
 *  meadPlayerManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/3/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadPlayerManager.h"

#include "SoraCore.h"

namespace mead {
	
	meadPlayerManager::meadPlayerManager() {
	}
	
	meadPlayerManager::~meadPlayerManager() {
		clearAllPlayers();
	}
	
	ulong32 meadPlayerManager::createPlayer(const std::wstring& script, const std::wstring& bullet, const std::wstring& animation) {
		meadPlayer* player = new meadPlayer;
		if(!player->loadSettings(bullet, animation)) {
			sora::SORA->log("meadPlayerManager: error load player settings");
			delete player;
			return 0;
		}
		if(!player->loadScript(script)) {
			sora::SORA->logf("meadPlayerManager: error load player script %s", sora::ws2s(script).c_str());
			delete player;
			return 0;
		}
		mPlayers[(ulong32)player] = player;
		return (ulong32)player;
	}
	
	bool meadPlayerManager::isPlayerDied(ulong32 handle) {
		PLAYER_LIST::iterator itPlayer = mPlayers.find(handle);
		if(itPlayer != mPlayers.end()) {
			return itPlayer->second->mPlayerStates[meadPlayer::PLAYER_NEED_DESTROY];
		}
		return true;
	}

	void meadPlayerManager::update() {
		float32 dt = sora::SORA->getDelta();
		PLAYER_LIST::iterator itPlayer = mPlayers.begin();
		while(itPlayer != mPlayers.end()) {
			itPlayer->second->update(dt);
			if(itPlayer->second->mPlayerStates[meadPlayer::PLAYER_NEED_DESTROY]) {
				delete itPlayer->second;
				itPlayer->second = NULL;
				mPlayers.erase(itPlayer);
			}
			++itPlayer;
		}
	}
	
	void meadPlayerManager::render() {
		PLAYER_LIST::iterator itPlayer = mPlayers.begin();
		while(itPlayer != mPlayers.end()) {
			itPlayer->second->render();
			++itPlayer;
		}
	}
	
	void meadPlayerManager::clearAllPlayers() {
		PLAYER_LIST::iterator itPlayer = mPlayers.begin();
		while(itPlayer != mPlayers.end()) {
			delete itPlayer->second;
			itPlayer->second = NULL;
			
			++itPlayer;
		}
		mPlayers.clear();
	}
	
} // namespace mead