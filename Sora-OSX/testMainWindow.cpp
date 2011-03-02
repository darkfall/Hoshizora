/*
 *  testMainWindow.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/22/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#include "testMainWindow.h"
#include "SoraFont.h"

#include "SoraEventManager.h"
#include "SoraLocalizer.h"

float32 waveToCol(float32* waveData, uint32 size, float32& u, float32& d) {
	int up = 0, down = 0;
	float32 upval = 0.f, downval = 0.f;
	for(int i=0; i<size; ++i) {
		if(waveData[i] > 0.f) {
			++up;
			upval += waveData[i];
		} else if(waveData[i] < 0.f) {
			++down;
			downval += waveData[i];
		}
	}
	u = upval;
	d = downval;
	return upval+downval;
}

bool testMainWindow::updateFunc() { 
	pAnimatedSprite->update(CORE->getDelta());
	/*
	FMOD_System_Update(sys);
	if(bgm) {
		float ddd[16];
		FMOD_Channel_GetWaveData(cha, ddd, 16, 0);
		float32 data, data1, t;
		t = waveToCol(ddd, 16, data, data1);
		printf("%f, %f\n", data, data1);
		
		//par->setBaseColor(sora::SoraColorRGBA(t, 0.f, 0.f, 0.f));
	
		par->pheader.dwMinStartColor = (sora::SoraColorRGBA(sinf(ddd[0]+ddd[4]), sinf(ddd[1]+ddd[5]), sinf(ddd[2]+ddd[6]), sinf(ddd[3]+ddd[7])));
		par->pheader.dwMaxStartColor = (sora::SoraColorRGBA(sinf(ddd[0]+ddd[4]), sinf(ddd[1]+ddd[5]), sinf(ddd[2]+ddd[6]), sinf(ddd[3]+ddd[7])));
		
		par->pheader.dwMinEndColor = par->pheader.dwMinStartColor;
		par->pheader.dwMaxEndColor = par->pheader.dwMaxStartColor;

		par->pheader.nEmitNum = 1000.f*(int)abs(t);
	}
	*/
	par->update(CORE->getDelta());
	
	sora::GCN_GLOBAL->gcnLogic();
	sceneManager->update(CORE->getDelta());
	
	//if(par) par->update(CORE->getDelta());
	return false;
}

bool testMainWindow::renderFunc() {
	CORE->beginScene();
	
	font->print(0, 0, sora::FONT_ALIGNMENT_LEFT, L"fps: %f\npar: %d", CORE->getFPS(), par->getLiveParticle());
	font->render(getWindowWidth()-font->getStringWidth(L"Up/Down: roll"), 0, sora::FONT_ALIGNMENT_CENTER, L"Up/Down: roll\nLeft/Right: pitch\nComma/Period: yaw");
	font->print(getWindowWidth(), getWindowHeight()-font->getHeight()*3, sora::FONT_ALIGNMENT_RIGHT, L"Resolution: %d*%d\n%s", getWindowWidth(), getWindowHeight(), CORE->videoInfo().c_str());
	
	/*
	if(CORE->keyDown(SORA_KEY_UP)) {
		rr += 1.f;
	} 
	if(CORE->keyDown(SORA_KEY_DOWN)) {
		rr -= 1.f;
	}
	if(CORE->keyDown(SORA_KEY_LEFT)) {
		rp += 1.f;
	}
	if(CORE->keyDown(SORA_KEY_RIGHT)) {
		rp -= 1.f;
	}
	if(CORE->keyDown(SORA_KEY_COMMA)) {
		ry += 1.f;
	}
	if(CORE->keyDown(SORA_KEY_PERIOD)) {
		ry -= 1.f;
	}*/
	
	if(CORE->keyDown(SORA_KEY_Z)) {
		sora::SORA_LOCALIZER->setCurrentLocale("zh-cn");
	}
	
	if(CORE->keyDown(SORA_KEY_ESCAPE))
		CORE->shutDown();
	
	if(CORE->keyDown(SORA_KEY_F))
		sceneManager->switchToScene(sceneend, 5.f);
	if(CORE->keyDown(SORA_KEY_D))
		sceneManager->switchToScene(scenestart, 5.f);
	
	
	par->rotate(sora::DGR_RAD(rr), sora::DGR_RAD(rp), sora::DGR_RAD(ry));
	
	pAnimatedSprite->render(300.f, 300.f);
	
	sceneManager->render();
	sora::GCN_GLOBAL->gcnDraw();
	
	CORE->endScene();
	return false;
}

class boundEffector: public sora::SoraParticleEffector {
public:
	boundEffector(const hgeRect& rect): boundingBox(rect) {}
	bool effect(sora::SoraParticleNode& node, float32 dt) {
		if(!boundingBox.TestPoint(node.position.x, node.position.y))
			return true;
		return false;
	}
		   
private:
	hgeRect boundingBox;	   
};

class rollBackEffector: public sora::SoraParticleEffector {
public:
	rollBackEffector(float32  _dec): fSpeedDec(_dec) {}
	bool effect(sora::SoraParticleNode& node, float32 dt) {
		node.fSpeed -= fSpeedDec * dt;
		return false;
	}
	
private:
	float32 fSpeedDec;
};

