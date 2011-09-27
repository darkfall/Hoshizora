#include "SoraSprite.h"

#include "SoraTextureMap.h"
#include "SoraCore.h"

#ifdef OS_IOS
#include "SoraiOSDeviceHelper.h"
#endif

namespace sora {
    
    SoraSprite::SoraSprite()  { 
        mTexture=0; 
        _initDefaults();
    }

    SoraSprite::SoraSprite(HSORATEXTURE tex) {
        SoraTexture* ptex = tex==0?NULL:(SoraTexture*)tex;
        _init(ptex, 0.f, 0.f, ptex!=NULL?ptex->mTextureWidth:1.f, ptex!=NULL?ptex->mTextureHeight:1.f);
		_initDefaults();
    }
    
    SoraSprite::SoraSprite(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h) {
        if(w == 0.f)
            w = (float32)SORA->getTextureWidth(tex);
        if(h == 0.f)
            h = (float32)SORA->getTextureHeight(tex);
        _init((SoraTexture*)tex, x, y, w, h);
		_initDefaults();
		setTextureRect(x, y, w, h);
    }
    
    void SoraSprite::_init(SoraTexture* tex, float32 x, float32 y, float32 width, float32 height) {
        float texx1, texy1, texx2, texy2;
		
		mTextureRect.x1 = x; mTextureRect.y1 = y;
		if(tex) {
			mTextureRect.x2 = width; mTextureRect.y2 = height;
		} else {
			mTextureRect.x2 = 1.f;
			mTextureRect.y2 = 1.f;
		}
		
		mQuad.tex = tex;
		mTexture = tex;
		
		texx1=x/mTextureRect.x2;
		texy1=y/mTextureRect.y2;
		texx2=(x+width)/mTextureRect.x2;
		texy2=(y+height)/mTextureRect.y2;

		mQuad.v[0].tx = texx1; mQuad.v[0].ty = texy1;
		mQuad.v[1].tx = texx2; mQuad.v[1].ty = texy1;
		mQuad.v[2].tx = texx2; mQuad.v[2].ty = texy2;
		mQuad.v[3].tx = texx1; mQuad.v[3].ty = texy2;
		
		if(tex) {
	       mSprWidth = static_cast<int32>(tex->mTextureWidth);
	       mSprHeight = static_cast<int32>(tex->mTextureHeight);
		} else {
			mSprWidth = 1;
			mSprHeight = 1;
		}
		if(mTexture && SoraTextureMap::Instance()->exist((HSORATEXTURE)tex)) {
			setName(SoraTextureMap::Instance()->getTextureName((HSORATEXTURE)tex));
			SoraTextureMap::Instance()->addRf((HSORATEXTURE)tex);
		}
    }

	SoraSprite::~SoraSprite() {
		clearEffects();
        clearShader();
	}

	void SoraSprite::setTexture(HSORATEXTURE tex) {
        if(mQuad.tex) {
            if(SoraTextureMap::Instance()->exist((HSORATEXTURE)mQuad.tex)) {
                SoraTextureMap::Instance()->decRf((HSORATEXTURE)mQuad.tex);
            }
        }
        
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			_init(ptex, 0.f, 0.f, (float32)ptex->mTextureWidth, (float32)ptex->mTextureHeight);
	}

	void SoraSprite::_initDefaults() {
        mVScale = mHScale = 1.f;
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            if(mTexture && !mTexture->mIsRetinaTexture) {
                mVScale = mHScale = getScaleFactor();
            }
        }
#endif
        
		bVFlip = bHFlip = false;
		mRotation = mRotationZ = 0.f;
		mCenterX = mCenterY = 0.f;
		if(hasEffect())
			clearEffects();
		setPosition(0.f, 0.f);
        
		mQuad.blend=BLEND_DEFAULT;
		
		setType(SPRITE_TYPE);
        mSora = SoraCore::Instance();
	}

	void SoraSprite::setTextureRect(float32 x, float32 y, float32 width, float32 height) {
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            float scale = sora::getScaleFactor();
            x *= scale; y *= scale;
            width *= scale; height *= scale;
        }
