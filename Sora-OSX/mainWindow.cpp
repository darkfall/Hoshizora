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
#include "SoraGenericObjects/SoraCustomShapeSprite.h"

#include "../MEAD/bulletLuaExport.h"
#include "SoraSoundManager/SoundManagerLuaExport.h"


#include "SoraThread/SoraThreadPool.h"

#include "SoraThread/SoraCountDownLatch.h"
#include "SoraThread/SoraBlockingQueue.h"

#include "SoraBox2D/SoraPhysicalWorld.h"
#include "SoraBox2D/SoraPhysicalObject.h"

#include "SoraShaderManager.h"

sora::SoraGifSprite* gifSprite;
sora::SoraCustomShapeSprite* customSprite;

mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
    
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
    
}

mainWindow::~mainWindow() {
}
int32 posy = 0;
bool mouseReleased = true;


float32 cx = 300.f, cy = 400.f, cz = 1.f;
void transform3d(float32& x, float32& y, float32 z) {
	float scale = (cz - z) / cz;
	x = (x - cx) * scale + cx;
	y = (y - cy) * scale + cy;
	z = 0.f;
}

bool mainWindow::updateFunc() {
    
    if(sora->keyDown(SORA_KEY_LBUTTON)) {
        if(mouseReleased) {
            float32 x, y;
            sora::SORA->getMousePos(&x, &y);
            mouseReleased = false;
        }
    } else 
        mouseReleased = true;
    
	if(sora->keyDown(SORA_KEY_ESCAPE))
		sora->shutDown();
	if(sora->keyDown(SORA_KEY_DOWN)) {
		pSpr->setScale(pSpr->getHScale()+0.01, pSpr->getVScale()+0.01);
        
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
    
    mead::globalBulletManagerUpdate();
    //    sora::SORA->setCursor("./icon.icns");
    return false;
}

sora::SoraPhysicalObject* py1;
sora::SoraPhysicalObject* py2;
sora::SoraSprite* mEdge;

bool mainWindow::renderFunc() {
	pCanvas->beginRender(0x000000FF);
	sora::SORA->beginZBufferSort();
    pSpr2->render();
    
    pressAnyKey->render();
    
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
        
        //   customSprite->render();
        py1->update(sora::SORA->getDelta());
        py2->update(sora::SORA->getDelta());
        py1->render();
        py2->render();
        //platformerGeo->update(sora::SORA->getDelta());
        //platformerGeo->render();
        mEdge->render(0.f, 0.f);
	}
	//obj.update(sora::SORA->getDelta());
    
    //pFont->setLineRotation(90.f, true);
    
	pFont->print(900, 20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
    
   // pFont->setLineRotation(0.f);
    
	pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"Camera:(X=%f, Y=%f, Z=%f)", cx,cy,cz);
	pFont->print(0.f, getWindowHeight()-60.f, sora::FONT_ALIGNMENT_LEFT, L"Alive Particles: %d, total %d", ps->size(), ps->getTotalParticleAlive());
    
    pFont->print(0.f, getWindowHeight()-100.f, sora::FONT_ALIGNMENT_LEFT, L"Body: %d", sora::SoraPhysicalWorld::Instance()->getBodyCount());
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


void downloadDelegate(sora::SoraHttpFile& file) {
    sora::SORA->messageBox(sora::vamssg("downloadtime: %d", file.getDownloadTime()), "test", MB_OK);
}


#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"

void mainWindow::onScreenBufferRender(ulong32& tex) {
    pScreenSpr->setTexture(tex);
    pScreenSpr->render();
}

void mainWindow::init() {
    sora::SoraPhysicalWorld::Instance()->initBox2DWorld(0.f, 1.f);

	registerEventFunc(this, &mainWindow::onMenuEvent);
	registerEventFunc(this, &mainWindow::onDownloadEvent);
	registerEventFunc(this, &mainWindow::onFileChangeEvent);
    
    sora::SORA->registerFullscreenBufferDelegate(sora::DelegatePtr(this, &mainWindow::onScreenBufferRender));
    //sora::SORA->enableFullscreenBuffer(true);
	/*file.downloadFileWithDelegate("http://www.gamemastercn.com/wp-content/uploads/2011/05/angel_600_338.png.pagespeed.ce.T4FzGASQ6s.png", Delegate(downloadDelegate).clone());*/
    
	/*double testSize = sora::SoraHttpFile::getRemoteFileSize("http://www.gamemastercn.com/wp-content/uploads/2011/05/angel_600_338.png.pagespeed.ce.T4FzGASQ6s.png");
     if(testSize != 0.0) {
     sora::SORA->messageBox(sora::vamssg("%f", testSize), "test", MB_OK);
     file.setEventHandler(this);
     file.downloadFileTo("http://www.gamemastercn.com/wp-content/uploads/2011/05/angel_600_338.png.pagespeed.ce.T4FzGASQ6s.png", L"~/Desktop/download.png");
     }*/
    //sora::SORA->enableFullscreenBuffer(false);
    
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	sora::SORA->setSystemFont(L"cour.ttf", 16);
    
	sora::SORA->enableMenuBar(true);
    sora::SoraMenuBar* menuBar = sora::SORA->getMenuBar();
	menuBar->setShowAlways(false);
    
    sora::SoraMenuBarMenu* menu1 = new sora::SoraMenuBarMenu(L"File");
	menu1->addItem(L"Open", this);
	menu1->addItem(L"Save", this);
    
	menuBar->addMenu(menu1);
    
	sora::SoraMenuBarMenu* menu2 = new sora::SoraMenuBarMenu(L"Edit");
	menu2->addItem(L"Find", this);
	menu2->addItem(L"Replace", this);
    
	menuBar->addMenu(menu2);
    
	pFont = sora::SORA->createFont(L"cour.ttf", 16);
	pFont->setColor(0xFFFFCC00);
    
	pCanvas = new sora::SoraBaseCanvas(800, 600);
    
	sora::GCN_GLOBAL->initGUIChan(L"Bank Gothic Medium BT.ttf", 16);
    
	pSpr = sora::SORA->createSprite(L"background.png");

    //pSpr->setCenter(pSpr->getSpriteWidth()/2, pSpr->getSpriteHeight()/2);
    //pSpr->setScale(3.f, 3.f);
    customSprite = new sora::SoraCustomShapeSprite(pSpr, sora::SORA_TRIANGLES);
    
    
	pressAnyKey = sora::SORA->createSprite(L"road.png");
	pSpr2 = sora::SORA->createSprite(L"grass.png");
    
    pScreenSpr = new sora::SoraSprite(NULL);
    sora::SoraShader* shader = pScreenSpr->attachShader(L"HDRBlur.cg", "HDRBlur", sora::FRAGMENT_SHADER);
    if(!shader) {
        sora::SORA->messageBox("sdsd", "Sd", MB_OK);
    }
    pSpr->setBlendMode(BLEND_DEFAULT_Z); pSpr->setZ(1.f);
    pressAnyKey->setBlendMode(BLEND_DEFAULT_Z); pressAnyKey->setZ(0.5f);
    pSpr2->setBlendMode(BLEND_DEFAULT_Z); pSpr2->setZ(0.f);
    
	ps = new sora::SoraParticleManager;
	ps->setGlobalSprite(sora::SORA->createSprite(L"pics/particles.png"));
	ps->emitS(L"astar.sps", 200.f, 100.f);
    
    
    gifSprite = new sora::SoraGifSprite;
    gifSprite->load(L"giftest.gif");
    gifSprite->setFrameRate(24);
	gifSprite->setPosition(100.f, 100.f);
    
	sora::SORA_EVENT_MANAGER->registerFileChangeEventHandler(L"test.lua", this);

    sora::SoraPhysicalWorld::Instance()->initBox2DWorld(0.f, 1.f);
    
    mead::exportBulletManager(obj.getState());
    mead::exportGlobal(obj.getState());
    sora::exportSoundManager(obj.getState());
    mead::globalBulletManagerInit();
    obj.doScript(L"mybullettest.lua");
    
    
    
    py1 = sora::RenderablePhysicalOject(200.f, 200.f, sora::B2CreateBox(200.f, 150.f), sora::SORA->createSprite(L"geotest.png"));
    //py2 = sora::RenderablePhysicalOject(100.f, 0.f, sora::B2CreateBox(400, 301, 500, 0), sora::SORA->createSprite(L"geotest.png"));
    
    py1->enableDrawBoundingBox(true);
    //py2->enableDrawBoundingBox(true);
    
    mEdge = new sora::SoraSprite(NULL);
    mEdge->setColor(0xFFFFFFFF);
    mEdge->setTextureRect(0, 0, 800, 20);
    py2 = sora::RenderablePhysicalOject(0.f,0.f, sora::B2CreateEdge(0.f, 0.f, 1024.f, 0.f), mEdge, false);
    py2->createFixture(sora::B2CreateEdge(0.f, 0.f, 0.f, 800.f), 1.f);
    py2->createFixture(sora::B2CreateEdge(980.f, 0.f, 980, 800.f), 1.f);
    py2->createFixture(sora::B2CreateEdge(0.f, 700.f, 1024.f, 700.f), 1.f);
    py2->enableDrawBoundingBox(true);

}

void mainWindow::onFileChangeEvent(sora::SoraFileChangeEvent* cev) {
	sora::SORA->messageBoxW(cev->getChangedFile().c_str(), L"test", MB_OK);
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
	if(kev->isKeyPressed(SORA_KEY_1))
        customSprite->setRenderMode(sora::SORA_TRIANGLES);
    else if(kev->isKeyPressed(SORA_KEY_2))
        customSprite->setRenderMode(sora::SORA_TRIANGLES_FAN);
    else if(kev->isKeyPressed(SORA_KEY_3))
        customSprite->setRenderMode(sora::SORA_TRIANGLES_STRIP);
    else if(kev->isKeyPressed(SORA_KEY_4))
        customSprite->setRenderMode(sora::SORA_QUAD);
    else if(kev->isKeyPressed(SORA_KEY_5))
        customSprite->clearVertices();
    else if(kev->isKeyPressed(SORA_KEY_6))
        customSprite->saveVertciesToFile(L"vertices.raw");
    else if(kev->isKeyPressed(SORA_KEY_7)) {
        if(!customSprite->loadVerticesFromFile(L"vertices.raw"))
            sora::SORA->messageBox("sc", "sc", MB_OK);
    }
    
}