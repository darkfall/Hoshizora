/*
 *  basicSTGPlayerScene.h
 *  Sora
 *
 *  Created by GriffinBu on 1/5/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef BASIC_STG_PLAYER_SCENE_H_
#define BASIC_STG_PLAYER_SCENE_H_

#include "actor.h"
#include "keyMapper.h"
#include "stgkey.h"

class basicSTGPlayerMontor: public actorMontor {
public:
	basicSTGPlayerMontor(): vx(0.f), vy(0.f), vxs(0.f), vys(0.f) {
		km = keyMapper::Instance();
	}
	
	void setSpeedNormal(float32 _vx, float32 _vy) {
		vx = _vx;
		vy = _vy;
	}
	void setSpeedSlow(float32 _vxs, float32 _vys) {
		vxs = _vxs;
		vys = _vys;
	}
	
	void onMove(actor* player) {
		if(km->keyDown(STG_KEY_SLOW)) {
			if(km->keyDown(STG_KEY_LEFT)) {
				player->setPosition(player->getPositionX()-vxs, player->getPositionY());
			} else if(km->keyDown(STG_KEY_RIGHT)) {
				player->setPosition(player->getPositionX()+vxs, player->getPositionY());
			}
			if(km->keyDown(STG_KEY_UP)) {
				player->setPosition(player->getPositionX(), player->getPositionY()-vys);
			} else if(km->keyDown(STG_KEY_DOWN)) {
				player->setPosition(player->getPositionX(), player->getPositionY()+vys);
			}
		} else {
			if(km->keyDown(STG_KEY_LEFT)) {
				player->setPosition(player->getPositionX()-vx, player->getPositionY());
			} else if(km->keyDown(STG_KEY_RIGHT)) {
				player->setPosition(player->getPositionX()+vx, player->getPositionY());
			}
			if(km->keyDown(STG_KEY_UP)) {
				player->setPosition(player->getPositionX(), player->getPositionY()-vy);
			} else if(km->keyDown(STG_KEY_DOWN)) {
				player->setPosition(player->getPositionX(), player->getPositionY()+vy);
			}
		}
	}
	
	keyMapper* km;
	float32 vx, vy;
	float32 vxs, vys;
};

class basicSTGPlayerLoader: public actorLoader {
public:
	actor* loadPlayer(const SoraWString& configFile) {
		sora::SoraINIFile* pconfigfile = new sora::SoraINIFile;
		ulong32 size;
		void* pdata = sora::SoraCore::Instance()->getResourceFile(configFile, size);
		if(size != 0) {
			if(pconfigfile->readFileMem(pdata, size)) {
				actor* player = new actor;
				
				SoraString anmFile = pconfigfile->getString("", "anm", "");
				if(anmFile.size() != 0) {
					if(player->load(sora::s2ws(anmFile))) {
						
					}
				}
			}
		}
		return 0;
	}
};

#endif