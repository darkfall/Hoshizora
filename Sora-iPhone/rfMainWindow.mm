/*
 *  rfMainWindow.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#include "rfMainWindow.h"

#include "SoraFont.h"

//#include "SoraGUIChan/guichan.hpp"
#include "llexer.h"
#include "SoraiOSDeviceHelper.h"
#include "SoraTimestamp.h"

class buttonListener: public gcn::ActionListener {
public:
	void action(const gcn::ActionEvent& actionEvent) {
		std::string id = actionEvent.getId();
		rfMainWindow* mainWindow = (rfMainWindow*)sora::SoraCore::Instance()->getMainWindow();
		if(id.compare("exit") == 0) {
			sora::SoraCore::Instance()->shutDown();
		} else if(id.compare("retry") == 0) {
			mainWindow->retry();
		} else if(id.compare("continue") == 0) {
			mainWindow->tonext();
		}
	}
};

rfMainWindow::~rfMainWindow() {
//	if(pext) delete pext;
//	if(pcontinue) delete pcontinue;
//	if(pretry) delete pretry;
	if(pBG) delete pBG;
	if(prfMapRenderer) delete prfMapRenderer;
//	if(pFont) delete pFont;
	if(pState) delete pState;
}

sora::SoraSprite* spr;


sora::SoraMusicFile* bgm;
sora::SoraSoundEffectFile* se;
bool rfMainWindow::renderFunc() {		
	//CORE->setTransform(0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f)
    
	CORE->beginScene(0);
	if(rfState != STATE_LOGO) {
		pBG->render(0.f, 0.f);
		if(prfMapRenderer) {
            
			prfMapRenderer->render();
            
			renderState();
		}
		if(stateCount != 0) {
			if(pBlackBG) pBlackBG->render(0.f, 0.f);
			if(pState)  {
				pState->render();
			}
		}
        
		sora::GCN_GLOBAL->gcnLogic();
		sora::GCN_GLOBAL->gcnDraw();
	} else {
		if(pFont) {
			pFont->setColor(CARGB(255, 0, 153, 253), 0);
			pFont->setColor(CARGB(255, 0, 153, 155), 1);
			pFont->setColor(CARGB(255, 155, 153, 0), 2);
			pFont->setColor(CARGB(255, 255, 153, 0), 3);
			pFont->render((float32)getWindowWidth()/2-pFont->getStringWidth(L"Presented By Darkfall")/2, 
						  (float32)getWindowHeight()/2-96, sora::FONT_ALIGNMENT_LEFT, L"Presented By Darkfall");
		}
        //		pLogoAnim->render((float32)getWindowWidth()/2-64, (float32)getWindowHeight()/2-64);
	}
		CORE->endScene();

   /* CORE->beginScene();
    spr->setTexture(sora::SORA->getTargetTexture(fbo));
 //   spr->setScale(0.5f, 0.5f);
    spr->render();

    

    CORE->endScene();*/
	return false;
}

