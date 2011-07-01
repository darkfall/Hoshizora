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

#include "SoraGifSprite/SoraGifSprite.h"

sora::SoraGifSprite* gifSprite;


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
	//	pFont->setFontSize(pFont->getFontSize()+1);
	}
	
	if(sora->keyDown(SORA_KEY_LEFT))
		cx -= 2.f;
	if(sora->keyDown(SORA_KEY_RIGHT))
		cx += 2.f;
	if(sora->keyDown(SORA_KEY_UP))
		cy -= 2.f;
	if(sora->keyDown(SORA_KEY_DOWN))
		ps->emitS(L"astar.sps", sora::SORA->randomFloat(0.f, 800.f), sora::SORA->randomFloat(0.f, 600.f));
	
	if(sora->keyDown(SORA_KEY_O))
		sora::SoraBGMManager::Instance()->play(L"01.ogg", false);
	if(sora->keyDown(SORA_KEY_P))
		sora::SoraBGMManager::Instance()->playBGS(L"click_08.wav", 1, 1, 1.f, 0.5f);;

	
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
	
	
	{
		sora::PROFILE("p1");
		
		pFont->setColor(0xFFFFFFFF);	

	
	{
	//	ps->update(sora::SORA->getDelta());
	//	ps->render();
	}
        gifSprite->update(0.f);
        gifSprite->render();
        pFont->print(gifSprite->getPositionX()+gifSprite->getSpriteWidth()/2,
                     gifSprite->getPositionY()+gifSprite->getSpriteHeight(),
                     sora::FONT_ALIGNMENT_CENTER,
                     L"FrameRate: %d, Total Frame: %d",
                     gifSprite->getFrameRate(),
                     gifSprite->getFrameSize());
	//pCanvas->render();

	}
	
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"Camera:(X=%f, Y=%f, Z=%f)", cx,cy,cz);
	pFont->print(0.f, getWindowHeight()-60.f, sora::FONT_ALIGNMENT_LEFT, L"Alive Particles: %d, total %d", ps->size(), ps->getTotalParticleAlive());
	
	
	sora->endScene();
	return false;
}

void mainWindow::onMenuEvent(sora::SoraMenuBarClickEvent* ev) {
	sora::DebugPtr->log(ev->getItem()->getName());
}

void mainWindow::onDownloadEvent(sora::SoraHttpDownloadEvent* ev) {
	sora::SORA->messageBox(sora::vamssg("download from %s finished, recevied size = %.2fkb, received time = %.2f, download speed = %.2fkb/s", 
										ev->getURL().c_str(), 
										ev->getReceivedSize()/1024.f, 
										ev->getReceiveTime(), 
										ev->getDownloadSpeed()/1024.f),
										"test", MB_OK);
	file.writeToFile(L"./test.png");
}

void downloadEvent(sora::SoraHttpDownloadEvent* ev) {
	sora::SORA->messageBox(sora::vamssg("download from %s finished, recevied size = %.2fkb, received time = %.2f, download speed = %.2fkb/s", 
										ev->getURL().c_str(), 
										ev->getReceivedSize()/1024.f, 
										ev->getReceiveTime(), 
										ev->getDownloadSpeed()/1024.f),
						   "test", MB_OK);
}

void mainWindow::init() {
	registerEventFunc(this, &mainWindow::onMenuEvent);
	registerEventFunc(this, &mainWindow::onDownloadEvent);
	registerEventFunc(this, &mainWindow::onFileChangeEvent);
	registerEventFunc(downloadEvent);
	
	/*double testSize = sora::SoraHttpFile::getRemoteFileSize("http://www.gamemastercn.com/wp-content/uploads/2011/05/angel_600_338.png.pagespeed.ce.T4FzGASQ6s.png");
	if(testSize != 0.0) {
		sora::SORA->messageBox(sora::vamssg("%f", testSize), "test", MB_OK);
		file.setEventHandler(this);
		file.downloadFileTo("http://www.gamemastercn.com/wp-content/uploads/2011/05/angel_600_338.png.pagespeed.ce.T4FzGASQ6s.png", L"~/Desktop/download.png");
	}*/
	
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	sora::SORA->setSystemFont(L"cour.ttf", 16);
	
	sora::SORA->enableMenuBar(true);
	sora::SORA->setMenuBarShowAlways(false);
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
	
	ps = new sora::SoraParticleManager;
	ps->setGlobalSprite(sora::SORA->createSprite(L"pics/particles.png"));
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
	
	pSpr->setBlendMode(BLEND_DEFAULT_Z); pSpr->setZ(1.f);
	pressAnyKey->setBlendMode(BLEND_DEFAULT_Z); pressAnyKey->setZ(0.5f);
	pSpr2->setBlendMode(BLEND_DEFAULT_Z); pSpr2->setZ(0.0f); 
    
    gifSprite = new sora::SoraGifSprite;
    gifSprite->load(L"giftest.gif");
	gifSprite->setPosition(100.f, 100.f);
	
	sora::SORA_EVENT_MANAGER->registerFileChangeEventHandler(L"test.lua", this);
}

void mainWindow::onFileChangeEvent(sora::SoraFileChangeEvent* cev) {
	sora::SORA->messageBoxW(cev->getChangedFile().c_str(), L"test", MB_OK);
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
			
		}
	}
}