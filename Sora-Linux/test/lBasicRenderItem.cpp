/*
 *  lBasicRenderItem.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/30/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "lBasicRenderItem.h"
#include "SoraFont.h"

namespace projl {
	
	lBasicRenderItem::lBasicRenderItem() {
		spr = 0;
		type = RENDERITEM_NULL;
	}
	
	lBasicRenderItem::~lBasicRenderItem() {
		if(spr) {
			delete spr;
			spr = 0;
		}
	}
	
	void lBasicRenderItem::initWithString(const sora::SoraLocalizedString& str, sora::SoraFont* font) {
		this->str = str;
		this->font = font;
		type = RENDERITEM_STR;
	}
	
	void lBasicRenderItem::initWithSprite(const SoraWString& spr, float32 x, float32 y, float32 w, float32 h) {
		this->spr = sora::SoraCore::Instance()->createSprite(spr);
		if(w != 0.f && h != 0.f) {
			this->spr->setTextureRect(x, y, w, h);
		}
		this->spr->setCenter(this->spr->getSpriteWidth()/2, this->spr->getSpriteHeight()/2);
		type = RENDERITEM_SPR;
		str.setString(spr);
	}
	
	void lBasicRenderItem::render(float32 x, float32 y) {
		if(type == RENDERITEM_STR) {
			font->render(x, y, str.getStr().c_str(), true, true);
		} else if(type == RENDERITEM_SPR) 
			if(spr) spr->render(x, y);
	}
	
	uint32 lBasicRenderItem::update(float32 dt) {
		if(type == RENDERITEM_SPR) 
			if(spr) spr->update(dt);
		return 0;
	}
	
	void lBasicRenderItem::setColor(ulong32 col) {
		if(type == RENDERITEM_SPR) {
			spr->setColor(col);
		} else if(type == RENDERITEM_STR) {
			font->setColor(col);
		}
	}
	
	float32 lBasicRenderItem::getHeight() {
		if(type == RENDERITEM_SPR) {
			if(spr) return spr->getSpriteHeight();
		}
		else if(type == RENDERITEM_STR) {
			return font->getHeight();
		}
		return 0.f;
	}
	
	float32 lBasicRenderItem::getWidth() {
		if(type == RENDERITEM_SPR) {
			if(spr) return spr->getSpriteWidth();
		} else if(type == RENDERITEM_STR)
			return font->getStringWidth(str.c_str());
		return 0.f;
	}
	
	ulong32 lBasicRenderItem::getColor() {
		if(type == RENDERITEM_SPR)
			if(spr) return spr->getColor();
		else if(type == RENDERITEM_STR) 
			return font->getColor();
		return 1.f;
	}
	
	void lBasicRenderItem::setScale(float32 sx, float32 sy) {
		if(type == RENDERITEM_SPR) 
			if(spr) spr->setScale(sx, sy);
		else if(type == RENDERITEM_STR)
			font->setScale(sx);
	}
	
	void lBasicRenderItem::setRotation(float32 rot) {
		if(type == RENDERITEM_SPR)
			if(spr) spr->setRotation(rot);
		else if(type == RENDERITEM_STR)
			font->setCharRotation(rot);
	}
} // namespace projl