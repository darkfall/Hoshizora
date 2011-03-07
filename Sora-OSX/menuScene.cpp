/*
 *  menuScene.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "menuScene.h"

lShineMenuScene::lShineMenuScene(float32 x, float32 y, sora::SoraFont* font) {
	setPosition(x, y);
	currItem = 0;
	pfont = font;
	//assert(font != 0);
	
	kerningHeight = 0.f;
	kerningWidth = 0.f;
	
	sora = sora::SoraCore::Instance();
	
	responseTime = DEFAULT_MENU_RESPONSE_TIME;
	tev.stop();
	
	bActive = false;
	alpha = 1.f;
	
	colorTransitionTime = DEFAULT_MENU_COLOR_TRANSITION_TIME;
	colStart = sora::SoraColorRGBA(1.f, 1.f, 1.f, 1.f);
	colEnd = sora::SoraColorRGBA(1.f, 1.f, 1.f, 1.f);
	colStartHL = sora::SoraColorRGBA(1.f, 0.7f, 0.7f, 1.f);
	colEndHL = sora::SoraColorRGBA(1.f, 0.5f, 0.5f, 1.f);
	colorHighLight = new sora::IEColorTransitions(colStartHL, colEndHL, colorTransitionTime, sora::IMAGE_EFFECT_PINGPONG, &trigTransformer);
	colorNormal = new sora::IEColorTransitions(colStart, colEnd, colorTransitionTime, sora::IMAGE_EFFECT_PINGPONG, &trigTransformer);

	registerEventFunc(this, &lShineMenuScene::onSceneTimerEvent);
}

lShineMenuScene::~lShineMenuScene() {
	delete colorHighLight;
	delete colorNormal;
	for(size_t i=0; i<items.size(); ++i) {
		delete items[i];
		items[i] = 0;
	}
	clearEffects();
}

void lShineMenuScene::setColorTransitionTime(float32 t) {
	colorTransitionTime = t;
}

void lShineMenuScene::setTransitionColor(const sora::SoraColorRGBA& start,
										 const sora::SoraColorRGBA& end,
										 const sora::SoraColorRGBA& starthl,
										 const sora::SoraColorRGBA& endhl) {
	colStart = start;
	colEnd = end;
	colStartHL = starthl;
	colEndHL = endhl;
	
	delete colorHighLight;
	delete colorNormal;
	colorHighLight = new sora::IEColorTransitions(colStartHL, colEndHL, colorTransitionTime, sora::IMAGE_EFFECT_PINGPONG, &trigTransformer);
	colorNormal = new sora::IEColorTransitions(colStart, colEnd, colorTransitionTime, sora::IMAGE_EFFECT_PINGPONG, &trigTransformer);
}

void lShineMenuScene::onSceneTimerEvent(const projl::lSceneTimerEvent* tev) {
}

void lShineMenuScene::setKerningHeight(float32 h) {
	kerningHeight = h;
}

void lShineMenuScene::setKerningWidth(float32 w) {
	kerningWidth = w;
}

void lShineMenuScene::addItem(const SoraString& item) {
	if(!pfont) return;
	
	projl::lBasicRenderItem* britem = new projl::lBasicRenderItem;
	britem->initWithString(sora::SoraLocalizedString(item), pfont);
	items.push_back(britem);

	currItem = 0;
}

void lShineMenuScene::addItemSpr(const SoraWString& spr, float32 x, float32 y, float32 w, float32 h) {
	projl::lBasicRenderItem* britem = new projl::lBasicRenderItem;
	britem->initWithSprite(spr, x, y, w, h);
	items.push_back(britem);
	
	currItem = 0;
}

uint32 lShineMenuScene::getCurrItem() {
	return currItem;
}

const wchar_t* lShineMenuScene::getCurrItemName() const {
	return items[currItem]->getString().c_str();
}

void lShineMenuScene::setCurrItem(uint32 id) {
	if(id >= 0 && id < items.size())
		currItem = id;
}

projl::lBasicRenderItem* lShineMenuScene::at(uint32 id) {
	if(id >= 0 && id < items.size())
		return items[id];
	return 0;
}

uint32 lShineMenuScene::size() {
	return items.size();
}

uint32 lShineMenuScene::update(float32 dt) {
	if(items.empty()) return -1;
	
	for(size_t i=0; i<items.size(); ++i)
		items[i]->update(dt);
	
	colorHighLight->update(dt);
	colorNormal->update(dt);
	
	updateEffects(dt);
	
	if(tev.isStarted()) {
		tev.update(dt);
		if(tev.getTime() < DEFAULT_MENU_RESPONSE_TIME)
			return -1;
		else
			tev.stop();
	}
	
	if(sora->keyDown(SORA_KEY_DOWN)) {
		++currItem;
		if(currItem == items.size())
			currItem = 0;
		tev.start();
	} else if(sora->keyDown(SORA_KEY_UP)) {
		--currItem;
		if(currItem == -1)
			currItem = items.size()-1;
		tev.start();
	}
	
	return 0;
}

void lShineMenuScene::updateEffects(float32 dt) {
	if(!vEffects.empty()) {
		IMAGE_EFFECT_LIST::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
			uint32 result = (*eff)->update(dt);
			switch((*eff)->getType()) {
				case sora::IMAGE_EFFECT_FADEIN:
				case sora::IMAGE_EFFECT_FADEOUT:
					alpha = (*eff)->get1st();
					break;
					
				case sora::IMAGE_EFFECT_TRANSITIONS:
					setPosition((*eff)->get1st(), (*eff)->get2nd());
					break;
					
				case sora::IMAGE_EFFECT_TENSILE:
				case sora::IMAGE_EFFECT_SCALEIN:
				case sora::IMAGE_EFFECT_SCALEOUT:
					setScale((*eff)->get1st(), (*eff)->get2nd());
					break;
					
				case sora::IMAGE_EFFECT_ROTATE:
					setRotation((*eff)->get1st());
					break;
			}
			
			if(result == sora::IMAGE_EFFECT_END) {
				delete (*eff);
				(*eff) = 0;
				eff = vEffects.erase(eff);
			} else
				++eff;
		}
	}
}

void lShineMenuScene::render() {	
	float32 cx = getPositionX(),
			cy = getPositionY();
	for(size_t i=0; i<items.size(); ++i) {
		if(i==currItem)
			items[i]->setColor(sora::SoraColorRGBA::GetHWColor(colorHighLight->get1st(),
															  colorHighLight->get2nd(),
															  colorHighLight->get3rd(),
															  alpha));
		else
			items[i]->setColor(sora::SoraColorRGBA::GetHWColor(colorNormal->get1st(),
															  colorNormal->get2nd(),
															  colorNormal->get3rd(), 
															  alpha));
		items[i]->render(cx, cy);
		cy += kerningHeight;
		cy += items[i]->getHeight();
		cx += kerningWidth;
	}
}


void lShineMenuScene::addEffect(sora::SoraImageEffect* effect) {
	vEffects.push_back(effect);
}

void lShineMenuScene::clearEffects() {
	IMAGE_EFFECT_LIST::iterator eff = vEffects.begin();
	while(eff != vEffects.end()) {
		delete (*eff);
		(*eff) = 0;
		++eff;
	}
	vEffects.clear();
}

void lShineMenuScene::setScale(float32 sx, float32 sy) {
	for(size_t i=0; i<items.size(); ++i) {
		items[i]->setScale(sx, sy);
	}
}

void lShineMenuScene::setRotation(float32 rot) {
	for(size_t i=0; i<items.size(); ++i) {
		items[i]->setRotation(rot);
	}
}
