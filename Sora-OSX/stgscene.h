/*
 *  stgscene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/31/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_STG_SCENE_H_
#define L_STG_SCENE_H_

#include "backgroundScene.h"
#include "lScene.h"
#include "menuScene.h"
#include "lBasicRenderItem.h"

#include "SoraCore.h"
#include "SoraFont.h"

class stgScene: public projl::lScene {
public:
	stgScene();
	~stgScene();
	
	void render();
	uint32 update(float32 dt);
	
	void onSceneTimerEvent(const projl::lSceneTimerEvent* tev);
	
private:
	sora::SoraCore* sora;
	
	backgroundScene* stgui;
	scrollingBackgroundScene* stgbg;
};

#endif