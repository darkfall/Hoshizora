/*
 *  meadPlayerManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/3/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_PLAYER_MANAGER_H_
#define MEAD_PLAYER_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "meadPlayer.h"

namespace mead {
	
	class meadPlayerManager: public sora::SoraSingleton<meadPlayerManager> {
	protected:
		friend class sora::SoraSingleton<meadPlayerManager>;
		
		meadPlayerManager();
		~meadPlayerManager();
		
	public:
		ulong32 createPlayer(const std::wstring& script, const std::wstring& bullet, const std::wstring& animation);
		
		bool isPlayerDied(ulong32 handle);
		
		void update();
		void render();
				
		void clearAllPlayers();
		
	private:
		typedef std::map<ulong32, meadPlayer*> PLAYER_LIST;
		PLAYER_LIST mPlayers;
	};
	
} // namespace mead

#endif // MEAD_PLAYER_MANAGER_H_