/*
 *  AnimationLuaExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ANIMATION_LUA_EXRORT_H_
#define SORA_ANIMATION_LUA_EXPORT_H_

#include "SoraLua/LuaPlusHelper.h"
#include "SoraSpriteAnimation.h"

namespace sora {
	
	static void exportSpriteAnimation(LuaPlus::LuaState* state) {
		LuaClass<SoraSpriteAnimation>(state)
			.create<const std::wstring&>("AnimatedSprite")
			.destroy("Free")
			.def("update", &SoraSpriteAnimation::update)
			.def("render", &SoraSpriteAnimation::render)
			.def("play", &SoraSpriteAnimation::play)
			.def("playEx", &SoraSpriteAnimation::play)
			.def("pause", &SoraSpriteAnimation::pause)
			.def("resume", &SoraSpriteAnimation::resume)
			.def("setDefaultAnimation", &SoraSpriteAnimation::setDefaultAnimation)
			.def("setAnchor", &SoraSpriteAnimation::setAnchor);
		
		LuaObject globals = state->GetGlobals();
		globals.SetInteger("ANCHOR_UPPER_LEFT", ANCHOR_UPPER_LEFT);
		globals.SetInteger("ANCHOR_UPPER_RIGHT", ANCHOR_UPPER_RIGHT);
		globals.SetInteger("ANCHOR_LOWER_LEFT", ANCHOR_LOWER_LEFT);
		globals.SetInteger("ANCHOR_LOWER_RIGHT", ANCHOR_LOWER_RIGHT);
		globals.SetInteger("ANCHOR_MIDDLE", ANCHOR_MIDDLE);

	}
	
}


#endif // SORA_ANIMATION_LUA_EXPORT_H_