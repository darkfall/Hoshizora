#ifndef PARTICLE_EDITOR_WINDOW_H
#define PARTICLE_EDITOR_WINDOW_H

#include "Hoshizora.h"
#include "SoraFont.h"
#include "SoraParticleSystem/SoraParticleSystem.h"
#include "SoraGUIChan/guichansora.hpp"
#include "SoraGUIChan/guichan.hpp"
#include "SoraGUIChan/SoraGUIListener.h"
#include "SoraGUIChan/SoraGUIResponser.h"
#include "SoraGUIChan/SoraGUIResponserMap.h"

#include "SoraEvent.h"
#include "SoraKeyInfo.h"

#ifdef WIN32
#include "DarkLib/FileDlg.h"
#endif

void loadBGSprite(const SoraWString& path);
class editorWindow: public sora::SoraEventHandler {
public:
	editorWindow();
	~editorWindow(){}

	void update();
	void render();

	gcn::Widget* loadXML(const SoraWString& xmlPath);
	
	
	void onInputEvent(sora::SoraKeyEvent* kev);

private:
	sora::SoraFont* pFont;
	
	float mBGPosX, mBGPosY;
};

#endif