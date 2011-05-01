/*
 *  rfEditor.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfEditor.h"
#include "SoraCore.h"
#include "SoraGUIChan/guichansetup.h"
#include "gcnExtend/gcnDraggableImageButtonIncubator.h"

namespace reflection {

	gcn::Widget* myIncubateFunc_hexogon(void* userData) {
		reflection::rfShapeBox* box = new reflection::rfShapeBox;
		
		reflection::rfMap* map = (reflection::rfMap*)(userData);
		if(map) {
			box->setShapeSpriteContainer(map->getShapeSpriteContainer());
		}
		
		box->enableFrameWhenMouseEntered(true);
		
		box->setEdgeNum(6);
		
		return box;
	}
	
	gcn::Widget* myIncubateFunc_square(void* userData) {
		reflection::rfShapeBox* box = new reflection::rfShapeBox;
		
		reflection::rfMap* map = (reflection::rfMap*)(userData);
		if(map) {
			box->setShapeSpriteContainer(map->getShapeSpriteContainer());
		}
		
		box->enableFrameWhenMouseEntered(true);
		
		box->setEdgeNum(4);
		
		return box;
	}
	
	gcn::Widget* myIncubateFunc_trigangle(void* userData) {
		reflection::rfShapeBox* box = new reflection::rfShapeBox;
		
		reflection::rfMap* map = (reflection::rfMap*)(userData);
		if(map) {
			box->setShapeSpriteContainer(map->getShapeSpriteContainer());
		}
		
		box->enableFrameWhenMouseEntered(true);
		
		box->setEdgeNum(3);
		
		return box;
	}
	
	gcn::Widget* myIncubateFunc_oxogen(void* userData) {
		reflection::rfShapeBox* box = new reflection::rfShapeBox;
		
		reflection::rfMap* map = (reflection::rfMap*)(userData);
		if(map) {
			box->setShapeSpriteContainer(map->getShapeSpriteContainer());
		}
		
		box->enableFrameWhenMouseEntered(true);
		
		box->setEdgeNum(8);
		
		return box;
	}
	
	rfEditor::rfEditor(): map(NULL) {
	}
	
	rfEditor::~rfEditor() {
	}
	
	void rfEditor::renderInfo() {
		reflection::rfMap* map = (reflection::rfMap*)sora::GCN_GLOBAL->findWidget("map");
		if(map) {
			pFont->print(map->getWidth()-10.f, 0.f, sora::FONT_ALIGNMENT_RIGHT, L"LightSource: %d", map->getLightSourceNumber());
			pFont->print(map->getWidth()-10.f, 15.f, sora::FONT_ALIGNMENT_RIGHT, L"Mirror: %d", map->getMirrorNumber());
			pFont->print(map->getWidth()-10.f, 30.f, sora::FONT_ALIGNMENT_RIGHT, L"Light: %d", map->getLightNumber());
			pFont->print(map->getWidth()-10.f, 45.f, sora::FONT_ALIGNMENT_RIGHT, L"Shape: %d", map->getShapeBoxNumber());
			pFont->print(map->getWidth()-10.f, 60.f, sora::FONT_ALIGNMENT_RIGHT, L"LightedShape: %d", map->getLightedBoxNumber());
		}
	}
	
	void rfEditor::init() {
		if(map)
			return;
		
		map = new reflection::rfMap;
		map->setId("map");
		map->setDimension(gcn::Rectangle(0, 0, sora::SORA->getScreenWidth(), sora::SORA->getScreenHeight()));
		map->setImage("background1.png");
		map->setState(reflection::rfMap::STATE_EDITING);
		
		map->setLightSourceImage(L"boxhl.png");
		map->setMirrorImage(L"mirror1.png");
		
		map->pushSprite(4, reflection::rfShapeSprite("box.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(0, 0, 64, 64)));
		map->pushSprite(6, reflection::rfShapeSprite("boxbutton.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(64, 0, 64, 64)));
		
		sora::GCN_GLOBAL->addWidget(map, "top");
		
		reflection::rfLightSource* ls = new reflection::rfLightSource;
		ls->setDimension(gcn::Rectangle(100, 100, 10, 10));
		ls->setFacing(sora::DGR_RAD(-90));
		map->add(ls);
		
		gcn::DraggableImageButtonIncubator* di = new gcn::DraggableImageButtonIncubator();
		di->setImage("boxbutton.png", gcn::Rectangle(0, 64, 64, 64), gcn::Rectangle(0, 64, 64, 64)); 
		di->setSelectionColor(gcn::Color(255, 255, 255, 155));
		di->registerIncubatorFunc(myIncubateFunc_hexogon);
		di->setUserData(map);
		di->setAddParent(map);
		sora::GCN_GLOBAL->addWidget(di, "top");
		
		pFont = sora::SORA->createFont(L"ARIALN.ttf", 20);
	}

} // namespace reflection