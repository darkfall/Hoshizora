/*
 *  lBasicRenderItem.h
 *  Sora
 *
 *  Created by GriffinBu on 12/30/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_BASIC_RENDER_ITEM_H_
#define L_BASIC_RENDER_ITEM_H_

#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraCore.h"

namespace projl {
	
	enum {
		RENDERITEM_NULL = 0,
		RENDERITEM_STR = 1,
		RENDERITEM_SPR = 2,
	};
	
	class lBasicRenderItem {
	public:
		lBasicRenderItem();
		virtual ~lBasicRenderItem();
		
		void initWithString(const SoraWString& str, sora::SoraFont* font);
		void initWithSprite(const SoraWString& spr, float32 x=0.f, float32 y=0.f, float32 w=0.f, float32 h=0.f);
			
		void render(float32 x, float32 y);
		uint32 update(float32 dt);
		
		sora::SoraSprite* getSprite() { return spr; }
		SoraWString getString() { return str; }
		
		int32 getType() const { return type; }
		
		void setColor(ulong32 col);
		ulong32 getColor();
		
		float32 getHeight();
		float32 getWidth();
		
		void setScale(float32 sx, float32 sy);
		void setRotation(float32 rot);
		
	private:
		sora::SoraSprite* spr;
		
		sora::SoraFont* font;
		SoraWString str;
		
		int32 type;
	};
	
} // namespace projl


#endif