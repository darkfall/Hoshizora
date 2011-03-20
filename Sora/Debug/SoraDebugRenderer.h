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


namespace sora {
	
	class SoraDebugRenderer: public SoraSingleton<SoraDebugRenderer> {
		friend class SoraSingleton<SoraDebugRenderer>;
		
	public:
		SoraDebugRenderer() {}
		~SoraDebugRenderer() {}
		
		void addLine(const hgeVector& point1, const hgeVector& point2, float32 width, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point1;
            item.color = color;
            item.depth = depth;
            item.extra = point2;
            item.width = width;
            debugRenderItems.push_back(item);
        }
        
		void addAABB(const hgeVector& point, const hgeVector& wh, bool bFill=false, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.extra = wh;
            item.bFill = bFill;
            debugRenderItems.push_back(item);
        }
        
		void addCricle(const hgeVector& point, float32 radius, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.extra.x = radius;
            debugRenderItems.push_back(item);
        }
        
		void addString(const hgeVector& point, const SoraWString& str, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=0.f) {
            debugRenderItem item(ITEM_LINE);
            item.pos = point;
            item.color = color;
            item.depth = depth;
            item.str = str;
            debugRenderItems.push_back(item);
        }
		
		void render() {
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
			hgeVector pos;
			SoraColorRGBA color;
			
			hgeVector extra;
			float32 depth;
			
			SoraWString str;
			
			uint32 type;
            float32 width;
            bool bFill;
			
			debugRenderItem(uint8 _type): type(_type), color(COLOR_WHITE), bFill(false) {}
            
            void render() {
                switch(type) {
                    case ITEM_LINE: SORA->renderRect(pos.x, pos.y, extra.x, extra.y, width, color.GetHWColor(), depth); break;
                    case ITEM_AABB: 
                        if(!bFill)
                            SORA->renderRect(pos.x, pos.y, pos.x+extra.x, pos.y+extra.y, width, color.GetHWColor(), depth); 
                        break;
                    case ITEM_CIRCLE:
                        break;
                    case ITEM_STRING:
                        break;
                }
            }
		};
		typedef std::list<debugRenderItem> DEBUG_RENDER_ITEMS;
		
		DEBUG_RENDER_ITEMS debugRenderItems;
	};
	
	static SoraDebugRenderer* DEBUG_RENDERER = SoraDebugRenderer::Instance();
} // namespace sora

#endif // DEBUG_RENDERER_H_
