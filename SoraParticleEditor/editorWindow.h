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

#ifdef WIN32
#include "DarkLib/FileDlg.h"
#endif


class editorWindow {
public:
	editorWindow();
	~editorWindow();

	void update();
	void render();

	gcn::Widget* loadXML(const SoraWString& xmlPath);

private:
	sora::SoraFont* pFont;
	
	float32 mBGPosX, mBGPosY;
};

#endif