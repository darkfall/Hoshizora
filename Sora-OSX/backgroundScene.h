/*
 *  backgroundScene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_BACKGROUND_SCENE_H_
#define L_BACKGROUND_SCENE_H_

#include "lScene.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraImageEffect.h"

class backgroundScene: public projl::lScene {
	friend class scrollingBackgroundScene;
	
public:
	backgroundScene(float32 x, float32 y, float32 w, float32 h);
	virtual ~backgroundScene();
	
	virtual void setBackgroundSprite(const SoraWString& spr);
	
	uint32 update(float32 dt);
	void render();
	
	void onSceneTimerEvent(const projl::lSceneTimerEvent* tev) {}
	
private:
	sora::SoraCore* sora;
	sora::SoraSprite* pbgspr;
};

class scrollingBackgroundScene: public backgroundScene {
public:
	scrollingBackgroundScene(float32 x, float32 y, float32 w, float32 h);
	virtual ~scrollingBackgroundScene();
	
	void setScrollingSpeed(float32 _speed) { speed = _speed; }
	float32 getScrollingSpeed() const { return speed; }
	
	void setBackgroundSprite(const SoraWString& spr);
	uint32 update(float32 dt);
	
private:
	float32 camx, camy;
	float32 speed;
};


#endif