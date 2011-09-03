//
//  spriteWrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef _SPRITE_WRAPPER_H_
#define _SPRITE_WRAPPER_H_

#include "SoraSprite.h"
#include "SoraCore.h"

namespace spritewrapper {
    
    using namespace sora;
    
    HSORASPRITE createSprite(const SoraWString& path);
    HSORASPRITE createSpriteWithTex(HSORATEXTURE tex);
    HSORASPRITE createSpriteWithTexEx(HSORATEXTURE tex, float x, float y, float w, float h);
	HSORASPRITE createSpriteWH(int32 w, int32 h);
	
	HSORATEXTURE createTexture(const SoraWString& path);
	HSORATEXTURE createTextureWH(int32 w, int32 h);
	void releaseTexture(HSORATEXTURE tex);
    
    void setTextureRect	(HSORASPRITE h, float x, float y, float w, float fh);
    void setColor		(HSORASPRITE h, float r, float g, float b, float a);
	void setVertexColor (HSORASPRITE h, float r, float g, float b, float a, int32 i);
	void setAlpha		(HSORASPRITE h, float a);
    void setZ			(HSORASPRITE h, float z, int32 i=-1);
    ulong32 getColor	(HSORASPRITE h, int32 i=-1);
    float getZ		(HSORASPRITE h, int32 i=-1);
    void setCenter		(HSORASPRITE h, float x, float y);
    float getCenterX	(HSORASPRITE h);
    float getCenterY	(HSORASPRITE h);
    void setFlip		(HSORASPRITE h, bool hf, bool vf);
    bool getVFlip		(HSORASPRITE h);
    bool getHFlip		(HSORASPRITE h);
    
    int32 getTextureWidth		(HSORASPRITE h);
    int32 getTextureHeight		(HSORASPRITE h);
    int32 getSpriteWidth		(HSORASPRITE h);
    int32 getSpriteHeight		(HSORASPRITE h);
    void setScale				(HSORASPRITE h, float vs, float hs);
    float getVScale			(HSORASPRITE h);
    float getHScale			(HSORASPRITE h);
    void setRotation			(HSORASPRITE h, float r);
    float getRotation			(HSORASPRITE h);
	HSORATEXTURE getTexture		(HSORASPRITE h);

    void addEffect  (HSORASPRITE h, ulong32 eff);
    void stopEffect (HSORASPRITE h, ulong32 eff);
    void clearEffects (HSORASPRITE h);
    bool hasEffect  (HSORASPRITE h);
    
    void update(HSORASPRITE h);
    void render(HSORASPRITE h);
    void renderWithPos(HSORASPRITE h, float x, float y);
    
    float getPosX(HSORASPRITE h);
    float getPosY(HSORASPRITE h);
    void setPosition(HSORASPRITE h, float x, float y);
    
    void setBlendMode		(HSORASPRITE h, int32 mode);
    int32 getBlendMode		(HSORASPRITE h);
	
	
	ulong32 attachShader(HSORASPRITE h, const SoraWString& shader, const SoraString& entry, int32 type);
	
	void setShaderParam1f(ulong32 shader, const char* name, float param);
	void setShaderParam2f(ulong32 shader, const char* name, float p1, float p2);
	void setShaderParam3f(ulong32 shader, const char* name, float p1, float p2, float p3);
	void setShaderParam4f(ulong32 shader, const char* name, float p1, float p2, float p3, float p4);
	
	void setShaderParam1i(ulong32 shader, const char* name, int32 param);
	void setShaderParam2i(ulong32 shader, const char* name, int32 p1, int32 p2);
	void setShaderParam3i(ulong32 shader, const char* name, int32 p1, int32 p2, int32 p3);
	void setShaderParam4i(ulong32 shader, const char* name, int32 p1, int32 p2, int32 p3, int32 p4);
	
	void setShaderTextureParam(ulong32 shader, const char* name, ulong32 tex);
    
    void release (HSORASPRITE h);
} // namespace spritewrapper


namespace imageeffectwrapper {
    
    using namespace sora;

    ulong32 makeEffect_fade(float src, float dst, float time, int32 mode);
    ulong32 makeEffect_transitions(float sx, float sy, float dx, float dy, float time, int32 mode);
    ulong32 makeEffect_scale_tensile(float sv, float dv, float sh, float dh, float time, int32 mode);
    ulong32 makeEffect_scale(float s, float d, float time, int32 mode);
    ulong32 makeEffect_colorTransitions(ulong32 start, ulong32 end, float time, int32 mode);
    ulong32 makeEffect_rotation(float start, float end, float time, int32 mode);
	ulong32 makeEffect_rotationZ(float start, float zstart, float end, float zend, float time, int32 mode);
    
} // namespace iamgeeffectwrapper

#endif