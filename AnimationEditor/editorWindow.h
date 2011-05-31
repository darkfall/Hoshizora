/*
 *  ANIMATION_.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/17/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef ANIMATION_EDITOR_WINDOW_H
#define ANIMATION_EDITOR_WINDOW_H

#include "Hoshizora.h"
#include "SoraFont.h"
#include "SoraGUIChan/guichansora.hpp"
#include "SoraGUIChan/guichan.hpp"
#include "SoraGUIChan/SoraGUIListener.h"
#include "SoraGUIChan/SoraGUIResponser.h"
#include "SoraGUIChan/SoraGUIResponserMap.h"
#include "SoraGUIChan/jsongui.h"

#include "SoraSpriteAnimation/SoraSpriteAnimation.h"

#ifdef WIN32
#include "DarkLib/FileDlg.h"
#else
#include "DarkLib/FileDlgOsx.h"
#endif

class animationEditorWindow: public sora::SoraGUIResponser  {
public:
	animationEditorWindow();
	~animationEditorWindow();
	
	void update();
	void render();
	
	gcn::Widget* loadGUIConfig(const SoraWString& confPath);
	
	void action();
	void valueChanged();
	
	void loadAnimation(const SoraString& path);
	
private:
	sora::SoraFont* pFont;
	
	FileDlg fileopener;
	gcn::ListBox* pAvailableAnimations;
	
	sora::SoraSpriteAnimation* pAnimatedSprite;
	std::string currentAnm;
};

#endif