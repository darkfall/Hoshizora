#include "SoraSprite.h"

#include "SoraTextureMap.h"
#include "SoraCore.h"

namespace sora {

    SoraSprite::SoraSprite(HSORATEXTURE tex): shaderContext(NULL) {
        SoraTexture* ptex = tex==0?NULL:(SoraTexture*)tex;
        _init(ptex, 0.f, 0.f, ptex!=NULL?ptex->mTextureWidth:1.f, ptex!=NULL?ptex->mTextureHeight:1.f);
		_initDefaults();
    }
    
    SoraSprite::SoraSprite(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h): shaderContext(NULL) {
        _init((SoraTexture*)tex, x, y, w, h);
		_initDefaults();
    }
    
    void SoraSprite::_init(SoraTexture* tex, float32 x, float32 y, float32 width, float32 height) {
        float texx1, texy1, texx2, texy2;
		
		textureRect.x1 = 0.f; textureRect.y1 = 0.f;
		if(tex) {
			textureRect.x2 = width; textureRect.y2 = height;
		} else {
			textureRect.x2 = 1.f;
			textureRect.y2 = 1.f;
		}
		
		quad.tex = tex;
		texture = tex;
		
		texx1=x/textureRect.x2;
		texy1=y/textureRect.y2;
		texx2=(x+width)/textureRect.x2;
		texy2=(y+height)/textureRect.y2;

		quad.v[0].tx = texx1; quad.v[0].ty = texy1;
		quad.v[1].tx = texx2; quad.v[1].ty = texy1;
		quad.v[2].tx = texx2; quad.v[2].ty = texy2;
		quad.v[3].tx = texx1; quad.v[3].ty = texy2;
		
        sprWidth = texture!=NULL?texture->mOriginalWidth:1;
        sprHeight = texture!=NULL?texture->mOriginalHeight:1;
		
		shaderContext = NULL;
    }

	SoraSprite::~SoraSprite() {
		clearEffects();
        clearShader();
	//	if(texture) delete texture;
		SoraTextureMap::Instance()->decRf((HSORATEXTURE)texture);
	}

	void SoraSprite::setTexture(HSORATEXTURE tex) {
        if(quad.tex) {
            if(SoraTextureMap::Instance()->exist((HSORATEXTURE)quad.tex)) {
                sora::SORA->releaseTexture((HSORATEXTURE)quad.tex);
            }
        }
        
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			_init(ptex, 0.f, 0.f, (float32)ptex->mTextureWidth, (float32)ptex->mTextureHeight);
	}

	void SoraSprite::_initDefaults() {
		vscale = hscale = 1.f;
		bVFlip = bHFlip = false;
		rot = rotZ = 0.f;
		centerX = centerY = 0.f;
		if(shaderContext)
			clearShader();
		if(hasEffect())
			clearEffects();
		setPosition(0.f, 0.f);

		quad.v[0].z = 
		quad.v[1].z = 
		quad.v[2].z = 
		quad.v[3].z = 0.f;
		
		quad.v[0].col = 
		quad.v[1].col = 
		quad.v[2].col = 
		quad.v[3].col = 0xffffffff;
        
		quad.blend=BLEND_DEFAULT;
		
		setType(SPRITE_TYPE);
        sora = SoraCore::Instance();
	}

	void SoraSprite::setTextureRect(float32 x, float32 y, float32 width, float32 height) {
		if(!texture)
			return;
		
		float tx1, ty1, tx2, ty2;
		
		textureRect.x1=x;
		textureRect.y1=y;
		textureRect.x2=width;
		textureRect.y2=height;

		tx1=textureRect.x1/texture->mTextureWidth; ty1=textureRect.y1/texture->mTextureHeight;
		tx2=(textureRect.x1+width)/texture->mTextureWidth; ty2=(textureRect.y1+height)/texture->mTextureHeight;
		
		quad.v[0].tx=tx1; quad.v[0].ty=ty1; 
		quad.v[1].tx=tx2; quad.v[1].ty=ty1; 
		quad.v[2].tx=tx2; quad.v[2].ty=ty2; 
		quad.v[3].tx=tx1; quad.v[3].ty=ty2; 
		
        sprWidth = (int32)width;
        sprHeight = (int32)height;
	}

	
	void SoraSprite::render() {
        render(getPositionX(), getPositionY());
    }

