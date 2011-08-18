/*
 *  ANIMATION_.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/17/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "editorWindow.h"
#include "SoraFileUtility.h"
#include "SoraGUIChan/guichansetup.h"

using namespace sora;

animationEditorWindow::animationEditorWindow() {
	SoraGUIResponserMap::Instance()->registerResponser("AnimationPanel", this);

	pAnimatedSprite = 0;
	pAvailableAnimations = 0;
}

animationEditorWindow::~animationEditorWindow() {
}

gcn::Widget* animationEditorWindow::loadGUIConfig(const SoraWString& confPath) {
	sora::JsonGui* pGuiParser = new sora::JsonGui;
		
	if(!pGuiParser->parse(confPath)) {
		SORA->messageBox("Cannot load window configuration file", "Fatal error", MB_OK | MB_ICONERROR);
		SORA->shutDown();
	}
	
	pAvailableAnimations = (gcn::ListBox*)(pGuiParser->getWidget("AvailableAnimations"));
	if(!pAvailableAnimations) {
		SORA->messageBox("Cannot find animation listbnox GUI widget, make sure your gui config file is complelte", "Fatal Error", MB_OK | MB_ICONERROR);
	}
	return pGuiParser->getWidget("AnimationPanel");

	return NULL;
}

void animationEditorWindow::action() {
	if(getID().compare("Open") == 0) {
		SoraWString path;
		path = SoraCore::Instance()->fileOpenDialog("AnimationConfig(*.txt)\0*.txt\0\0", sora::ws2s(SoraFileUtility::getApplicationPath().c_str()).c_str());
		loadAnimation(sora::ws2s(path));
	} else if(getID().compare("Refresh") == 0) {
		loadAnimation(currentAnm);
	} else if(getID().compare("PlayFrameRate") == 0) {
		gcn::Slider* slider = dynamic_cast<gcn::Slider*> (getSource());
		if(slider) {
			gcn::Label* label = dynamic_cast<gcn::Label*> (GCN_GLOBAL->findWidget("PlayFrameRateLabel"));
			if(label) {
				label->setCaption("PlayerFPS: "+int_to_str((int)slider->getValue()));
			}
			SORA->setFPS(slider->getValue());
		}
	}
}

void animationEditorWindow::loadAnimation(const SoraString& path) {
	std::string anmDest = path+".anm";
	if(!PACK_ANIMATION(path.c_str(), anmDest.c_str())) 
		SORA->messageBox("Error packing animation, please check your format", "Fatal Erro", MB_OK | MB_ICONERROR);
	else {
		if(pAnimatedSprite) {
			delete pAnimatedSprite;
			pAnimatedSprite = 0;
		}
		
		pAnimatedSprite = UNPACK_ANIMATION(anmDest);
		if(!pAnimatedSprite) {
			SORA->messageBox("Error loading animation file, please check your pack", "Error", MB_OK | MB_ICONERROR);
		} else {
			pAnimatedSprite->setPosition(SORA->getScreenWidth()/2.f, SORA->getScreenHeight()/2.f);
			pAnimatedSprite->setAnchor(ANCHOR_MIDDLE);
			pAnimatedSprite->play();
			
			if(pAvailableAnimations) {
				JsonListModel* jlm = (JsonListModel*)pAvailableAnimations->getListModel();
				if(!jlm)
					jlm = new JsonListModel;
				else
					jlm->items.clear();
				
				for(int i=0; i<pAnimatedSprite->getAnimationSize(); ++i) {
					jlm->pushElement(pAnimatedSprite->getAnimationName(i));
				}
				pAvailableAnimations->setListModel(jlm);
				pAvailableAnimations->adjustSize();
				
				currentAnm = path;
				std::string canm = pAnimatedSprite->getCurrentAnimationName();
				for(int i=0; i<jlm->items.size(); ++i) {
					if(jlm->items[i].compare(canm) == 0) {
						pAvailableAnimations->setSelected(i);
						break;
					}
				}
			}
			
			gcn::Label* currentViewing = dynamic_cast<gcn::Label*> (GCN_GLOBAL->findWidget("CurrentAnimationFile"));
			if(currentViewing) {
				size_t dpos = path.rfind("/");
				if(dpos == std::string::npos)
					dpos = path.rfind("\\");
				if(dpos != std::string::npos)
					currentViewing->setCaption("Viewing: "+path.substr(dpos+1, path.size()));
				else {
					currentViewing->setCaption("Viewing: "+path);
				}

			}
		}
	}
}

void animationEditorWindow::valueChanged() {
	if(getID().compare("AvailableAnimations") == 0) {
		pAvailableAnimations = dynamic_cast<gcn::ListBox*>(getSource());
		if(pAvailableAnimations) {
			std::string anmId = pAvailableAnimations->getListModel()->getElementAt(pAvailableAnimations->getSelected());
			if(pAnimatedSprite) {
				pAnimatedSprite->playEx(anmId.c_str(), true, false);
			}
		}
	}
}

void animationEditorWindow::render() {
	if(pAnimatedSprite)
		pAnimatedSprite->render();
}

void animationEditorWindow::update() {
	if(pAnimatedSprite)
		pAnimatedSprite->update(SORA->getDelta());
}