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
#include "SoraSprite.h"
#include "SoraShader.h"

namespace sora {
	
	/*
		Base class for canvas which render things onto a texture, using RenderTarget
	 */
	class SORA_API SoraBaseCanvas: public SoraObject {
	public:
        SoraBaseCanvas(int32 width, int32 height, bool bDepthBuffer=true);
        virtual ~SoraBaseCanvas();
        
        void render();
        uint32 update(float32 dt);
        
        void beginRender(ulong32 col=0xFF000000);
        void finishRender();

		void setZ(float32 z);
		void setBlendMode(int32 mode);

        void attachShader(SoraShader*);
        void detachShader(SoraShader*);
        SoraShader* attachShader(const wchar_t* shaderPath, const char* entry, SORA_SHADER_TYPE type);
        bool hasShader() const;
        void clearShader();
		
		void addEffect(SoraImageEffect* effect);
		void stopEffect(SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
        
        SoraSprite* getCanvasSprite() const;
        
	private:
        inline void _attachTarget();
        inline void _detachTarget();
        
		ulong32 canvasTarget;
        SoraSprite* pCanvasSprite;
        
        bool bSceneBegan;
	};
	
} // namespace sora

#endif