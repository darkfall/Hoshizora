/*
 *  titleScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "titleScene.h"
#include "SoraINIFile/SoraINIFile.h"
#include "SoraEventManager.h"

class lShineMenuSceneWithScaleEff: public lShineMenuScene {
public:
	lShineMenuSceneWithScaleEff(float32 x, float32 y, sora::SoraFont* font=0): lShineMenuScene(x, y, font) {
		basex = x;
		basey = y;
	}
	void onSceneTimerEvent(const projl::lSceneTimerEvent* tev) {
		if(tev->id == projl::EVENT_SWITCH_FROM_START) {
				clearEffects();

				/*setPosition(basex, basey);
				addEffect(new sora::IETransitions(getPositionX(), 
												  getPositionY()-100, 
												  getPositionX(), 
												  getPositionY(), 
												  tev->switchTime/2));
			*/
				addEffect(new sora::IEScale(0.f, 1.f, tev->switchTime/4));
				addEffect(new sora::IEFade(0.f, 1.f, tev->switchTime/4));
			
		} else if(tev->id == projl::EVENT_SWITCH_TO_START) {
				clearEffects();
			
				/*setPosition(basex, basey);
				addEffect(new sora::IETransitions(getPositionX(), 
												  getPositionY(), 
												  getPositionX(), 
												  getPositionY()+100, 
												  tev->switchTime/2));*/
				addEffect(new sora::IEScale(1.f, 0.f, tev->switchTime/4));
				addEffect(new sora::IEFade(1.f, 0.f, tev->switchTime/4));
		
		} else if(tev->id == projl::EVENT_SWITCH_TO_FINISH) {
		}
	}
	
	void setBasePosition(float32 bx, float32 by) {
		basex = bx;
		basey = by;
	}
	
private:
	float32 basex, basey;
	
};

titleScene::titleScene() {
	sora = sora::SoraCore::Instance();
	
	bg = new backgroundScene(0.f, 0.f, sora->getScreenWidth(), sora->getScreenHeight());
	bg->setPosition(0.f, 0.f);
	add(bg);
	
	menus = new menuSceneManager(0.f, 0.f);
	
	init();
	
	//registerEventFunc(this, &titleScene::onSceneTimerEvent);
	registerEventFunc(this, &titleScene::onTimerEvent);
}

titleScene::~titleScene() {
	delete bg;
	delete menus;
}

void titleScene::init() {
	bg->setBackgroundSprite(L"titlebg2.png");

	//sora::SoraFont* pMenuFont = sora->createFont(L"ARIALN.TTF", 40);
	lShineMenuSceneWithScaleEff* mainMenu = new lShineMenuSceneWithScaleEff(400.f, 350.f, sora::SORA->createFont(L"ARIALN.TTF", 30));

	mainMenu->addItemSpr(L"title.png", 0, 0, 225, 75);
	mainMenu->addItemSpr(L"title.png", 0, 75, 225, 75);
	//mainMenu->addItem("Start");
	//mainMenu->addItem("Option");
	
	titleMenuId = menus->addMenuScene(mainMenu);
	
	lShineMenuSceneWithScaleEff* difficultyMenu = new lShineMenuSceneWithScaleEff(400.f, 350.f);
	difficultyMenu->addItemSpr(L"title.png", 225, 0, 226, 75);
	difficultyMenu->addItemSpr(L"title.png", 225, 75, 225, 75);
	
	difficultyMenuId = menus->addMenuScene(difficultyMenu);
	
	menus->switchToMenu(titleMenuId, 0.f);
	
	bAvailable = true;
}

void titleScene::render() {
	//bg->render();
	lScene::render();

	menus->render();
}

uint32 titleScene::update(float32 dt) {
	//bg->update(dt);
	lScene::update(dt);
	
	menus->update(dt);
	
	if(bAvailable) {
		if(sora->keyDown(SORA_KEY_Z)) {
			uint32 menuid = menus->getCurrActiveMenu();
			if(menuid == titleMenuId) {
				switch(menus->getCurrActiveMenuItem()) {
					case 0: menus->switchToMenu(difficultyMenuId, 0.5f); break;
				//	case 1: break;
					case 1: sora->shutDown(); break;
				}
			} else if(menuid == difficultyMenuId) {
				sora::SORA_EVENT_MANAGER->sendMessage("MainWindow", new menuEvent("difficulty", menus->getCurrActiveMenuItem()));
			}
			
			bAvailable = false;
			sora::SORA_EVENT_MANAGER->createTimerEvent(this, DEFAULT_MENU_RESPONSE_TIME);
		} else if(sora->keyDown(SORA_KEY_X)) {
			if(menus->getCurrActiveMenu() == difficultyMenuId) {
				menus->switchToMenu(titleMenuId, 0.5f);
			}
			
		}
	}
	
	return 0;
}

void titleScene::onSceneTimerEvent(const projl::lSceneTimerEvent* tev) {
	bAvailable = true;
}

void titleScene::onTimerEvent(const sora::SoraTimerEvent* tev) {
	//printf("%f\n", tev->getTime());
	bAvailable = true;
}