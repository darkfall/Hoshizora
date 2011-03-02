/*
 *  lGUIScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#include "lGUIScene.h"

namespace projl {
	
	lGUIScene::lGUIScene(float32 x, float32 y, float32 x1, float32 y1) {
		pCont = new gcn::Container;
		pCont->setDimension(gcn::Rectangle(x, y, x1, y1));
		setPosition(x, y);
		pCont->setOpaque(false);
		pCont->setEnabled(false);
		pCont->setVisible(false);
		
		srgc::GCN_GLOBAL->getTop()->add(pCont);
	}
	
	lGUIScene::~lGUIScene() {
		srgc::GCN_GLOBAL->getTop()->remove(pCont);
		delete pCont;
		pCont = 0;
	}
	
	void lGUIScene::addWidget(gcn::Widget* widget) {
		widget->setPosition(widget->getX()+getPositionX(), widget->getY()+getPositionY());			
		pCont->add(widget);
	}
	
	void lGUIScene::delWidget(gcn::Widget* widget) {
		pCont->remove(widget);
	}
	
	void lGUIScene::onSceneTimerEvent(const lSceneTimerEvent* tev) {
		switch (tev->id) {
			case EVENT_SWITCH_TO_START: pCont->setEnabled(false); break;
			case EVENT_SWITCH_TO_HALF: pCont->setVisible(false); break;
			case EVENT_SWITCH_FROM_FINISH: pCont->setEnabled(true); break;
			case EVENT_SWITCH_FROM_HALF: pCont->setVisible(true); break;
		}
		
		pCont->showPart(gcn::Rectangle(0.f, 0.f, getSceneWidth(), getSceneHeight()));

#ifdef _DEBUG
		printf("received scene timer event, id=%d, time=%f\n", tev->id, tev->getTime());
#endif
	}
} // namespace projl