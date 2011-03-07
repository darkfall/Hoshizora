/*
 *  playerScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/4/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "actor.h"

actor::actor(): actorSprite(0) {
	sora = sora::SoraCore::Instance();
}

actor::~actor() {
	delete actorSprite;
}

bool actor::load(const SoraWString& file) {
	actorSprite = new ls::lsAnimatedSprite(file);
	return false;
}