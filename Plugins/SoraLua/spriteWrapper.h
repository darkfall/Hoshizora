//
//  spriteWrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/25/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef _SPRITE_WRAPPER_H_
#define _SPRITE_WRAPPER_H_

#include "SoraSprite.h"
#include "SoraCore.h"

namespace spritewrapper {
    
    using namespace sora;
    
    HSORASPRITE createSprite(const SoraWString& path);
    HSORASPRITE createSpriteWithTex(HSORATEXTURE tex);
    HSORASPRITE createSpriteWithTexEx(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h);
	HSORASPRITE createSpriteWH(float32 w, float32 h);
	
	HSORATEXTURE createTexture(const SoraWString& path);
	HSORATEXTURE createTextureWH(float32 w, float32 h);
	void releaseTexture(HSORATEXTURE tex);
    
    void setTextureRect	(HSORASPRITE h, float32 x, float32 y, float32 w, float32 fh);
    void setColor		(HSORASPRITE h, float32 r, float32 g, float32 b, float32 a);
	void setVertexColor (HSORASPRITE h, float32 r, float32 g, float32 b, float32 a, int32 i);
	void setAlpha		(HSORASPRITE h, float32 a);
    void setZ			(HSORASPRITE h, float32 z, int32 i=-1);
    ulong32 getColor	(HSORASPRITE h, int32 i=-1);
    float32 getZ		(HSORASPRITE h, int32 i=-1);
    void setCenter		(HSORASPRITE h, float32 x, float32 y);
    float32 getCenterX	(HSORASPRITE h);
    float32 getCenterY	(HSORASPRITE h);
    void setFlip		(HSORASPRITE h, bool hf, bool vf);
    bool getVFlip		(HSORASPRITE h);
    bool getHFlip		(HSORASPRITE h);
    
    int32 getTextureWidth		(HSORASPRITE h);
    int32 getTextureHeight		(HSORASPRITE h);
    int32 getSpriteWidth		(HSORASPRITE h);
    int32 getSpriteHeight		(HSORASPRITE h);
    void setScale				(HSORASPRITE h, float32 vs, float32 hs);
    float32 getVScale			(HSORASPRITE h);
    float32 getHScale			(HSORASPRITE h);
    void setRotation			(HSORASPRITE h, float32 r);
    float32 getRotation			(HSORASPRITE h);
    
    void addEffect  (HSORASPRITE h, ulong32 eff);
    void stopEffect (HSORASPRITE h, ulong32 eff);
    void clearEffects (HSORASPRITE h);
    bool hasEffect  (HSORASPRITE h);
    
    void update(HSORASPRITE h);
    void render(HSORASPRITE h);
    void renderWithPos(HSORASPRITE h, float32 x, float32 y);
    
    int32 getPosX(HSORASPRITE h);
    int32 getPosY(HSORASPRITE h);
    void setPosition(HSORASPRITE h, float32 x, float32 y);
    
    void setBlendMode		(HSORASPRITE h, int32 mode);
    int32 getBlendMode		(HSORASPRITE h);
    
    void release (HSORASPRITE h);
} // namespace spritewrapper


namespace imageeffectwrapper {
    
    using namespace sora;

    ulong32 makeEffect_fade(float32 src, float32 dst, float32 time, int32 mode);
    ulong32 makeEffect_transitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 time, int32 mode);
    ulong32 makeEffect_scale_tensile(float32 sv, float32 dv, float32 sh, float32 dh, float32 time, int32 mode);
    ulong32 makeEffect_scale(float32 s, float32 d, float32 time, int32 mode);
    ulong32 makeEffect_colorTransitions(ulong32 start, ulong32 end, float32 time, int32 mode);
    ulong32 makeEffect_rotation(float32 start, float32 end, float32 time, int32 mode);
	ulong32 makeEffect_rotationZ(float32 start, float32 zstart, float32 end, float32 zend, float32 time, int32 mode);
    
} // namespace iamgeeffectwrapper

#endif