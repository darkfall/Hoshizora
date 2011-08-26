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
#include "SoraLua/SoraLuaExporter.h"

namespace sora {
	
    void lua_export_sprite_animation(LuaPlus::LuaState* state);

#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_ANIMATED_SPRITE)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_sprite_animation)
#endif
    
}


#endif // SORA_ANIMATION_LUA_EXPORT_H_