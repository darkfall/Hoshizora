/*
 *  lGUIScene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_GUISCENE_H_
#define L_GUISCENE_H_

#include "lScene.h"
#include "SoraGUIChan/guichan.hpp"
#include "guichansetup.h"

namespace projl {
	
	class lGUIScene: public lScene {
	public:
		lGUIScene(float32 x, float32 y, float32 x1, float32 y1);
		~lGUIScene();
		
		void addWidget(gcn::Widget* widget);
		void delWidget(gcn::Widget* widget);
		
	
		void onSceneTimerEvent(const lSceneTimerEvent* tev);
		
	private:
		gcn::Container* pCont;
	};
	
} // namespace projl

#endif