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
    
	HSORASPRITE createSpriteWH(int32 w, int32 h){
		return (HSORASPRITE)(new SoraSprite(SORA->createTextureWH(w, h)));
	}
	
	HSORATEXTURE createTexture(const SoraWString& path) {
		return SORA->createTexture(path);
	}
	
	HSORATEXTURE createTextureWH(int32 w, int32 h) {
		return SORA->createTextureWH(w, h);
	}
	
	void releaseTexture(HSORATEXTURE tex) {
		SORA->releaseTexture(tex);
	}
	
    void setTextureRect(HSORASPRITE h, float32 x, float32 y, float32 w, float32 fh) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setTextureRect(x, y, w, fh);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void setColor(HSORASPRITE h, float32 r, float32 g, float32 b, float32 a) {
        SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), -1);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
	
	void setVertexColor(HSORASPRITE h, float32 r, float32 g, float32 b, float32 a, int32 i) {
		SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
	}
    
    void setZ(HSORASPRITE h, float32 z, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setZ(z, i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    ulong32 getColor(HSORASPRITE h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getColor(i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    float32 getZ(HSORASPRITE h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getZ(i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void setCenter(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setCenter(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float32 getCenterX(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterX();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    float32 getCenterY(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterY();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setFlip(HSORASPRITE h, bool hf, bool vf) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setFlip(hf, vf);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    bool getVFlip(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVFlip();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    bool getHFlip(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHFlip();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    int32 getTextureWidth(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureWidth();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getTextureHeight(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureHeight();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteWidth(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteWidth();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteHeight(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteHeight();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void setScale(HSORASPRITE h, float32 vs, float32 hs) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setScale(vs, hs);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float32 getVScale(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVScale();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    float32 getHScale(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHScale();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setRotation(HSORASPRITE h, float32 r) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setRotation(r);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float32 getRotation(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getRotation();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setBlendMode(HSORASPRITE h, int32 mode) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setBlendMode(mode);
        else 
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    int32 getBlendMode(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getBlendMode();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void addEffect  (HSORASPRITE h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->addEffect((SoraImageEffect*)eff);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void stopEffect (HSORASPRITE h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->stopEffect((SoraImageEffect*)eff);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void clearEffects (HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->clearEffects();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    bool hasEffect  (HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->hasEffect();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    void update(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->update(sora::SORA->getDelta());
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void render(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void renderWithPos(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float32 getPosX(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionX();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float32 getPosY(HSORASPRITE h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionY();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void setPosition(HSORASPRITE h, float32 x, float32 y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setPosition(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
	
	ulong32 getTexture(HSORASPRITE h) {
		SoraSprite* p = (SoraSprite*)(h);
        if(p) {
			return p->getTexture();
		}
		return 0;
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
	
	ulong32 attachShader(HSORASPRITE h, const SoraWString& shader, const SoraString& entry, int32 type) {
		SoraSprite* p = (SoraSprite*)h;
		if(p) {
			return (ulong32)p->attachShader(shader, entry, type);
		}
		return 0;
	}
	
	void setShaderParam1f(ulong32 shader, const char* name, float32 param) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			ps->setParameterfv(name, &param, 1);
		}
	}
	
	void setShaderParam2f(ulong32 shader, const char* name, float32 p1, float32 p2) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float32 val[2];
			val[0] = p1;
			val[1] = p2;
			ps->setParameterfv(name, val, 2);
		}
	}
	
	void setShaderParam3f(ulong32 shader, const char* name, float32 p1, float32 p2, float32 p3) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float32 val[3];
			val[0] = p1;
			val[1] = p2;
			val[2] = p3;
			ps->setParameterfv(name, val, 3);
		}
	}
	
	void setShaderParam4f(ulong32 shader, const char* name, float32 p1, float32 p2, float32 p3, float32 p4) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float32 val[4];
			val[0] = p1;
			val[1] = p2;
			val[2] = p3;
			val[3] = p4;
			ps->setParameterfv(name, val, 4);
		}
	}
	
	void setShaderParam1i(ulong32 shader, const char* name, int32 param) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			ps->setParameteriv(name, &param, 1);
		}
	}
	
	void setShaderParam2i(ulong32 shader, const char* name, int32 p1, int32 p2) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			int32 val[2];
			val[0] = p1;
			val[1] = p2;
			ps->setParameteriv(name, val, 2);
		}
		
	}
	
	void setShaderParam3i(ulong32 shader, const char* name, int32 p1, int32 p2, int32 p3) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			int32 val[3];
			val[0] = p1;
			val[1] = p2;
			val[2] = p3;
			ps->setParameteriv(name, val, 3);
		}
	}
	
	void setShaderParam4i(ulong32 shader, const char* name, int32 p1, int32 p2, int32 p3, int32 p4) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			int32 val[4];
			val[0] = p1;
			val[1] = p2;
			val[2] = p3;
			val[3] = p4;
			ps->setParameteriv(name, val, 4);
		}
	}
	
	void setShaderTextureParam(ulong32 shader, const char* name, ulong32 tex) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			ps->setTexture(name, tex);
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