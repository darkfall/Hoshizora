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

#include "SoraSoundManager/SoraBGMManager.h"

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
		sora->setFullscreen(true);
    
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
		} else if(ev.key == SORA_KEY_N) {
			sora::SoraBGMManager::Instance()->toPrevBGM();
		} else if(ev.key == SORA_KEY_M) {
			sora::SoraBGMManager::Instance()->toNextBGM();
		} else if(ev.key == SORA_KEY_B) {
			sora::SoraBGMManager::Instance()->pause();
		} else if(ev.key == SORA_KEY_V) {
			sora::SoraBGMManager::Instance()->resume();
		}
	}

    return false;
}

sora::SoraSprite* pSpr2;

bool mainWindow::renderFunc() {
	
	//mainScenes->render();
	//pSpr->render4V(100.f, 100.f, 700.f, 0.f, 700.f, 600.f, 100.f, 500.f);
   // sora->setViewPoint(0.f, 0.f, 1.f);
  	
 	float t = 0.56;
	//shader->setParameterfv("twisting", &t, 1);

	canvas1->beginRender();
	
//	pSpr->render(0.f, 0.f);
//	pSpr2->render(100.f, 100.f);
	
	canvas1->finishRender();
		
	sora->beginScene();
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
	
	canvas1->render();
	
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	
	reflection::rfMap* map = (reflection::rfMap*)sora::GCN_GLOBAL->findWidget("map");
	if(map) {
		pFont->print(getWindowWidth()-10.f, 0.f, sora::FONT_ALIGNMENT_RIGHT, L"LightSource: %d", map->getLightSourceNumber());
		pFont->print(getWindowWidth()-10.f, 15.f, sora::FONT_ALIGNMENT_RIGHT, L"Mirror: %d", map->getMirrorNumber());
		pFont->print(getWindowWidth()-10.f, 30.f, sora::FONT_ALIGNMENT_RIGHT, L"Light: %d", map->getLightNumber());
		pFont->print(getWindowWidth()-10.f, 45.f, sora::FONT_ALIGNMENT_RIGHT, L"Shape: %d", map->getShapeBoxNumber());
		pFont->print(getWindowWidth()-10.f, 60.f, sora::FONT_ALIGNMENT_RIGHT, L"LightedShape: %d", map->getLightedBoxNumber());
	}
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
	
	box->enableFrameWhenMouseEntered(true);
	box->setClickSound("start.wav");
	box->setEnterSound("click_08.wav");
	
	box->setEdgeNum(6);
	
	return box;
}

void mainWindow::init() {
    sora::SORA->setFPS(999);
    pSpr = sora::SORA->createSprite(L"titlebg2.png");
	pSpr->setScale(0.3f, 0.3f);
		pSpr->setBlendMode(pSpr->getBlendMode() | BLEND_ZWRITE);
	pSpr->setZ(0.5);
	pSpr2 = sora::SORA->createSprite(L"sea.png");
	pSpr2->setBlendMode(pSpr2->getBlendMode() | BLEND_ZWRITE);
	pSpr2->setZ(0.3);
//	pSpr2->setScale(0.5f, 0.5f);
	//shader = pSpr2->attachShader(L"C3E2v_varying.cg", "C3E2v_varying", sora::VERTEX_SHADER);
//	pSpr2->attachShader(L"gray.ps", "gray", sora::FRAGMENT_SHADER);
    
    pFont = sora::SORA->createFont(L"ARIALN.ttf", 20);
	pFont->setColor(0xFFFFFFFF);
	
	if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
		sora::GCN_GLOBAL->createTop();
			
		reflection::rfMap* map = new reflection::rfMap;
		map->setId("map");
		map->setDimension(gcn::Rectangle(0, 0, getWindowWidth(), getWindowHeight()));
		map->setImage("background1.png");
		map->setState(reflection::rfMap::STATE_EDITING);
		
		map->setLightSourceImage(L"boxhl.png");
		
		map->setMirrorImage(L"mirror1.png");
		
		map->pushSprite(4, reflection::rfShapeSprite("box.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(0, 0, 64, 64)));
		map->pushSprite(6, reflection::rfShapeSprite("boxbutton.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(64, 0, 64, 64)));
	
		sora::GCN_GLOBAL->addWidget(map, "top");
		
		reflection::rfLightSource* ls = new reflection::rfLightSource;
		ls->setDimension(gcn::Rectangle(100, 100, 10, 10));
	//	ls->setImage("boxhl.png");
		ls->setFacing(sora::DGR_RAD(-90));
		map->add(ls);
					
		gcn::DraggableImageButtonIncubator* di = new gcn::DraggableImageButtonIncubator();
		di->setImage("boxbutton.png", gcn::Rectangle(0, 64, 64, 64), gcn::Rectangle(0, 64, 64, 64)); 
		di->setSelectionColor(gcn::Color(255, 255, 255, 155));
		di->registerIncubatorFunc(myIncubateFunc_hexogon);
		di->setUserData(map);
		di->setAddParent(map);
		sora::GCN_GLOBAL->addWidget(di, "top");
		
		canvas1 = new sora::SoraBaseCanvas(getWindowWidth(), getWindowHeight(), true);
		rt1 = sora::SORA->createTarget(getWindowWidth(), getWindowHeight());
	}
	
	sora::SoraBGMManager::Instance()->play(L"01.ogg", true);
	sora::SoraBGMManager::Instance()->play(L"02.mp3", true);
	sora::SoraBGMManager::Instance()->setFadeTime(1.f, 1.f);
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);

}