	void SoraSprite::render(float32 x, float32 y) {
		float32 tx1, ty1, tx2, ty2;
		float32 sint, cost;
	
		tx1 = -centerX*hscale;
		ty1 = -centerY*vscale;
		tx2 = (textureRect.x2-centerX)*hscale;
		ty2 = (textureRect.y2-centerY)*vscale;
		
		if(rot != 0.0f) {
			cost = cosf(rot);
			sint = sinf(rot);
			
			quad.v[0].x  = tx1*cost - ty1*sint + x;
			quad.v[1].x  = tx2*cost - ty1*sint + x;
			quad.v[2].x  = tx2*cost - ty2*sint + x;
			quad.v[3].x  = tx1*cost - ty2*sint + x;
		
			if(rotZ != 0.0f) {
				float32 costz = cosf(rotZ);
				quad.v[0].y = (tx1*sint + ty1*cost) * costz + y;
				quad.v[1].y = (tx2*sint + ty1*cost) * costz + y;	
				quad.v[2].y = (tx2*sint + ty2*cost) * costz + y;	
				quad.v[3].y = (tx1*sint + ty2*cost) * costz + y;	
			} else {
				quad.v[0].y  = tx1*sint + ty1*cost + y;	
				quad.v[1].y  = tx2*sint + ty1*cost + y;	
				quad.v[2].y  = tx2*sint + ty2*cost + y;	
				quad.v[3].y  = tx1*sint + ty2*cost + y;
			}
		}
		else {
			quad.v[0].x = tx1 + x; 
			quad.v[1].x = tx2 + x; 
			quad.v[2].x = quad.v[1].x; 
			quad.v[3].x = quad.v[0].x; 
					
			if(rotZ != 0.0f) {
				float32 costz = cosf(rotZ);
				quad.v[0].y = ty1 * costz + y;
				quad.v[1].y = ty1 * costz + y;	
				quad.v[2].y = ty2 * costz + y;	
				quad.v[3].y = ty2 * costz + y;	
			} else {
				quad.v[0].y = ty1 + y;
				quad.v[1].y = quad.v[0].y;
				quad.v[2].y = ty2 + y;
				quad.v[3].y = quad.v[2].y;
			}
		}
		
        if(hasShader()) 
            SORA->attachShaderContext(shaderContext);
		SORA->renderQuad(quad);
        if(hasShader())
            SORA->detachShaderContext();
	}

