/*
 *  playerScene.h
 *  Sora
 *
 *  Created by GriffinBu on 1/4/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef PLAYER_SCENE_H_
#define PLAYER_SCENE_H_

#include "SoraPlatform.h"
#include "SoraCore.h"
#include "lsAnimatedSprite.h"
#include "SoraINIFile/SoraINIFile.h"

class actorMontor;
class actorShooter;
class actorController;

class actor: public sora::SoraObject {
	friend class playerMontor;
	
public:
	actor();
	~actor();
	
	bool load(const SoraWString& anm);
	
	void setMontor(actorMontor* montor);
	void setShooter(actorShooter* shooter);
	void setController(actorController* controller);
	
private:
	sora::SoraCore* sora;
	ls::lsAnimatedSprite* actorSprite;
	
	actorMontor* montor;
	actorShooter* shooter;
	actorController* controller;
};

class actorMontor {
public:
	virtual void onMove(actor* player) = 0;
};

class actorShooter {
public:
	virtual void update(float32 dt) = 0;
	
	virtual void onShoot(actor* player) = 0;
	virtual void onBomb(actor* player) = 0;
};

class actorController {
public:
	virtual void onLoad(actor* player) = 0;
	virtual void onHit(actor* player) = 0;
};

class actorLoader {
public:
	virtual actor* loadPlayer(const SoraWString& configFile) = 0;
};

#endif