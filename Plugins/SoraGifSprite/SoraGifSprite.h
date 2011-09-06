/*
 *  SoraGifSprite.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GIF_SPRITE_H_
#define SORA_GIF_SPRITE_H_

#include "SoraSprite.h"

#include "SoraPreDeclare.h"
#include "factory/SoraSpriteFactory.h"

namespace sora {
	
	class SoraGifSprite: public SoraSprite {
	public:
		SoraGifSprite();
		SoraGifSprite(const SoraWString& gifPath);
        virtual ~SoraGifSprite();
		
		bool load(const SoraWString& gifPath);
        
        void setFrameRate(int32 mFrameRate);
        int32 getFrameRate() const;
        
        void toNextFrame();
        void toPrevFrame();
        void toFirstFrame();
        void toLastFrame();
        void setFrame(int32 frameIndex);
        
        int32 getFrameSize() const;
        int32 getCurrFrame() const;
		
		virtual uint32 update(float dt);
        
    private:
        void init();
        void clear();
        
        struct GifFrameNode {
            SoraTextureHandle mFrameTex;
            uint32 mFrameWidth, mFrameHeight;
        };
        
        typedef std::vector<GifFrameNode> GIF_FRAME_CONT;
        GIF_FRAME_CONT mFrames;
        
        int32 mCurrFrame;
        int32 mFrameRate;
        float mFrameInterval;
        float mFrameCount;
	};
    	
} // namespace sora

#endif // SORA_GIF_SPRITE_H_