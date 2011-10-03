//
//  spriteWrapper.cpp
//  Sora
//
//  Created by Griffin Bu on 3/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "spriteWrapper.h"

#include "SoraImageEffect.h"

namespace spritewrapper {
    
    SoraSpriteHandle createSprite(const SoraWString& path) {
        return (SoraSpriteHandle)SORA->createSprite(path);
    }
    
    SoraSpriteHandle createSpriteWithTex(SoraTextureHandle tex) {
        return (SoraSpriteHandle)(new SoraSprite(tex));
    }
    
    SoraSpriteHandle createSpriteWithTexEx(SoraTextureHandle tex, float x, float y, float w, float h) {
        return (SoraSpriteHandle)(new SoraSprite(tex, x, y, w, h));
    }
    
	SoraSpriteHandle createSpriteWH(int32 w, int32 h){
		return (SoraSpriteHandle)(new SoraSprite(SORA->createTextureWH(w, h)));
	}
	
	SoraTextureHandle createTexture(const SoraWString& path) {
		return SORA->createTexture(path);
	}
	
	SoraTextureHandle createTextureWH(int32 w, int32 h) {
		return SORA->createTextureWH(w, h);
	}
	
	void releaseTexture(SoraTextureHandle tex) {
		SORA->releaseTexture(tex);
	}
	
    void setTextureRect(SoraSpriteHandle h, float x, float y, float w, float fh) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setTextureRect(x, y, w, fh);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void setColor(SoraSpriteHandle h, float r, float g, float b, float a) {
        SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), -1);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
	
	void setVertexColor(SoraSpriteHandle h, float r, float g, float b, float a, int32 i) {
		SoraSprite* p = (SoraSprite*)(h);
		SoraColorRGBA color(r, g, b, a);
        if(p) p->setColor(color.GetHWColor(), i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
	}
    
    void setZ(SoraSpriteHandle h, float z, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setZ(z, i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    ulong32 getColor(SoraSpriteHandle h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getColor(i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    float getZ(SoraSpriteHandle h, int32 i) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getZ(i);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void setCenter(SoraSpriteHandle h, float x, float y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setCenter(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float getCenterX(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterX();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    float getCenterY(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getCenterY();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setFlip(SoraSpriteHandle h, bool hf, bool vf) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setFlip(hf, vf);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    bool getVFlip(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVFlip();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    bool getHFlip(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHFlip();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    int32 getTextureWidth(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureWidth();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getTextureHeight(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getTextureHeight();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteWidth(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteWidth();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    int32 getSpriteHeight(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getSpriteHeight();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void setScale(SoraSpriteHandle h, float vs, float hs) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setScale(vs, hs);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float getVScale(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getVScale();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    float getHScale(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getHScale();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setRotation(SoraSpriteHandle h, float r) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setRotation(r);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float getRotation(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getRotation();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0.f;
    }
    
    void setBlendMode(SoraSpriteHandle h, int32 mode) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setBlendMode(mode);
        else 
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    int32 getBlendMode(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getBlendMode();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return 0;
    }
    
    void addEffect  (SoraSpriteHandle h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->addEffect((SoraImageEffect*)eff);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void stopEffect (SoraSpriteHandle h, ulong32 eff) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->stopEffect((SoraImageEffect*)eff);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void clearEffects (SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->clearEffects();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    bool hasEffect  (SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->hasEffect();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
        return false;
    }
    
    void update(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->update(sora::SORA->getDelta());
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void render(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void renderWithPos(SoraSpriteHandle h, float x, float y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->render(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float getPosX(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionX();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    float getPosY(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) return p->getPositionY();
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
    
    void setPosition(SoraSpriteHandle h, float x, float y) {
        SoraSprite* p = (SoraSprite*)(h);
        if(p) p->setPosition(x, y);
        else
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Invalid sprite arg");
    }
	
	ulong32 getTexture(SoraSpriteHandle h) {
		SoraSprite* p = (SoraSprite*)(h);
        if(p) {
			return p->getTexture();
		}
		return 0;
	}
	
    void release(SoraSpriteHandle h) {
        SoraSprite* p = (SoraSprite*)h;
        if(p) {
            delete p;
            p = 0;
        }
    }
	
	void setAlpha(SoraSpriteHandle h, float a) {
		SoraSprite* p = (SoraSprite*)h;
		if(p) {
			SoraColorRGBA col = p->getColor();
			col.a = a;
			p->setColor(col.GetHWColor());
		}
	}
	
	ulong32 attachShader(SoraSpriteHandle h, const SoraWString& shader, const SoraString& entry, int32 type) {
		SoraSprite* p = (SoraSprite*)h;
		if(p) {
			return (ulong32)p->attachShader(shader, entry, type);
		}
		return 0;
	}
	
	void setShaderParam1f(ulong32 shader, const char* name, float param) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			ps->setParameterfv(name, &param, 1);
		}
	}
	
	void setShaderParam2f(ulong32 shader, const char* name, float p1, float p2) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float val[2];
			val[0] = p1;
			val[1] = p2;
			ps->setParameterfv(name, val, 2);
		}
	}
	
	void setShaderParam3f(ulong32 shader, const char* name, float p1, float p2, float p3) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float val[3];
			val[0] = p1;
			val[1] = p2;
			val[2] = p3;
			ps->setParameterfv(name, val, 3);
		}
	}
	
	void setShaderParam4f(ulong32 shader, const char* name, float p1, float p2, float p3, float p4) {
		SoraShader* ps = (SoraShader*)shader;
		if(ps) {
			float val[4];
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
    
    ulong32 makeEffect_fade(float src, float dst, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectFade(src, dst, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_transitions(float sx, float sy, float dx, float dy, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectTransitions(sx, sy, dx, dy, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_scale_tensile(float sv, float dv, float sh, float dh, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectScale(sv, dv, sh, dh, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_scale(float s, float d, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectScale(s, d, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_colorTransitions(ulong32 start, ulong32 end, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectColorTransitions(start, end, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_rotation(float start, float end, float time, int32 mode) {
        return (ulong32)(new SoraImageEffectRotation(start, end, time, (sora::ImageEffectMode)mode));
    }
    ulong32 makeEffect_rotationZ(float start, float zstart, float end, float zend, float time, int32 mode) {
		return (ulong32)(new SoraImageEffectRotation(start, zstart, end, zend, time, (sora::ImageEffectMode)mode));
	}
}