#endif
		float tx1, ty1, tx2, ty2;
		
		mTextureRect.x1=x;
		mTextureRect.y1=y;
		mTextureRect.x2=width;
		mTextureRect.y2=height;
        
        mSprWidth = (int32)width;
        mSprHeight = (int32)height;
        if(!mTexture)
			return;

		tx1=mTextureRect.x1/mTexture->mTextureWidth; ty1=mTextureRect.y1/mTexture->mTextureHeight;
		tx2=(mTextureRect.x1+width)/mTexture->mTextureWidth; ty2=(mTextureRect.y1+height)/mTexture->mTextureHeight;
		
		mQuad.v[0].tx=tx1; mQuad.v[0].ty=ty1; 
		mQuad.v[1].tx=tx2; mQuad.v[1].ty=ty1; 
		mQuad.v[2].tx=tx2; mQuad.v[2].ty=ty2; 
		mQuad.v[3].tx=tx1; mQuad.v[3].ty=ty2;
	}
	
	void SoraSprite::render() {
        render(getPositionX(), getPositionY());
    }

	void SoraSprite::render(float32 x, float32 y) {
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            x *= sora::getScaleFactor();
            y *= sora::getScaleFactor();
        }
#endif
		float32 tx1, ty1, tx2, ty2;
		float32 sint, cost;
	
		tx1 = -mCenterX*mHScale;
		ty1 = -mCenterY*mVScale;
		tx2 = (mTextureRect.x2-mCenterX)*mHScale;
		ty2 = (mTextureRect.y2-mCenterY)*mVScale;
		
		if(mRotation != 0.0f) {
			cost = cosf(mRotation);
			sint = sinf(mRotation);
			
			mQuad.v[0].x  = tx1*cost - ty1*sint + x;
			mQuad.v[1].x  = tx2*cost - ty1*sint + x;
			mQuad.v[2].x  = tx2*cost - ty2*sint + x;
			mQuad.v[3].x  = tx1*cost - ty2*sint + x;
		
			if(mRotationZ != 0.0f) {
				float32 costz = cosf(mRotationZ);
				mQuad.v[0].y = (tx1*sint + ty1*cost) * costz + y;
				mQuad.v[1].y = (tx2*sint + ty1*cost) * costz + y;	
				mQuad.v[2].y = (tx2*sint + ty2*cost) * costz + y;	
				mQuad.v[3].y = (tx1*sint + ty2*cost) * costz + y;	
			} else {
				mQuad.v[0].y  = tx1*sint + ty1*cost + y;	
				mQuad.v[1].y  = tx2*sint + ty1*cost + y;	
				mQuad.v[2].y  = tx2*sint + ty2*cost + y;	
				mQuad.v[3].y  = tx1*sint + ty2*cost + y;
			}
		}
		else {
			mQuad.v[0].x = tx1 + x; 
			mQuad.v[1].x = tx2 + x; 
			mQuad.v[2].x = mQuad.v[1].x; 
			mQuad.v[3].x = mQuad.v[0].x; 
					
			if(mRotationZ != 0.0f) {
				float32 costz = cosf(mRotationZ);
				mQuad.v[0].y = ty1 * costz + y;
				mQuad.v[1].y = ty1 * costz + y;	
				mQuad.v[2].y = ty2 * costz + y;	
				mQuad.v[3].y = ty2 * costz + y;	
			} else {
				mQuad.v[0].y = ty1 + y;
				mQuad.v[1].y = mQuad.v[0].y;
				mQuad.v[2].y = ty2 + y;
				mQuad.v[3].y = mQuad.v[2].y;
			}
		}
		
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
	}

	void SoraSprite::render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4) {
#ifdef OS_IOS
        float scale = sora::getScaleFactor();
        
		mQuad.v[0].x = x1 * scale; mQuad.v[0].y = y1 * scale;
		mQuad.v[1].x = x2 * scale; mQuad.v[1].y = y2 * scale;
		mQuad.v[2].x = x3 * scale; mQuad.v[2].y = y3 * scale;
		mQuad.v[3].x = x4 * scale; mQuad.v[3].y = y4 * scale;
#else
		mQuad.v[0].x = x1; mQuad.v[0].y = y1;
		mQuad.v[1].x = x2; mQuad.v[1].y = y2;
		mQuad.v[2].x = x3; mQuad.v[2].y = y3;
		mQuad.v[3].x = x4; mQuad.v[3].y = y4;
#endif
		
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
	}

	void SoraSprite::renderWithVertices(SoraVertex* vertices, uint32 size, int32 mode) {
		attachShaderToRender();
		mSora->renderWithVertices((HSORATEXTURE)mQuad.tex, mQuad.blend, vertices, size, mode);
		detachShaderFromRender();
	}
	
	void SoraSprite::setColor(uint32 c, int32 i) {
		if(i != -1)
			mQuad.v[i].col = c;
		else
			mQuad.v[0].col = mQuad.v[1].col = mQuad.v[2].col = mQuad.v[3].col = c;
	}

	uint32 SoraSprite::getColor(int32 i)  const{
		return mQuad.v[i].col;
	}
	
	void SoraSprite::setZ(float32 z, int32 i) {
		if(i != -1)
			mQuad.v[i].z = z;
		else
			mQuad.v[0].z = mQuad.v[1].z = mQuad.v[2].z = mQuad.v[3].z = z;
	}

	float32 SoraSprite::getZ(int32 i)  const{
		return mQuad.v[i].z;
	}

	void SoraSprite::setBlendMode(int32 mode) {
		mQuad.blend = mode;
	}

	int32 SoraSprite::getBlendMode() const {
		return mQuad.blend;
	}

	uint32* SoraSprite::getPixelData() const {
		return mSora->textureLock((HSORATEXTURE)mQuad.tex);
		return 0;
	}
    
    void SoraSprite::unlockPixelData() {
        mSora->textureUnlock((HSORASPRITE)mQuad.tex);
    }

	hgeRect SoraSprite::getTextureRect() const {
		return mTextureRect;
	}

	void SoraSprite::setCenter(float32 x, float32 y) { 
		mCenterX = x;
		mCenterY = y;
	}

	void SoraSprite::getCenter(float32& x, float32& y) {
		y = mCenterY;
		x = mCenterX;
	}

	void SoraSprite::setFlip(bool hflag, bool vflag, bool bFlipCenter) {
		float tx, ty;
        
        if(bCFlip & bHFlip) mCenterX = getSpriteWidth() - mCenterX;
        if(bCFlip & bVFlip) mCenterY = getSpriteHeight() - mCenterY;
        
        bCFlip = bFlipCenter;
        
        if(bCFlip & bHFlip) mCenterX = getSpriteWidth() - mCenterX;
        if(bCFlip & bVFlip) mCenterY = getSpriteHeight() - mCenterY;
        
        if(hflag != bHFlip) {
            tx=mQuad.v[0].tx; mQuad.v[0].tx=mQuad.v[1].tx; mQuad.v[1].tx=tx;
            ty=mQuad.v[0].ty; mQuad.v[0].ty=mQuad.v[1].ty; mQuad.v[1].ty=ty;
            tx=mQuad.v[3].tx; mQuad.v[3].tx=mQuad.v[2].tx; mQuad.v[2].tx=tx;
            ty=mQuad.v[3].ty; mQuad.v[3].ty=mQuad.v[2].ty; mQuad.v[2].ty=ty;
            
            bHFlip = !bHFlip;
        }
        
        if(vflag != bVFlip) {
            tx=mQuad.v[0].tx; mQuad.v[0].tx=mQuad.v[3].tx; mQuad.v[3].tx=tx;
            ty=mQuad.v[0].ty; mQuad.v[0].ty=mQuad.v[3].ty; mQuad.v[3].ty=ty;
            tx=mQuad.v[1].tx; mQuad.v[1].tx=mQuad.v[2].tx; mQuad.v[2].tx=tx;
            ty=mQuad.v[1].ty; mQuad.v[1].ty=mQuad.v[2].ty; mQuad.v[2].ty=ty;
            
            bVFlip = !bVFlip;
        }
	}

	int32 SoraSprite::getTextureWidth(bool bOriginal)  const{
		if(mTexture) return bOriginal?mTexture->mOriginalWidth:mTexture->mTextureWidth;
		return 0;
	}

	int32 SoraSprite::getTextureHeight(bool bOriginal) const {
		if(mTexture) return bOriginal?mTexture->mOriginalWidth:mTexture->mTextureHeight;
		return 0;
	}

	int32 SoraSprite::getSpriteWidth()  const{
		return (int32)(mSprWidth);
	}

	int32 SoraSprite::getSpriteHeight()  const{
		return (int32)(mSprHeight);
	}

	void SoraSprite::setScale(float32 h, float32 v) {
		mVScale = v;
		mHScale = h;
        
        // scale resources that are not retina resource(@2x)
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            if(mTexture && !mTexture->mIsRetinaTexture) {
                mVScale *= getScaleFactor();
                mHScale *= getScaleFactor();
            }
        }
