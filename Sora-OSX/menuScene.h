/*
 *  menuScene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_MENU_SCENE_H_
#define L_MENU_SCENE_H_

#include "lScene.h"
#include "SoraImageEffect.h"
#include "SoraFont.h"
#include "SoraCore.h"

#include "lBasicRenderItem.h"

#include <vector>

const float32 DEFAULT_MENU_RESPONSE_TIME = 0.2f;
const float32 DEFAULT_MENU_COLOR_TRANSITION_TIME = 0.5f;

class menuEvent: public sora::SoraEvent {
public:
	menuEvent(const SoraString& _menu, uint32 _item): menu(_menu), item(_item) {}
	
	SoraString menu;
	uint32 item;
};

class lShineMenuScene: public projl::lScene {
public:
	lShineMenuScene(float32 posx, float32 posy, sora::SoraFont* font);
	virtual ~lShineMenuScene();
	
	void addItem(const SoraString& item);
	void addItemSpr(const SoraWString& spr, float32 x=0.f, float32 y=0.f, float32 w=0.f, float32 h=0.f);
	
	void setKerningHeight(float32 h);
	void setKerningWidth(float32 w);
	
	void setKeyResponseTime(float32 t);
	void setColorTransitionTime(float32 t);
	
	void setTransitionColor(const sora::SoraColorRGBA& start,
							const sora::SoraColorRGBA& end,
							const sora::SoraColorRGBA& starthl,
							const sora::SoraColorRGBA& endhl);
	
	uint32 getCurrItem();
	const wchar_t* getCurrItemName() const;
	
	void render();
	uint32 update(float32 dt);
		
	virtual void onSceneTimerEvent(const projl::lSceneTimerEvent* tev);
	
	void setCurrItem(uint32 id);
	
	projl::lBasicRenderItem* at(uint32 id);
	uint32 size();
	
	void addEffect(sora::SoraImageEffect* eff);
	void clearEffects();
	
	void setScale(float32 sx, float32 sy);
	void setRotation(float32 rot);
	
private:
	inline void updateEffects(float32 dt);
	
	sora::SoraCore* sora;

	typedef std::vector<projl::lBasicRenderItem*> MENU_ITEM_CONT;
	MENU_ITEM_CONT items;
	
	sora::IEColorTransitions* colorHighLight;
	sora::IEColorTransitions* colorNormal;
	
	sora::SoraColorRGBA colStart;
	sora::SoraColorRGBA colEnd;
	sora::SoraColorRGBA colStartHL;
	sora::SoraColorRGBA colEndHL;
	float32 colorTransitionTime;
	
	sora::SoraFont* pfont;
	
	int32 currItem;
	
	float32 kerningHeight;
	float32 kerningWidth;
	
	float32 responseTime;
	sora::timer tev;
	
	sora::CoreTrigTransformer<sora::CoreTransform> trigTransformer;
	
	typedef std::list<sora::SoraImageEffect*> IMAGE_EFFECT_LIST;
	IMAGE_EFFECT_LIST vEffects;
	float32 alpha;
	
	bool bActive;
};

#endif