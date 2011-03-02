/*
 *  lScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "lScene.h"
#include "SoraEventManager.h"
#include "hash.h"

namespace projl {
	
	lScene::lScene() {
		setType(LSCENE_TYPE);
		
		registerEventFunc(this, &lScene::onSceneTimerEvent);
		
		pParentManager = 0;
		depth = 0.f;
	}
	
	lScene::~lScene() {
	}
	
	void lScene::add(SoraObject* obj) {
		obj->setPosition(getPositionX()+obj->getPositionX(), getPositionY()+obj->getPositionY());
		SoraObject::add(obj);
	}
		
	void lScene::render() {
		sora::SoraObject* pobj = getObjList();
		while(pobj != NULL) {
			if(pobj->type == SPRITE_TYPE) {
				sora::SoraSprite* pspr = (sora::SoraSprite*)pobj;
				pspr->setScale((float32)sceneRect.x2/pspr->getSpriteWidth(), (float32)sceneRect.y2/pspr->getSpriteHeight());
			}
			pobj->render();
			pobj = pobj->getNext();
		}
	}
	
	uint32 lScene::update(float32 dt) {
		sora::SoraObject* pobj = getObjList();
		while(pobj != NULL) {
			pobj->update(dt);
			pobj = pobj->getNext();
		}
		return 0;
	}
	
	float32 lScene::getSceneWidth() const {
		return sceneRect.x2;
	}
	
	float32 lScene::getSceneHeight() const {
		return sceneRect.y2;
	}
	
	float32 lScene::getScenePosX() const {
		return sceneRect.x1;
	}
	
	float32 lScene::getScenePosY() const {
		return sceneRect.y1;
	}
	
	void lScene::setDimension(float32 x, float32 y, float32 w, float32 h) {
		sceneRect.Set(x, y, w, h);
	}
	
	bool lScene::isActive() const {
		return bActive;
	}
	
	lSceneManager* lScene::getParentManager() const {
		return pParentManager;
	}
	
	lSceneManager::lSceneManager(float32 x, float32 y, float32 w, float32 h) {
		sora = sora::SoraCore::Instance();
		pSprBlackCover = new sora::SoraSprite((sora::SoraTexture*)sora->createTextureWH(w, h));
		pSprBlackCover->setPosition(x, y);
		pSprBlackCover->setColor(0x00FFFFFF);
		
		evToStart.id = EVENT_SWITCH_TO_START;
		evToHalf.id = EVENT_SWITCH_TO_HALF;
		evToFinish.id = EVENT_SWITCH_TO_FINISH;
		evFromStart.id = EVENT_SWITCH_FROM_START;
		evFromHalf.id = EVENT_SWITCH_FROM_HALF;
		evFromFinish.id = EVENT_SWITCH_FROM_FINISH;
		
		bSwitching = false;
		bSwitchHalf = false;
		
		registerEventFunc(this, &lSceneManager::onSceneTimerEvent);
		
		sceneRect.Set(x, y, w, h);
		
		bEffectEnabled = true;
	}
	
	lSceneManager::~lSceneManager() {
		delete pSprBlackCover;
	}
	
	void lSceneManager::setSwitchColor(const sora::SoraColorRGBA& color) {
		pSprBlackCover->setColor(CSETA(color.GetHWColor(), 0));
	}
	
	void lSceneManager::setSwitchEffectEnabled(bool flag) {
		bEffectEnabled = flag;
	}
	
	bool compSceneDepth(lScene* s1, lScene* s2) {
		return s1->getDepth() < s2->getDepth();
	}
	
	void lSceneManager::addScene(lScene* scene) {
		scene->setDimension(scene->getPositionX(), scene->getPositionY(), sceneRect.x2-scene->getPositionX(), sceneRect.y2-scene->getPositionY());

		scene->setPosition(sceneRect.x1+scene->getPositionX(), sceneRect.y1+scene->getPositionY());
		scenes.push_back(scene);
		currScene = scenes.begin();
		
		scene->pParentManager = this;
	}
	
	void lSceneManager::delScene(lScene* scene) {
		scenes.remove(scene);
	}
	
	void lSceneManager::update(float32 dt) {
		if(scenes.empty()) return;
		
		if(currScene != scenes.end())
			if((*currScene) != NULL)
				(*currScene)->update(dt);
		if(bSwitchHalf) {
			(*nextScene)->render();
		}
		if(bSwitching)
			pSprBlackCover->update(dt);
	}
	
	void lSceneManager::render() {
		if(currScene != scenes.end()) {
			if((*currScene) != NULL)
				(*currScene)->render();
		}
		if(bSwitchHalf) {
			(*nextScene)->render();
		}
		if(bSwitching)
			pSprBlackCover->render();
	}
	
	void lSceneManager::toNextScene(float32 switchTime) {
		if(currScene == scenes.end())
			return;
		
		nextScene = currScene;
		++nextScene;
		if(nextScene == scenes.end())
			return;
		
		this->switchTime = switchTime;
		switchToScene();
	}
	
	void lSceneManager::switchToScene() {
		if(switchTime == 0.f) {
			currScene = nextScene;
			(*currScene)->bActive = true;
			return;
		}
		
		evToStart.switchTime = switchTime;
		evFromStart.switchTime = switchTime;
		(*currScene)->onSceneTimerEvent(&evToStart);
		(*nextScene)->onSceneTimerEvent(&evFromStart);
		sora::SORA_EVENT_MANAGER->registerTimerEvent((*currScene), &evToHalf, switchTime/2); 
		sora::SORA_EVENT_MANAGER->registerTimerEvent((*currScene), &evToFinish, switchTime);
		sora::SORA_EVENT_MANAGER->registerTimerEvent((*nextScene), &evFromHalf, switchTime/2);
		sora::SORA_EVENT_MANAGER->registerTimerEvent((*nextScene), &evFromFinish, switchTime);
		sora::SORA_EVENT_MANAGER->registerTimerEvent(this, &evToHalf, switchTime/2);
		sora::SORA_EVENT_MANAGER->registerTimerEvent(this, &evToFinish, switchTime);

		bSwitching = true;
		
		if(bEffectEnabled) {
			pSprBlackCover->clearEffects();
			pSprBlackCover->addEffect(new sora::IEFade(0.f, 1.f, switchTime/4, sora::IMAGE_EFFECT_ONCE));		
		}
	}
	
	lSceneManager::SCENE_LIST::iterator lSceneManager::getSceneIterator(lScene* scene) {
		SCENE_LIST::iterator itScene = scenes.begin();
		while(itScene != scenes.end()) {
			if((*itScene == scene)) {
				return itScene;
			}
			++itScene;
		}
		return scenes.end();
	}
	
	void lSceneManager::switchToScene(lScene* scene, float32 switchTime) {
		if(currScene == scenes.end())
			return;
		
		nextScene = getSceneIterator(scene);
		if(nextScene == scenes.end())
			return;
		
		if(currScene == nextScene)
			return;
		
		this->switchTime = switchTime;
		switchToScene();
	}
	
	void lSceneManager::onSceneTimerEvent(const lSceneTimerEvent* tev) {
		if(tev->id == EVENT_SWITCH_FROM_HALF || tev->id == EVENT_SWITCH_TO_HALF) {
			bSwitchHalf = true;

			if(bEffectEnabled) {
				pSprBlackCover->clearEffects();
				pSprBlackCover->addEffect(new sora::IEFade(1.f, 0.f, switchTime/4, sora::IMAGE_EFFECT_ONCE));
			}
			(*currScene)->bActive = false;
			(*nextScene)->bActive = true;
			
			currScene = nextScene;

		} else if(tev->id == EVENT_SWITCH_TO_FINISH) {
			bSwitching = false;
			bSwitchHalf = false;
		}
	}
	
	
	lScene* lSceneManager::getCurrScene() {
		if(currScene != scenes.end())
			return (*currScene);
		return 0;
	}
	
	lScene* lSceneManager::getSceneByName(const SoraString& name) {
		uint32 hash = sora::BKDRHash(name.c_str());
		
		SCENE_LIST::iterator p = scenes.begin();
		while( p != scenes.end() ) {
			if((*p)->getNameHash() == hash)
				return (*p);
			++p;
		}
		return 0;
	}

} // namespace projl