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


#include "SoraSoundManager/SoraBGMManager.h"
#include "SoraSoundManager/SoundManagerLuaExport.h"

#include "SoraSpriteAnimation/AnimationLuaExport.h"

#include "SoraLua/SoraLuaExport.h"

#include "SoraPNGOps/SoraCompressedTexture.h"

#include "SoraGUIChan/Modifiers/CloseModifier.h"

#include "SoraPNGOps/SoraPNGOptimizer.h"
#include "Debug/SoraAutoProfile.h"

#include "cmd/SoraConsole.h"


mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
	
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
	
}

mainWindow::~mainWindow() {
}
int32 posy = 0;



float32 cx = 300.f, cy = 400.f, cz = 1.f;
void transform3d(float32& x, float32& y, float32 z) {
	float scale = (cz - z) / cz;
	x = (x - cx) * scale + cx;
	y = (y - cy) * scale + cy;
	z = 0.f;
}

bool mainWindow::updateFunc() {
	
	if(sora->keyDown(SORA_KEY_ESCAPE))
		sora->shutDown();
	if(sora->keyDown(SORA_KEY_DOWN)) {
		++posy;
		
	//	sora::SORA->releaseFont(pFont);
		pFont->setFontSize(pFont->getFontSize()+1);
	}
	
	if(sora->keyDown(SORA_KEY_LEFT))
		cx -= 2.f;
	if(sora->keyDown(SORA_KEY_RIGHT))
		cx += 2.f;
	if(sora->keyDown(SORA_KEY_UP))
		cy -= 2.f;
	if(sora->keyDown(SORA_KEY_DOWN))
		cy += 2.f;
	
	if(sora->keyDown(SORA_KEY_O))
		cz += 0.1f;
	if(sora->keyDown(SORA_KEY_P))
		cz -= 0.1f;

	
    return false;
}
void renderBottom() {
	
}

sora::SoraVertex vert[6];


bool mainWindow::renderFunc() {
	pCanvas->beginRender();
	sora::SORA->beginZBufferSort();

	pressAnyKey->render();
	pSpr2->render();
	pSpr->render();
	sora::SORA->endZBufferSort();

	pCanvas->finishRender();
	
	sora->beginScene(0);
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
		
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"Camera:(X=%f, Y=%f, Z=%f)", cx,cy,cz);

	{
		sora::PROFILE("p1");
		
	
	
	{
		ps->update(sora::SORA->getDelta());
		ps->render();
	}
	
	pCanvas->render();

	}
	
	sora->endScene();
	return false;
}

void onDownloadFinish(sora::SoraHttpFile* file){
	sora::SORA->log(sora::vamssg("Download finished from %s, size = %lu, buffersize=%lu,%lu, time = %f", file->getURL().c_str(), file->getDownloadedSize(), file->getMemoryBuffer()->size(), file->getMemoryBuffer()->realsize(), file->getDownloadTime()),
					sora::LOG_LEVEL_NORMAL);
	file->writeToFile(L"./lalalalal.png");
	file->closeFile();
}

void mainWindow::onMenuEvent(sora::SoraMenuBarClickEvent* ev) {
	sora::DebugPtr->log(ev->getItem()->getName());
}

void mainWindow::init() {
	registerEventFunc(this, &mainWindow::onMenuEvent);
	
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	sora::SORA->setSystemFont(L"cour.ttf", 16);
	
	sora::SORA->enableMenuBar(true);
	sora::SORA->setMenuBarShowAlways(true);
	sora::SoraMenuBarMenu* menu1 = new sora::SoraMenuBarMenu(L"File");
	menu1->addItem(L"Open", this);
	menu1->addItem(L"Save", this);
	
	sora::SORA->addMenu(menu1);
	
	sora::SoraMenuBarMenu* menu2 = new sora::SoraMenuBarMenu(L"Edit");
	menu2->addItem(L"Find", this);
	menu2->addItem(L"Replace", this);
	
	sora::SORA->addMenu(menu2);
	
	pFont = sora::SORA->createFont(L"cour.ttf", 16);
	pFont->setColor(0xFFFFCC00);
	
	pCanvas = new sora::SoraBaseCanvas(800, 600);
	
	sora::GCN_GLOBAL->initGUIChan(L"Bank Gothic Medium BT.ttf", 16);
	
	pSpr = sora::SORA->createSprite(L"background.png");
	pressAnyKey = sora::SORA->createSprite(L"road.png");
	pSpr2 = sora::SORA->createSprite(L"grass.png");
	
	pSpr2->setZ(0.f); 
	pressAnyKey->setZ(0.5f); 	
	pSpr->setZ(1.0); 
	
	ps = new sora::SoraParticleManager;
	ps->setGlobalSprite(sora::SORA->createSprite(L"pics/particles.png"));
	ps->emitS(L"bg13_fire_left.sps", 100.f, 100.f);
	ps->emitS(L"astar.sps", 200.f, 100.f);
	
	float32 px = 400.f, py = 300.f;
	for(int i=0; i<6; ++i) {
		vert[i].col = 0xFFFFFFFF;
		vert[i].z = 0.f;
		
		vert[i].x = px + 300*cosf(sora::DGR_RAD(i*60));
		vert[i].y = py + 300*sinf(sora::DGR_RAD(i*60));
		
		vert[i].tx = (pSpr->getSpriteWidth() / 2 + 300*cosf(sora::DGR_RAD(i*60))) / pSpr->getTextureWidth(false);
		vert[i].ty = (pSpr->getSpriteHeight() / 2 + 300*sinf(sora::DGR_RAD(i*60))) / pSpr->getTextureHeight(false);
	}
	
	uint8 localIp[4] = {192, 168, 1, 103};
	if(!psocket.Initialize())
		sora::DebugPtr->log("Error initializing passive socket");
	if(!psocket.Listen(localIp, 3000))
		sora::DebugPtr->log(sora::vamssg("Passive Socket Error, error code = %d", psocket.GetSocketError()));
	
	if(!asocket.Initialize())
		sora::DebugPtr->log("Error initializing active socket");
	if(!asocket.Open(localIp, 3000))
		sora::DebugPtr->log(sora::vamssg("Active Socket Error, error code = %d", asocket.GetSocketError()));
	
	
	pSpr->setBlendMode(BLEND_DEFAULT_Z); pSpr->setZ(0.f);
	pressAnyKey->setBlendMode(BLEND_DEFAULT_Z); pressAnyKey->setZ(0.5f);
	pSpr2->setBlendMode(BLEND_DEFAULT_Z); pSpr2->setZ(0.6f); 
	
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
		
		}
	}
}