#include "guichansetup.h"
#include "gcnImageButton2.h"
#include "menuScene.h"

void testMainWindow::init() {
	CORE = sora::SoraCore::Instance();
	
	font = (sora::SoraFont*)CORE->createFont(L"ARIALN.TTF", 20);
	if(!font) {
		std::cout<<"Error loading Font"<<std::endl;
	} else {
		font->setColor(0xFFFFFFFF);
	}
	
	sora::GCN_GLOBAL->initGUIChan(L"ARIALN.TTF", 20);
	sora::GCN_GLOBAL->createTop();
	sceneManager = new projl::lSceneManager(100.f, 100.f, 400.f, 400.f);

	scenestart = new projl::lGUIScene(0.f, 0.f, 400.f, 400.f);
	sceneManager->addScene(scenestart);
	
	sceneend = new projl::lGUIScene(0.f, 0.f, 500.f, 500.f);
	sceneManager->addScene(sceneend);
	
	lShineMenuScene* menu = new lShineMenuScene(0.f, 0.f, CORE->createFont(L"ARIALN.TTF", 30));
	menu->setPosition(0.f, 0.f);
	menu->addItem("Start");
	menu->addItem("Option");
	menu->addItem("End");
	scenestart->add(menu);
	
	lShineMenuScene* menu2 = new lShineMenuScene(0.f, 0.f, CORE->createFont(L"ARIALN.TTF", 30));
	menu2->addItem("Easy");
	menu2->addItem("Normal");
	menu2->addItem("Hard");
	menu2->addItem("Lunatic");
	sceneend->add(menu2);
	
	sceneManager->setSwitchEffectEnabled(false);
	
	pAnimatedSprite = new ls::lsAnimatedSprite(L"patchouli.lanm");
	//pAnimatedSprite->pSpr->setPosition(100.f, 100.f);
	//scenestart->add(pAnimatedSprite->pSpr);

	if(!pAnimatedSprite->valid())
		printf("error loading animated sprite\n");
	else {
		pAnimatedSprite->play("spellcard1");
		
		//pAnimatedSprite->pSpr->setCenter(64, 64);
		
		
		//sora::SORA_EVENT_MANAGER->registerTimerEvent(pAnimatedSprite, new ls::timeredAnimationEv(1, 0.5, true), 1.f, true);
		
		//pAnimatedSprite->pSpr->addEffect(new sora::IEColorTransitions(0xFF00FFFF, 0xFF0000FF, 1.f, sora::IMAGE_EFFECT_PINGPONG));
		//pAnimatedSprite->pSpr->addEffect(new sora::IERotation(0.f, 0.f, sora::DGR_RAD(360.f), sora::DGR_RAD(360.f), 3.f, sora::IMAGE_EFFECT_REPEAT));
		//pAnimatedSprite->pSpr->addEffect(new sora::SoraImageEffectTransitions(100.f, 100.f, 400.f, 300.f, 5.f, sora::IMAGE_EFFECT_PINGPONG));
	}
	
	
	par = new sora::SoraParticleSystem;
	sora::SoraParticleHeader header;
	
	header.nEmitNum = 300;
	header.fEmitLifetime = 0.f;
	
	header.fMinLifetime = 4.f;
	header.fMaxLifetime = 4.f;
	header.fMinSpeed = 350.f;
	header.fMaxSpeed = 350.f;
	header.fMinGravity = header.fMaxGravity = 0.f;
	header.fMinStartScale = 0.f; header.fMaxStartScale = 1.f;
	header.fMinEndScale = header.fMinStartScale; header.fMaxEndScale = header.fMaxStartScale ;
	header.fMinLinearAcc = header.fMaxLinearAcc = 0.f;
	header.fMinTrigAcc = 0.f;
	header.fMaxTrigAcc = 0.f;
	header.fMinAngle = 0.f;
	header.fMaxAngle = 0.f;
	header.fMinSpin = -1.f;
	header.fMaxSpin = 1.f;
	
	header.fEmitDuration = 4.f;
	
	header.dwMinStartColor = header.dwMaxStartColor = sora::SoraColorRGBA(1.f, 0.3f, 1.0f, 1.f);
	header.dwMinEndColor = header.dwMaxEndColor = sora::SoraColorRGBA(1.f, 1.0f, 1.f, 1.f);
	
	header.emitPos = sora::vector3(getWindowWidth()/2, getWindowHeight()/2, 300);
	header.emitRange = sora::vector3(20, 20, 0);
	header.minDirection = sora::vector3(-1, -1, 0);
	header.maxDirection = sora::vector3(1, 1, 0);
	
	header.texX = 96.f;
	header.texY = 32.f;
	header.texW = header.texH = 32.f;
	
	header.blendMode = BLEND_ZWRITE | BLEND_ALPHABLEND | BLEND_COLORADD;
	
	par->emit(header, CORE->createSprite(L"particles.png"));
	//boundEffector* eff = new boundEffector(hgeRect(100, 100, 600, 600));
	//par->addEffector(eff);
	par->addEffector(new rollBackEffector(175.f));
	
	rr = ry = rp = 0.f;
}

