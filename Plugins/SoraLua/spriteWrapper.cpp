//
//  spriteWrapper.cpp
//  Sora
//
//  Created by Griffin Bu on 3/25/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "spriteWrapper.h"

#include "SoraImageEffect.h"

namespace spritewrapper {
    
    HSORASPRITE createSprite(const SoraWString& path) {
        return (HSORASPRITE)SORA->createSprite(path);
    }
    
    HSORASPRITE createSpriteWithTex(HSORATEXTURE tex) {
        return (HSORASPRITE)(new SoraSprite(tex));
    }
    
    HSORASPRITE createSpriteWithTexEx(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h) {
        return (HSORASPRITE)(new SoraSprite(tex, x, y, w, h));
    }
    
	HSORASPRITE createSpriteWH(float32 w, float32 h){
		return (HSORASPRITE)(new SoraSprite(SORA->createTextureWH(w, h)));
	}
	
	HSORATEXTURE createTexture(const SoraWString& path) {
		return SORA->createTexture(path);
	}
	
	HSORATEXTURE createTextureWH(float32 w, float32 h) {
		return SORA->createTextureWH(w, h);
	}
	
	void releaseTexture(HSORATEXTURE tex) {
		SORA->releaseTexture(tex);
	}
	
    void setTextureRect(HSORASPRITE h, float32 x, float32 y, float32 w, float32 fh) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setTextureRect(x, y, w, fh);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void setColor(HSORASPRITE h, float32 r, float32 g, float32 b, float32 a) {
        SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), -1);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
	
	void setVertexColor(HSORASPRITE h, float32 r, float32 g, float32 b, float32 a, int32 i) {
		SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), i);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
	}
    
    void setZ(HSORASPRITE h, float32 z, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setZ(z, i);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    ulong32 getColor(HSORASPRITE h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getColor(i);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    float32 getZ(HSORASPRITE h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getZ(i);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    void setCenter(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setCenter(x, y);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    float32 getCenterX(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterX();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0.f;
    }
    
    float32 getCenterY(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterY();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0.f;
    }
    
    void setFlip(HSORASPRITE h, bool hf, bool vf) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setFlip(hf, vf);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    bool getVFlip(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVFlip();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return false;
    }
    
    bool getHFlip(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHFlip();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return false;
    }
    
    int32 getTextureWidth(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureWidth();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    int32 getTextureHeight(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureHeight();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteWidth(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteWidth();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteHeight(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteHeight();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    void setScale(HSORASPRITE h, float32 vs, float32 hs) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setScale(vs, hs);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    float32 getVScale(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVScale();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0.f;
    }
    
    float32 getHScale(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHScale();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0.f;
    }
    
    void setRotation(HSORASPRITE h, float32 r) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setRotation(r);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    float32 getRotation(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getRotation();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0.f;
    }
    
    void setBlendMode(HSORASPRITE h, int32 mode) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setBlendMode(mode);
        else 
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    int32 getBlendMode(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getBlendMode();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return 0;
    }
    
    void addEffect  (HSORASPRITE h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->addEffect((SoraImageEffect*)eff);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void stopEffect (HSORASPRITE h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->stopEffect((SoraImageEffect*)eff);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void clearEffects (HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->clearEffects();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    bool hasEffect  (HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->hasEffect();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
        return false;
    }
    
    void update(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->update(sora::SORA->getDelta());
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void render(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void renderWithPos(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render(x, y);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    int32 getPosX(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionX();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    int32 getPosY(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionY();
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void setPosition(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setPosition(x, y);
        else
            throw SORA_EXCEPTION("Invalid sprite arg");
    }
    
    void release(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)h;
        if(p) {
            delete p;
            p = 0;
        }
    }
	
	void setAlpha(HSORASPRITE h, float32 a) {
		SoraSprite* p = (SoraSprite*)h;
		if(p) {
			SoraColorRGBA col = p->getColor();
			col.a = a;
			p->setColor(col.GetHWColor());
		}
	}
} // namespace spritewrapper

namespace imageeffectwrapper {
    
    ulong32 makeEffect_fade(float32 src, float32 dst, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectFade(src, dst, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_transitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectTransitions(sx, sy, dx, dy, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_scale_tensile(float32 sv, float32 dv, float32 sh, float32 dh, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectScale(sv, dv, sh, dh, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_scale(float32 s, float32 d, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectScale(s, d, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_colorTransitions(ulong32 start, ulong32 end, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectColorTransitions(start, end, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_rotation(float32 start, float32 end, float32 time, int32 mode) {
        return (ulong32)(new SoraImageEffectRotation(start, end, time, (sora::IMAGE_EFFECT_MODE)mode));
    }
    ulong32 makeEffect_rotationZ(float32 start, float32 zstart, float32 end, float32 zend, float32 time, int32 mode) {
		return (ulong32)(new SoraImageEffectRotation(start, zstart, end, zend, time, (sora::IMAGE_EFFECT_MODE)mode));
	}
}