	void SoraSprite::render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4) {
		quad.v[0].x = x1; quad.v[0].y = y1;
		quad.v[1].x = x2; quad.v[1].y = y2;
		quad.v[2].x = x3; quad.v[2].y = y3;
		quad.v[3].x = x4; quad.v[3].y = y4;
		
        if(hasShader()) 
            sora->attachShaderContext(shaderContext);
		sora->renderQuad(quad);
        if(hasShader()) 
            sora->detachShaderContext();
	}

	void SoraSprite::setColor(ulong32 c, int32 i) {
		if(i != -1)
			quad.v[i].col = c;
		else
			quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = c;
	}

	ulong32 SoraSprite::getColor(int32 i)  const{
		return quad.v[i].col;
	}
	
	void SoraSprite::setZ(float32 z, int32 i) {
		if(i != -1)
			quad.v[i].z = z;
		else
			quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = z;
	}

	float32 SoraSprite::getZ(int32 i)  const{
		return quad.v[i].z;
	}

	void SoraSprite::setBlendMode(int32 mode) {
		quad.blend = mode;
	}

	int32 SoraSprite::getBlendMode() const {
		return quad.blend;
	}

	ulong32* SoraSprite::getPixelData() const {
		return SORA->textureLock((HSORATEXTURE)quad.tex);
		return 0;
	}
    
    void SoraSprite::unlockPixelData() {
        SORA->textureUnlock((HSORASPRITE)quad.tex);
    }

	const hgeRect& SoraSprite::getTextureRect() const {
		return textureRect;
	}

	void SoraSprite::setCenter(float32 x, float32 y) { 
		centerX = x;
		centerY = y;
	}

	void SoraSprite::getCenter(float32& x, float32& y) {
		y = centerY;
		x = centerX;
	}

	void SoraSprite::setFlip(bool hflag, bool vflag, bool bFlipCenter) {
		float tx, ty;
        
        if(bCFlip & bHFlip) centerX = getSpriteWidth() - centerX;
        if(bCFlip & bVFlip) centerY = getSpriteHeight() - centerY;
        
        bCFlip = bFlipCenter;
        
        if(bCFlip & bHFlip) centerX = getSpriteWidth() - centerX;
        if(bCFlip & bVFlip) centerY = getSpriteHeight() - centerY;
        
        if(hflag != bHFlip) {
            tx=quad.v[0].tx; quad.v[0].tx=quad.v[1].tx; quad.v[1].tx=tx;
            ty=quad.v[0].ty; quad.v[0].ty=quad.v[1].ty; quad.v[1].ty=ty;
            tx=quad.v[3].tx; quad.v[3].tx=quad.v[2].tx; quad.v[2].tx=tx;
            ty=quad.v[3].ty; quad.v[3].ty=quad.v[2].ty; quad.v[2].ty=ty;
            
            bHFlip = !bHFlip;
        }
        
        if(vflag != bVFlip) {
            tx=quad.v[0].tx; quad.v[0].tx=quad.v[3].tx; quad.v[3].tx=tx;
            ty=quad.v[0].ty; quad.v[0].ty=quad.v[3].ty; quad.v[3].ty=ty;
            tx=quad.v[1].tx; quad.v[1].tx=quad.v[2].tx; quad.v[2].tx=tx;
            ty=quad.v[1].ty; quad.v[1].ty=quad.v[2].ty; quad.v[2].ty=ty;
            
            bVFlip = !bVFlip;
        }
	}

	int32 SoraSprite::getTextureWidth(bool bOriginal)  const{
		if(texture) return bOriginal?texture->mTextureWidth:texture->mOriginalWidth;
		return 0;
	}

	int32 SoraSprite::getTextureHeight(bool bOriginal) const {
		if(texture) return bOriginal?texture->mTextureWidth:texture->mOriginalWidth;
		return 0;
	}

	int32 SoraSprite::getSpriteWidth()  const{
		return (int32)(sprWidth);
	}

	int32 SoraSprite::getSpriteHeight()  const{
		return (int32)(sprHeight);
	}

	void SoraSprite::setScale(float32 h, float32 v) {
		vscale = v;
		hscale = h;
	}

	float32 SoraSprite::getVScale()  const{
		return vscale;
	}

	float32 SoraSprite::getHScale()  const{
		return hscale;
	}

	void SoraSprite::setRotation(float32 r) { 
		rot = r;
	}

	float32 SoraSprite::getRotation()  const{
		return rot;
	}

	bool SoraSprite::getHFlip()  const{ 
		return bHFlip;
	}

	bool SoraSprite::getVFlip()  const{ 
		return bVFlip;
	}

	float32 SoraSprite::getCenterX()  const{
		return centerX;
	}

	float32 SoraSprite::getCenterY()  const{
		return centerY;
	}

	void SoraSprite::addEffect(SoraImageEffect* effect) {
		 vEffects.push_back(effect);
	}
	
	void SoraSprite::stopEffect(SoraImageEffect* _eff) {
		IMAGE_EFFECT_LIST::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
			if((*eff) == _eff) {
				if((*eff)->isInList()) {
					(*eff)->clearList();
				}
				delete _eff;
				_eff = 0;
				eff = vEffects.erase(eff);
				return;
			} else
				++eff;
		}
	}
	
	void SoraSprite::setRotationZ(float32 rz) {
		rotZ = rz;
	}
	
	float32 SoraSprite::getRotationZ() const {
		return rotZ;
	}
	
	void SoraSprite::clearEffects() {
		IMAGE_EFFECT_LIST::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
			if((*eff)->isInList()) {
				(*eff)->clearList();
			}
			if((*eff) != NULL) {
				delete (*eff);
				(*eff) = 0;
			}
			++eff;
		}
		vEffects.clear();
	}
	
	uint32 SoraSprite::update(float32 dt) {
		if(!vEffects.empty()) {
			IMAGE_EFFECT_LIST::iterator eff = vEffects.begin();
			while(eff != vEffects.end()) {
				uint32 result = (*eff)->update(dt);
				switch((*eff)->getType()) {
					case IMAGE_EFFECT_FADEIN:
					case IMAGE_EFFECT_FADEOUT:
						setColor(CSETA(getColor(), (*eff)->get1st()*255));
						break;
						
					case IMAGE_EFFECT_TRANSITIONS:
						setPosition((*eff)->get1st(), (*eff)->get2nd());
						break;
						
					case IMAGE_EFFECT_TRANSITIONS_Z:
						setPosition((*eff)->get1st(), (*eff)->get2nd());
						setZ((*eff)->get3rd());
						break;
						
					case IMAGE_EFFECT_TENSILE:
					case IMAGE_EFFECT_SCALEIN:
					case IMAGE_EFFECT_SCALEOUT:
						setScale((*eff)->get1st(), (*eff)->get2nd());
						break;
						
					case IMAGE_EFFECT_COLOR_TRANSITION:
						setColor(SoraColorRGBA::GetHWColor((*eff)->get1st(), (*eff)->get2nd(), (*eff)->get3rd(), (*eff)->get4th()));
						break;
						
					case IMAGE_EFFECT_ROTATE:
						setRotation((*eff)->get1st());
						break;
					case IMAGE_EFFECT_ROTATE_Z:
						setRotation((*eff)->get1st());
						setRotationZ((*eff)->get2nd());
						break;
				}
				
				if(result == IMAGE_EFFECT_END) {     
					if(!(*eff)->isInList()) {
						delete (*eff);
						(*eff) = 0;
						
						eff = vEffects.erase(eff);
						continue;
					} else {
						SoraImageEffect* nexteff = (*eff)->getNext();
						
						(*eff) = nexteff;
						if(nexteff == NULL) {
							(*eff)->clearList();
							
							delete (*eff);
							(*eff) = 0;
							
							eff = vEffects.erase(eff);
							continue;
						} else (*eff)->restart();
					}
				} else if(result != IMAGE_EFFECT_PLAYING) {
					if((*eff)->isInList()) {
						if(result == IMAGE_EFFECT_END_OF_LIST && (*eff)->getListMode() == IMAGE_EFFECT_REPEAT) {
							(*eff) = (*eff)->getListHead();
							(*eff)->restart();
						}
						else if(result == IMAGE_EFFECT_TONEXT) {
							SoraImageEffect* nexteff = (*eff)->getNext();
							(*eff) = nexteff;
							(*eff)->restart();
						} else if(result == IMAGE_EFFECT_TOPREV) {
							SoraImageEffect* preveff = (*eff)->getPrev();
							(*eff) = preveff;
							(*eff)->restart();
						}
					}
				}
				++eff;
			}
		}
		return 0;
	}
    
    SoraShader* SoraSprite::attachShader(const SoraWString& shaderPath, const SoraString& entry, SORA_SHADER_TYPE type) {
        if(!shaderContext) {
            shaderContext = sora->createShaderContext();
            if(!shaderContext)
                return NULL;
        }
        SoraShader* shader = shaderContext->attachShader(shaderPath, entry, type);
	    return shader;
    }
    
    void SoraSprite::attachShader(SoraShader* shader) {
        if(!shaderContext) {
            shaderContext = sora->createShaderContext();
            if(!shaderContext)
                return;
        }
        shaderContext->attachShader(shader);
    }
    
    void SoraSprite::detachShader(SoraShader* shader) {
        if(shaderContext) {
            shaderContext->detachShader(shader);
        }
    }
    
    bool SoraSprite::hasShader() const {
        return (shaderContext != NULL && shaderContext->getShaderSize() != 0);
    }
	
	bool SoraSprite::hasEffect() const {
		return !vEffects.empty();
	}
    
    void SoraSprite::clearShader() {
        if(shaderContext) {
            delete shaderContext;
            shaderContext = 0;
        }
    }
    
    HSORATEXTURE SoraSprite::getTexture() const {
        return (HSORATEXTURE)texture;
    }
    
	
} // namespace sora