#endif
	}

	float32 SoraSprite::getVScale()  const{
		return mVScale;
	}

	float32 SoraSprite::getHScale()  const{
		return mHScale;
	}

	void SoraSprite::setRotation(float32 r) { 
		mRotation = r;
	}

	float32 SoraSprite::getRotation()  const{
		return mRotation;
	}

	bool SoraSprite::getHFlip()  const{ 
		return bHFlip;
	}

	bool SoraSprite::getVFlip()  const{ 
		return bVFlip;
	}

	float32 SoraSprite::getCenterX()  const{
		return mCenterX;
	}

	float32 SoraSprite::getCenterY()  const{
		return mCenterY;
	}

	void SoraSprite::addEffect(SoraImageEffect* effect) {
		 vEffects.push_back(effect);
	}
	
	void SoraSprite::stopEffect(SoraImageEffect* _eff) {
		ImageEffectList::iterator eff = vEffects.begin();
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
		mRotationZ = rz;
	}
	
	float32 SoraSprite::getRotationZ() const {
		return mRotationZ;
	}
	
	void SoraSprite::clearEffects() {
		ImageEffectList::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
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
			ImageEffectList::iterator eff = vEffects.begin();
			while(eff != vEffects.end()) {
				int32 result = (*eff)->update(dt);
				(*eff)->modify(this);
				
				if(result == IMAGE_EFFECT_END) {     
					if((*eff)->isAutoRelease()) {
                        (*eff)->release();
                    }
					
					eff = vEffects.erase(eff);
					continue;
				}
				++eff;
			}
		}
		return 0;
	}
    
    HSORATEXTURE SoraSprite::getTexture() const {
        return (HSORATEXTURE)mTexture;
    }
    
    bool SoraSprite::hasEffect() const {
        return !vEffects.empty();
    }
    
    SoraRect SoraSprite::getBoundingBox() const {
        SoraRect rect;
        
        float32 tx1, ty1, tx2, ty2;
		float32 sint, cost;
        
		tx1 = -mCenterX*mHScale;
		ty1 = -mCenterY*mVScale;
		tx2 = (mTextureRect.x2-mCenterX)*mHScale;
		ty2 = (mTextureRect.y2-mCenterY)*mVScale;
		
		cost = cosf(mRotation);
        sint = sinf(mRotation);
			
        rect.x1  = tx1*cost - ty1*sint + getPositionX();
        rect.x2  = tx2*cost - ty2*sint + getPositionX();
        rect.y1  = tx1*sint + ty1*cost + getPositionY();	 
        rect.y2  = tx2*sint + ty2*cost + getPositionY();
        
        return rect;
    }
	
} // namespace sora