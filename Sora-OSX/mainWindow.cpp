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
	
	sora->beginScene(0x00000000);
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
		
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"Camera:(X=%f, Y=%f, Z=%f)", cx,cy,cz);

	sora::SORA->beginZBufferSort();
	
//	pSpr->update(sora::SORA->getDelta());
//	pSpr->render();
//	pressAnyKey->render(0, posy);
//	pSpr2->render(0, posy);
	pSpr->renderWithVertices(vert, 6, SORA_TRIANGLES_FAN);
	
	sora::SORA->endZBufferSort();

	sora->endScene();
	return false;
}


void mainWindow::init() {
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	
	pFont = sora::SORA->createFont(L"Bank Gothic Medium BT.ttf", 20);
	pFont->setColor(0xFFFFCC00);
	

		
	sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20);
	
	pSpr = sora::SORA->createSprite(L"background.png");
	pressAnyKey = sora::SORA->createSprite(L"road.png");
	pSpr2 = sora::SORA->createSprite(L"grass.png");
	
	
	float32 px = 400.f, py = 300.f;
	for(int i=0; i<6; ++i) {
		vert[i].col = 0xFFFFFFFF;
		vert[i].z = 0.f;
		
		vert[i].x = px + 300*cosf(sora::DGR_RAD(i*60));
		vert[i].y = py + 300*sinf(sora::DGR_RAD(i*60));
		
		vert[i].tx = (pSpr->getSpriteWidth() / 2 + 300*cosf(sora::DGR_RAD(i*60))) / pSpr->getTextureWidth(false);
		vert[i].ty = (pSpr->getSpriteHeight() / 2 + 300*sinf(sora::DGR_RAD(i*60))) / pSpr->getTextureHeight(false);
	}
	
	
	pSpr->setBlendMode(BLEND_DEFAULT_Z); pSpr->setZ(0.f);
	pressAnyKey->setBlendMode(BLEND_DEFAULT_Z); pressAnyKey->setZ(0.5f);
	pSpr2->setBlendMode(BLEND_DEFAULT_Z); pSpr2->setZ(0.6f); 
	
}

void mainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
		
		}
	}
}