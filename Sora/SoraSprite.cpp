#include "SoraSprite.h"

#include "SoraTextureMap.h"
#include "SoraCore.h"

namespace sora {

    SoraSprite::SoraSprite(HSORATEXTURE tex) {
        SoraSprite((SoraTexture*)tex);
    }
    
    SoraSprite::SoraSprite(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h) {
        SoraSprite((SoraTexture*)tex, x, y, w, h);
    }
    
	SoraSprite::SoraSprite(SoraTexture* tex) {
		float texx1, texy1, texx2, texy2;
		
		sora = SoraCore::Instance();
		
		textureRect.x1 = 0.f; textureRect.y1 = 0.f;
		if(tex) {
			textureRect.x2 = tex->mTextureWidth; textureRect.y2 = tex->mTextureHeight;
		} else {
			textureRect.x2 = 1.f;
			textureRect.y2 = 1.f;
		}
		quad.tex = tex;
		
		texx1 = 0.f;
		texy1 = 0.f;
		texx2 = 1.f;
		texy2 = 1.f;
		
		quad.v[0].tx = texx1; quad.v[0].ty = texy1;
		quad.v[1].tx = texx2; quad.v[1].ty = texy1;
		quad.v[2].tx = texx2; quad.v[2].ty = texy2;
		quad.v[3].tx = texx1; quad.v[3].ty = texy2;
		
		quad.v[0].z = 
		quad.v[1].z = 
		quad.v[2].z = 
		quad.v[3].z = 0.0f;
		
		quad.v[0].col = 
		quad.v[1].col = 
		quad.v[2].col = 
		quad.v[3].col = 0xffffffff;

		quad.blend=BLEND_DEFAULT;
		
		texture = tex;

		_initDefaults();
		
		setType(SPRITE_TYPE);
	}

	SoraSprite::SoraSprite(SoraTexture* tex, float32 x, float32 y, float32 width, float32 height) {
		float texx1, texy1, texx2, texy2;
		
		textureRect.x1 = 0.f; textureRect.y1 = 0.f;
		if(tex) {
			textureRect.x2 = width; textureRect.y2 = height;
		} else {
			textureRect.x2 = 1.f;
			textureRect.y2 = 1.f;
		}
		
		quad.tex = tex;
		
		texx1= x/textureRect.x2;
		texy1= y/textureRect.y2;
		texx2=(x+width)/textureRect.x2;
		texy2=(x+height)/textureRect.y2;
		
		quad.v[0].tx = texx1; quad.v[0].ty = texy1;
		quad.v[1].tx = texx2; quad.v[1].ty = texy1;
		quad.v[2].tx = texx2; quad.v[2].ty = texy2;
		quad.v[3].tx = texx1; quad.v[3].ty = texy2;
		
		quad.v[0].z = 
		quad.v[1].z = 
		quad.v[2].z = 
		quad.v[3].z = 0.f;
		
		quad.v[0].col = 
		quad.v[1].col = 
		quad.v[2].col = 
		quad.v[3].col = 0xffffffff;

		quad.blend=BLEND_DEFAULT;
		
		texture = tex;

		_initDefaults();
		
		setType(SPRITE_TYPE);
	}

	SoraSprite::~SoraSprite() {
		clearEffects();
        clearShader();
	//	if(texture) delete texture;
		SoraTextureMap::Instance()->decRf((HSORATEXTURE)texture);
	}

	void SoraSprite::setTexture(SoraTexture* tex) {
		float tx1,ty1,tx2,ty2;
		float tw,th;
		
		quad.tex=tex;
		
		if(tex) {
			tw = (float)tex->mTextureWidth;
			th = (float)tex->mTextureHeight;
		}
		else {
			tw = 1.0f;
			th = 1.0f;
		}
		
		if(tw!=textureRect.x2 || th!=textureRect.y2) {
			tx1=quad.v[0].tx*textureRect.x2;
			ty1=quad.v[0].ty*textureRect.y2;
			tx2=quad.v[2].tx*textureRect.x2;
			ty2=quad.v[2].ty*textureRect.y2;
			
			textureRect.x2=tw;
			textureRect.y2=th;
			
			tx1/=tw; ty1/=th;
			tx2/=tw; ty2/=th;
			
			quad.v[0].tx=tx1; quad.v[0].ty=ty1; 
			quad.v[1].tx=tx2; quad.v[1].ty=ty1; 
			quad.v[2].tx=tx2; quad.v[2].ty=ty2; 
			quad.v[3].tx=tx1; quad.v[3].ty=ty2; 
		}
		
		texture = tex;
	}

	void SoraSprite::_initDefaults() {
		vscale = hscale = 1.f;
		bVFlip = bHFlip = false;
		rot = rotZ = 0.f;
		centerX = centerY = 0.f;
        shaderContext = NULL;
	}

	void SoraSprite::setTextureRect(float32 x, float32 y, float32 width, float32 height) {
		if(!texture)
			return;
		
		float tx1, ty1, tx2, ty2;
		
		textureRect.x1=x;
		textureRect.y1=y;
		textureRect.x2=width;
		textureRect.y2=height;
	/*	if(adjSize)
		{
			width=w;
			height=h;
		}*/
		
		tx1=textureRect.x1/texture->mTextureWidth; ty1=textureRect.y1/texture->mTextureHeight;
		tx2=(textureRect.x1+width)/texture->mTextureWidth; ty2=(textureRect.y1+height)/texture->mTextureHeight;
		
		quad.v[0].tx=tx1; quad.v[0].ty=ty1; 
		quad.v[1].tx=tx2; quad.v[1].ty=ty1; 
		quad.v[2].tx=tx2; quad.v[2].ty=ty2; 
		quad.v[3].tx=tx1; quad.v[3].ty=ty2; 
		
	/*	bX=bXFlip; bY=bYFlip; bHS=bHSFlip;
		bXFlip=false; bYFlip=false;
		
		SetFlip(bX, bY, bHS);*/
		textureRect.Set(x, y, width, height);
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
            sora->attachShaderContext(shaderContext);
		sora->renderQuad(quad);
        if(hasShader())
            sora->detachShaderContext();
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
		return sora->textureLock((HSORATEXTURE)texture);
		return 0;
	}
    
    void SoraSprite::unlockPixelData() {
        sora->textureUnlock((HSORASPRITE)texture);
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

	void SoraSprite::setFlip(bool hflag, bool vflag) {
		bHFlip = hflag;
		bVFlip = vflag;
	}

	int32 SoraSprite::getTextureWidth()  const{
		if(texture) return texture->mOriginalWidth;
		return 0;
	}

	int32 SoraSprite::getTextureHeight() const {
		if(texture) return texture->mOriginalHeight;
		return 0;
	}

	int32 SoraSprite::getSpriteWidth()  const{
		return (int32)textureRect.x2*hscale;
	}

	int32 SoraSprite::getSpriteHeight()  const{
		return (int32)textureRect.y2*vscale;
	}

	int32 SoraSprite::getSpritePosX()  const{
		return (int32)textureRect.x1;
	}

	int32 SoraSprite::getSpritePosY()  const{
		return (int32)textureRect.y1;
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
			delete (*eff);
			(*eff) = 0;
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
					delete (*eff);
					(*eff) = 0;
					eff = vEffects.erase(eff);
				} else
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
    
	
} // namespace sora