//
//  AnimationLuaExport.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/26/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "AnimationLuaExport.h"
#include "SoraSpriteAnimation.h"

namespace sora {
    
    void lua_export_sprite_animation(LuaPlus::LuaState* state) {
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
    
} // namespace sora