void rfMainWindow::renderState() {
	if(pFont) {
		pFont->setColor(0xFFFFFFFF);
		pFont->print(10.f, 0.f, sora::FONT_ALIGNMENT_LEFT, L"Mirror: ");
		if(prfMapRenderer->getMaxMirrorNum() != 0) {
			if(prfMapRenderer->getMaxMirrorNum() == prfMapRenderer->getUsedMirrorNum())
				pFont->setColor(CARGB(255, 253, 89, 0));
			pFont->print(10.f+pFont->getStringWidth(L"Mirror: "), 0.f, sora::FONT_ALIGNMENT_LEFT, L"%d/%d", prfMapRenderer->getUsedMirrorNum(), prfMapRenderer->getMaxMirrorNum());
		}
		else
			pFont->print(10.f+pFont->getStringWidth(L"Mirror: "), 0.f, sora::FONT_ALIGNMENT_LEFT, L"Infinite");
		pFont->setColor(0xFFFFFFFF);
		if(prfMapRenderer->getMaxTime() != 0) {
			pFont->print(300.f, 0.f, sora::FONT_ALIGNMENT_LEFT, L"Time: ");
			uint16 timeleft = prfMapRenderer->getMaxTime()-prfMapRenderer->getUsedTime();
			if(timeleft <= 10)
				pFont->setColor(CSETG(CSETB(0xFFFFFFFF, (float)timeleft/10*255), (float)timeleft/10*255));
			pFont->print(300.f+pFont->getStringWidth(L"Time "), 0.f, sora::FONT_ALIGNMENT_LEFT, L"%d", timeleft);
		}
		else
			pFont->print(300.f, 0.f, sora::FONT_ALIGNMENT_LEFT, L"Time: Infinite");
		
		pFont->setColor(0xFFFFFFFF);
		pFont->print(getWindowWidth()-pFont->getStringWidth(L"Lighted Box:   /    "), getWindowHeight()-80.f, sora::FONT_ALIGNMENT_LEFT, L"Lighted Box: %d/%d", prfMapRenderer->getBoxGot(), prfMapRenderer->getBoxAim());
        pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %.2f", sora::SORA->getFPS());
	}
}

void rfMainWindow::success() {
	pState = new sora::SoraSprite(CORE->createTexture(L"youwin.png"));
	pState->setCenter(pState->getTextureWidth()/2, pState->getTextureHeight()/2);
	pState->setPosition(getWindowWidth()/2, getWindowHeight()/2);
	
	pState->addEffect(new sora::SoraImageEffectScale(3.f, 1.f, 0.5f, sora::IMAGE_EFFECT_ONCE));
	pState->addEffect(new sora::SoraImageEffectFade(0.f, 1.f, 1.f));
	
	stateCount = 0.01f;
	rfState = STATE_SUCCEED;
}

void rfMainWindow::fail() {
	pState = new sora::SoraSprite(CORE->createTexture(L"youlose.png"));
	pState->setCenter(pState->getTextureWidth()/2, pState->getTextureHeight()/2);
	pState->setPosition(getWindowWidth()/2, getWindowHeight()/2);
	
	pState->addEffect(new sora::SoraImageEffectScale(3.f, 1.f, 0.5f, sora::IMAGE_EFFECT_ONCE));
	pState->addEffect(new sora::SoraImageEffectFade(0.f, 1.f, 1.f));
	
	stateCount = 0.01f;
	rfState = STATE_FAILED;
}

bool rfMainWindow::updateFunc() {
	if(rfState == STATE_LOGO) {
//		pLogoAnim->update(CORE->getDelta());
		
		logoCount += CORE->getDelta();
		if(logoCount >= 5.f)
			rfState = STATE_GAMING;
		return false;
	}
	
	if(prfMapRenderer) {
		if(rfState == STATE_GAMING) prfMapRenderer->update(CORE->getDelta());
		if(prfMapRenderer->isWin() && stateCount == 0) {
			success();
		} else if(prfMapRenderer->isLose() && stateCount == 0) {
			fail();
		}
	}
	if(stateCount != 0) {
		if(pState) 
			pState->update(CORE->getDelta());
		stateCount += CORE->getDelta();
		if(stateCount > 0.8f) {
			if(rfState != STATE_PAUSE_MENU) {
				if(rfState == STATE_FAILED || rfState == STATE_SUCCEED) {
					pext->setVisible(true);
					pext->setEnabled(true);
					switch (rfState) {
						case STATE_FAILED:
							pretry->setVisible(true);
							pretry->setEnabled(true);
							break;
					
						case STATE_SUCCEED:
							pcontinue->setVisible(true);
							pcontinue->setEnabled(true);
							break;
					}
					
					rfState = STATE_PAUSE_MENU;
				}
			} 
			if(rfState == STATE_RV_RETRY || rfState == STATE_RV_CONT) {
				delete pState;
				pState = 0;
								
				if(rfState == STATE_RV_RETRY) {
					--currMapIndex;
					if(!toNextMap()) {
						CORE->messageBox("Error loading RFMap", "Fatal Error", MB_OK | MB_ICONERROR);
						CORE->shutDown();
					}
				} else if(rfState == STATE_RV_CONT) {
					if(!toNextMap()) {
						CORE->messageBox("Error loading RFMap", "Fatal Error", MB_OK | MB_ICONERROR);
						CORE->shutDown();
					}
				}
				
				rfState = STATE_GAMING;
				stateCount = 0.f;
			}
		}
	}
	return false;
}

