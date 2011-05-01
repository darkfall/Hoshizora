/*
 *  meadBossManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_BOSS_MANAGER_H_
#define MEAD_BOSS_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "meadBoss.h"

#include <map>

namespace mead {
	
	class meadBossManager: public sora::SoraSingleton<meadBossManager> {
	protected:
		friend class sora::SoraSingleton<meadBossManager>;
		
		meadBossManager();
		~meadBossManager();
		
	public:
		ulong32 createBoss(const std::wstring& script, const std::string& animation);
		
		bool isBossDied(ulong32 handle);
		
		void update();
		void render();
		
		void reloadScripts();
		
		void clearAllBosses();
		
	private:
		typedef std::map<ulong32, meadBoss*> BOSS_LIST;
		BOSS_LIST mBosses;
	};
	
	
} // namespace sora


#endif // MEAD_BOSS_MANAGER_H_