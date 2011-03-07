/*
 *  backgroundScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "backgroundScene.h"

backgroundScene::backgroundScene(float32 x, float32 y, float32 w, float32 h) {
	sora = sora::SoraCore::Instance();
	pbgspr = 0;
	
	setDimension(x, y, w, h);
}

backgroundScene::~backgroundScene() {
	if(pbgspr) {
		delete pbgspr;
		pbgspr = 0;
	}
}

void backgroundScene::setBackgroundSprite(const SoraWString& spr) {
	pbgspr = sora->createSprite(spr);
	
	pbgspr->setScale(getSceneWidth()/pbgspr->getTextureWidth(), getSceneHeight()/pbgspr->getTextureHeight());
	pbgspr->setPosition(getScenePosX(), getScenePosY());
}

uint32 backgroundScene::update(float32 dt) {
	if(pbgspr)
		pbgspr->update(dt);
	return 0;
}

void backgroundScene::render() {
	if(pbgspr)
		pbgspr->render();
}



scrollingBackgroundScene::scrollingBackgroundScene(float32 x, float32 y, float32 w, float32 h):
backgroundScene(x, y, w, h) {
	camx = 0.f;
	camy = 0.f;
	speed = 1.f;
}

scrollingBackgroundScene::~scrollingBackgroundScene() {
}

void scrollingBackgroundScene::setBackgroundSprite(const SoraWString& spr) {
	pbgspr = sora->createSprite(spr);
	
	pbgspr->setTextureRect(0.f, 0.f, getSceneWidth(), getSceneHeight());
	pbgspr->setPosition(getScenePosX(), getScenePosY());
}

uint32 scrollingBackgroundScene::update(float32 dt) {
	if(pbgspr) {
		pbgspr->update(dt);
		
		camy += speed;
		if(camy >= pbgspr->getTextureHeight() - getSceneHeight())
			camy = 0.f;
		pbgspr->setTextureRect(camx, camy, getSceneWidth(), getSceneHeight());
	}
	return 0;
}