void rfMainWindow::retry() {
	if(rfState == STATE_PAUSE_MENU) {
		if(pState) {
			pState->addEffect(new sora::SoraImageEffectScale(1.f, 3.f, 0.5f, sora::IMAGE_EFFECT_ONCE));
			pState->addEffect(new sora::SoraImageEffectFade(1.f, 0.f, 1.f));
			
			stateCount = 0.01f;
			rfState = STATE_RV_RETRY;
			
			pext->setVisible(false);
			pext->setEnabled(false);
			
			pretry->setVisible(false);
			pretry->setEnabled(false);
		}
	}
}

void rfMainWindow::tonext() {
	if(rfState == STATE_PAUSE_MENU) {
		if(pState) {
			pState->addEffect(new sora::SoraImageEffectScale(1.f, 3.f, 0.5f, sora::IMAGE_EFFECT_ONCE));
			pState->addEffect(new sora::SoraImageEffectFade(1.f, 0.f, 1.f));
			
			stateCount = 0.01f;
			rfState = STATE_RV_CONT;
			
			pext->setVisible(false);
			pext->setEnabled(false);
			
			pcontinue->setVisible(false);
			pcontinue->setEnabled(false);
		}
	}
}

int32 rfMainWindow::loadMap(const wchar_t* map, uint16 maxM, uint16 maxT) {
	if(!prfMapRenderer->loadMap(map)) {
		CORE->messageBox("Error loading Map "+sora::ws2s(map), "Fatal Error", MB_OK | MB_ICONERROR);
		delete prfMapRenderer;
		prfMapRenderer = 0;
		
		return 0;
	} else {
		prfMapRenderer->setPosition(0, 0);
		prfMapRenderer->setMaxMirrorNum(maxM);
		prfMapRenderer->setMaxTime(maxT);
	}
	return 1;
}

int32 rfMainWindow::toNextMap() {
	if(availMaps.size() == 0)
		return 0;
	
	++currMapIndex;
	if(currMapIndex >= availMaps.size()) 
		currMapIndex = availMaps.size()-1;
	
	return loadMap(availMaps[currMapIndex].mapPath.c_str(), availMaps[currMapIndex].maxMirror, availMaps[currMapIndex].maxTime);
}

int32 rfMainWindow::loadMapList(const wchar_t* pstrMapList) {
	ulong32 size;
	void* data = CORE->getResourceFile(pstrMapList, size);
	if(!data)
		return 0;
	
	llexer* lexer = new llexer;
	if(lexer->loadFileMem(data, size) != LEX_NO_ERROR)
		return 0;
	
	Token t = lexer->getNextToken();
	while(t != TOKEN_TYPE_END_OF_STREAM) {
		RF_MAP_INFO info;
		if(t != TOKEN_TYPE_STRING) {
			free(data);
			delete lexer;
			return 0;
		}
		info.mapPath = sora::s2ws(lexer->getCurrLexeme());
		
		t = lexer->getNextToken();
		if(t != TOKEN_TYPE_INT) {
			free(data);
			delete lexer;
			return 0;
		}
		info.maxMirror = atoi(lexer->getCurrLexeme());
		
		t = lexer->getNextToken();
		if(t != TOKEN_TYPE_INT) {
			free(data);
			delete lexer;
			return 0;
		}
		info.maxTime = atoi(lexer->getCurrLexeme());
		
		t = lexer->getNextToken();
		
		availMaps.push_back(info);
	}
	
	free(data);
	return 1;
}

