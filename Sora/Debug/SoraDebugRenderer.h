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
		
		void addLine(const hgeVector& point, float32 width, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=1.f);
		void addAABB(const hgeVector& point, const hgeVector& wh, bool bFill=false, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=1.f);
		void addCricle(const hgeVector& point, float32 radius, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=1.f);
		void addString(const hgeVector& point, const SoraWString& str, const SoraColorRGBA& color=COLOR_BLACK, float32 depth=1.f);
		
		void render() {}
		
		void clear();
		
	private:
		struct debugRenderItem {
			hgeVector pos;
			SoraColorRGBA color;
			
			hgeVector extra;
			float32 depth;
			
			SoraWString str;
			
			uint8 type;
			
			debugRenderItem(uint8 _type): type(_type), color(COLOR_WHITE) {}
		};
		typedef std::list<debugRenderItem> DEBUG_RENDER_ITEMS;
		
		DEBUG_RENDER_ITEMS debugRenderItems;
	};
	
	static SoraDebugRenderer* DEBUG_RENDERER = SoraDebugRenderer::Instance();
} // namespace sora

#endif // DEBUG_RENDERER_H_
