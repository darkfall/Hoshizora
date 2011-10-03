/*
 *  DebugRenderer.h
 *  Sora
 *
 *  Created by GriffinBu on 1/25/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_DEBUG_RENDERER_H_
#define SORA_DEBUG_RENDERER_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include "SoraCore.h"
#include "SoraFont.h"

#include "SoraSprite.h"

namespace sora {
	
	class SORA_API SoraDebugRenderer: public SoraSingleton<SoraDebugRenderer> {
		friend class SoraSingleton<SoraDebugRenderer>;
		
	public:
		SoraDebugRenderer(): pRenderSprite(0), renderTarget(0) {}
		~SoraDebugRenderer() {
            if(pRenderSprite)
                delete pRenderSprite;
            if(renderTarget)
                SORA->freeTarget(renderTarget);
        }
        
        void createTarget() {
            renderTarget = SORA->createTarget(SORA->getScreenWidth(), SORA->getScreenHeight(), false);

        }
        
        void setFont(SoraFont* font) {
            pFont = font;
        }
		
		void addLine(const SoraVector& point1, const SoraVector& point2, float width, const SoraColorRGBA& color=COLOR_BLACK, float depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point1;
            item.color = color;
            item.depth = depth;
            item.extra = point2;
            item.width = width;
            debugRenderItems.push_back(item);
        }
        
		void addAABB(const SoraVector& point, const SoraVector& wh, bool bFill=false, const SoraColorRGBA& color=COLOR_BLACK, float depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.extra = wh;
            item.bFill = bFill;
            debugRenderItems.push_back(item);
        }
        
		void addCricle(const SoraVector& point, float radius, const SoraColorRGBA& color=COLOR_BLACK, float depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.extra.x = radius;
            debugRenderItems.push_back(item);
        }
        
		void addString(const SoraVector& point, const SoraWString& str, const SoraColorRGBA& color=COLOR_BLACK, float depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.str = str;
            item.pFont = pFont;
            debugRenderItems.push_back(item);
        }
		
		void render() {
        //    SORA->beginScene(0x00000000, renderTarget);
            DEBUG_RENDER_ITEMS::iterator itItem = debugRenderItems.begin();
            while(itItem != debugRenderItems.end()) {
                itItem->render();
                ++itItem;
            }
         //   SORA->endScene();
        }
		
        void renderToScreen() {
            if(!pRenderSprite)
                pRenderSprite = new SoraSprite(SORA->getTargetTexture(renderTarget));
            else
                pRenderSprite->setTexture(SORA->getTargetTexture(renderTarget));
            pRenderSprite->render(0.f, 0.f);
        }
        
		void clear() {
            debugRenderItems.clear();
        }
		
	private:
        enum debugRenderItemType {
            ITEM_NULL,
         
            ITEM_LINE,
            ITEM_AABB,
            ITEM_CIRCLE,
            ITEM_STRING,
        };
        
		struct debugRenderItem {
			SoraVector pos;
			SoraColorRGBA color;
			
			SoraVector extra;
			float depth;
			
			SoraWString str;
			
			uint32 type;
            float width;
            bool bFill;
            
            SoraFont* pFont;
			
			debugRenderItem(uint8 _type): type(_type), color(COLOR_WHITE), bFill(false), pFont(NULL) {}
            
            void render() {
                switch(type) {
                    case ITEM_LINE: SORA->renderLine(pos.x, pos.y, extra.x, extra.y, color.GetHWColor(), depth); break;
                    case ITEM_AABB: 
                        if(!bFill)
                            SORA->renderBox(pos.x, pos.y, pos.x+extra.x, pos.y+extra.y, color.GetHWColor(), depth); 
                        else 
                            SORA->fillBox(pos.x, pos.y, pos.x+extra.x, pos.y+extra.y, color.GetHWColor(), depth); 
                        break;
                    case ITEM_CIRCLE:
                        break;
                    case ITEM_STRING:
                        if(pFont) 
                            pFont->render(pos.x, pos.y, str.c_str());
                        break;
                }
            }
		};
		typedef std::list<debugRenderItem> DEBUG_RENDER_ITEMS;
		
		DEBUG_RENDER_ITEMS debugRenderItems;
        
        SoraTargetHandle renderTarget;
        SoraSprite* pRenderSprite;
        SoraFont* pFont;
	};
	
#define DEBUG_RENDERER SoraDebugRenderer::Instance()
} // namespace sora

#endif // DEBUG_RENDERER_H_