size_t rfMainWindow::getAvailableMapCount() const {
	return availMaps.size();
}

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFileUtility.h"


void rfMainWindow::init() {
	CORE = sora::SoraCore::Instance();
	
	CORE->registerResourceManager(new sora::SoraZipResourceManager);
	CORE->attachResourcePack(CORE->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"data.rfResource"));
	printf("%s\n", sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
    printf("%d, %d\n", getWindowWidth(), getWindowHeight());
    
    bgm = CORE->createMusicFile(L"02.mp3", true);
    se = CORE->createSoundEffectFile(SoraWString(L"click_08.wav"));
    bgm->play();
    
	pBG = CORE->createSprite(L"magicCircle.png");
	//if(pBG) pBG->setScale(pBG->getTextureWidth()/getWindowWidth(), pBG->getTextureHeight()/getWindowHeight());
	pBlackBG = CORE->createSprite(L"blackbg.png");
	
	stateCount = 0;
	
	prfMapRenderer = new rf::rfMapRenderer;
	prfMapRenderer->setBoxSprite(L"box.png", L"boxhl.png");
	prfMapRenderer->setMirrorSprite(L"mirror2.png");
	prfMapRenderer->setLightSourceSprite(L"lightsource.png", L"light.png");
	
	if(!loadMapList(L"rfMapConf.rfConf")) {
		CORE->messageBox("Error loading RFMap Conf", "Fatal Error", MB_OK | MB_ICONERROR);
		CORE->shutDown();
	}
	currMapIndex = -1;
	if(!toNextMap()) {
		CORE->messageBox("Error loading RFMap", "Fatal Error", MB_OK | MB_ICONERROR);
		CORE->shutDown();
	}
		
	pFont = CORE->createFont(L"STHeitiSC-Medium", 30);
	if(pFont) pFont->setColor(0xFFFFFFFF);
	
	/* gui part */
	sora::GCN_GLOBAL->initGUIChan(L"STHeitiSC-Medium", 30);
	sora::GCN_GLOBAL->createTop();
	gcn::Container* pTop = sora::GCN_GLOBAL->getTop();
	buttonListener* listener = new buttonListener;
	
	pretry = new gcn::ImageButton2("retryButton.png", gcn::Rectangle(0, 0, 256, 128), gcn::Rectangle(0, 128, 256, 128));
	pretry->setPosition(0, 256);
	pretry->setActionEventId("retry");
	pretry->addActionListener(listener);
	pTop->add(pretry);
	
	pext = new gcn::ImageButton2("exitButton.png", gcn::Rectangle(0, 0, 256, 128), gcn::Rectangle(0, 128, 256, 128));
	pext->setPosition(288, 256);
	pext->setActionEventId("exit");
	pext->addActionListener(listener);
	pTop->add(pext);
	
	pcontinue = new gcn::ImageButton2("continueButton.png", gcn::Rectangle(0, 0, 256, 128), gcn::Rectangle(0, 128, 256, 128));
	pcontinue->setPosition(0, 256);
	pcontinue->setActionEventId("continue");
	pcontinue->addActionListener(listener);
	pTop->add(pcontinue);
	
	pretry->setEnabled(false);
	pretry->setVisible(false);
	pext->setEnabled(false);
	pext->setVisible(false);
	pcontinue->setEnabled(false);
	pcontinue->setVisible(false);
	
	rfState = STATE_LOGO;
	logoCount = 0.f;
//	pLogoAnim = new ls::lsAnimatedSprite(L"patchouli.lanm");
//	if(!pLogoAnim)
		rfState = STATE_GAMING;
    spr = new sora::SoraSprite(0);
    fbo = sora::SORA->createTarget(getWindowWidth(), getWindowHeight());
//	else {
//		pLogoAnim->play();
//	}
}