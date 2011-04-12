/*
 *  SoraCanvas.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CANVAS_H_
#define SORA_CANVAS_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraCore.h"
#include "SoraFont.h"

namespace sora {
	
	/*
		Base class for canvas which render things onto a texture, using RenderTarget
	 */
	class SoraBaseCanvas: public SoraObject {
	public:
        SoraBaseCanvas(int32 width, int32 height, bool bDepthBuffer=true);
        virtual ~SoraBaseCanvas();
        
        void render();
        uint32 update(float32 dt);
        
        void beginRender();
        void finishRender();
		
        void setFont(SoraFont* font);
        void renderString(const SoraString& str, float32 x, float32 y);
        void renderString(const SoraWString& str, float32 x, float32 y);
        void renderSprite(SoraSprite* spr, float32 x=0.f, float32 y=0.f);
        
        SoraSprite* getCanvasSprite() const { return pCanvasSprite; }
        
	private:
        inline void _attachTarget();
        inline void _detachTarget();
        
		ulong32 canvasTarget;
		
        SoraFont* pFont;
        SoraSprite* pCanvasSprite;
        
        bool bSceneBegan;
	};
	
} // namespace sora

#endif