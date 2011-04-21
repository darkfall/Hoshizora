/*
 *  lsAnimatedSprite.h
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#ifndef _LS_ANIMATED_SPRITE_H_
#define _LS_ANIMATED_SPRITE_H_

#include "SoraSprite.h"
#include "lAnm/lAnm.h"
#include "SoraEvent.h"
#include "SoraTimerEvent.h"

namespace ls {
	
	class lsAnimatedSprite: public sora::SoraEventHandler {
	public:
		lsAnimatedSprite();
		lsAnimatedSprite(const SoraWString& path);
		
		bool load(const SoraWString& path);
		
		void render(float32 x, float32 y);
		void update(float32 dt);
		
		bool valid();
		
		void play();
		void play(const char* name, bool bLoop=false);
	
	private:
		lsAnimatedSprite(const lsAnimatedSprite&);
		lsAnimatedSprite& operator=(const lsAnimatedSprite&);
		
		lAnm* pAnm;
		sora::SoraSprite* pSpr;
	};
} // namespace ls

#endif

