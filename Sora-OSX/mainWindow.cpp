/*
 *  mainWindow.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "mainWindow.h"
#include "SoraEventManager.h"
#include "SoraLocalizer.h"

#include "graphicEffects.h"

#include "SoraGUIChan/guichansetup.h"
#include "gcnExtend/gcnDraggableImageButtonIncubator.h"
#include "gcnExtend/gcnSelectableContainer.h"
#include "gcnExtend/gcnDraggableIcon.h"
#include "gcnExtend/gcnConnectable.h"


#include "../ReflectionRev/rfMap.h"
#include "../ReflectionRev/rfShape.h"


mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
	
	registerEventFunc(this, &mainWindow::onMenuClick);
	
	sora::SORA_EVENT_MANAGER->registerEvent("MainWindow", this, 0);
}

mainWindow::~mainWindow() {
}

bool mainWindow::updateFunc() {
	
	if(sora->keyDown(SORA_KEY_ESCAPE))
		sora->shutDown();
	if(sora->keyDown(SORA_KEY_F))
		sora->setFullscreen(false);
    
    if(sora->keyDown(SORA_KEY_1)) {
		reflection::rfMap* map = (reflection::rfMap*)(sora::GCN_GLOBAL->findWidget("map"));
		if(map) {
			map->writeConfig(L"mapconf.txt");
			map->write(L"lalala.txt");
		}
	} 
	
	sora::SoraKeyEvent ev;
	if(sora->getKeyEvent(ev)) {
		if(ev.key == SORA_KEY_2) {
			reflection::rfMap* map = (reflection::rfMap*)(sora::GCN_GLOBAL->findWidget("map"));
			if(map) {
				map->loadConfig(L"mapconf.txt");
				map->load(L"lalala.txt");
			}
		}
	}

	
    return false;
}

sora::SoraSprite* pSpr2;

bool mainWindow::renderFunc() {
	sora->beginScene();
	
	//mainScenes->render();
	//pSpr->render4V(100.f, 100.f, 700.f, 0.f, 700.f, 600.f, 100.f, 500.f);
   // sora->setViewPoint(0.f, 0.f, 1.f);
  	
 //   pSpr->render(0.f, 0.f);
	//pSpr2->render(100.f, 100.f);
	float t = 0.56;
	//shader->setParameterfv("twisting", &t, 1);
    
	
 //   pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());

	
	
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
	
	sora->endScene();
    

	return false;
}

sora::SoraSprite* mirrorImage;

gcn::Widget* myIncubateFunc_hexogon(void* userData) {
	reflection::rfShapeBox* box = new reflection::rfShapeBox;
	
	reflection::rfMap* map = (reflection::rfMap*)(userData);
	if(map) {
		box->setShapeSpriteContainer(map->getShapeSpriteContainer());
	}
	
	box->setEdgeNum(6);
	
	return box;
}

void mainWindow::init() {
    sora::SORA->setFPS(999);
 //   pSpr = sora::SORA->createSprite(L"titlebg2.png");
//	pSpr2 = sora::SORA->createSprite(L"sea.png");
	
//	pSpr2->setScale(0.5f, 0.5f);
	//shader = pSpr2->attachShader(L"C3E2v_varying.cg", "C3E2v_varying", sora::VERTEX_SHADER);
//	pSpr2->attachShader(L"gray.ps", "gray", sora::FRAGMENT_SHADER);
    
    pFont = sora::SORA->createFont(L"ARIALN.ttf", 20);
	
	if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
		sora::GCN_GLOBAL->createTop();
			
		reflection::rfMap* map = new reflection::rfMap;
		map->setId("map");
		map->setDimension(gcn::Rectangle(0, 0, getWindowWidth(), getWindowHeight()));
		map->setImage("background1.png");
		map->setState(reflection::rfMap::STATE_EDITING);
		
		map->setMirrorImage(L"mirror1.png");
		
		map->pushSprite(4, reflection::rfShapeSprite("box.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(0, 0, 64, 64)));
		map->pushSprite(6, reflection::rfShapeSprite("boxbutton.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(64, 0, 64, 64)));
	
		sora::GCN_GLOBAL->addWidget(map, "top");
					
		gcn::DraggableImageButtonIncubator* di = new gcn::DraggableImageButtonIncubator();
		di->setImage("boxbutton.png", gcn::Rectangle(0, 64, 64, 64), gcn::Rectangle(0, 64, 64, 64)); 
		di->setSelectionColor(gcn::Color(255, 255, 255, 155));
		di->registerIncubatorFunc(myIncubateFunc_hexogon);
		di->setUserData(map);
		di->setAddParent(map);
		sora::GCN_GLOBAL->addWidget(di, "top");
	}
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);

}