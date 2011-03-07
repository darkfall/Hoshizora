/*
 *  stgscene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/31/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "stgscene.h"

stgScene::stgScene() {
	sora = sora::SoraCore::Instance();
	
	
	stgbg = new scrollingBackgroundScene(175, 0.f, 450.f, 600.f);
	stgbg->setBackgroundSprite(L"sea.png");
	stgbg->setScrollingSpeed(5.f);
	add(stgbg);
	
	stgui = new backgroundScene(0.f, 0.f, sora->getScreenWidth(), sora->getScreenHeight());
	stgui->setBackgroundSprite(L"stgui.png");
	add(stgui);
	
}

stgScene::~stgScene() {
}

void stgScene::render() {
	lScene::render();
	
}

uint32 stgScene::update(float32 dt) {
	lScene::update(dt);
	
	return 0;
}

void stgScene::onSceneTimerEvent(const projl::lSceneTimerEvent* tev) {
}

