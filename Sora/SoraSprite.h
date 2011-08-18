#ifndef SORA_SPRITE_H_
#define SORA_SPRITE_H_

#include "SoraTexture.h"
#include "SoraColor.h"

#include "hgerect.h"
#include "SoraImageEffect.h"
#include "SoraObject.h"
#include "SoraShaderEnabled.h"

#include <list>

namespace sora {

	class SoraCore;
	
#define SPRITE_TYPE 0x00000010

	#define	BLEND_COLORADD		1
	#define	BLEND_COLORMUL		0
	#define	BLEND_ALPHABLEND	2
	#define	BLEND_ALPHAADD		0
	#define	BLEND_ZWRITE		4
	#define	BLEND_NOZWRITE		0
	#define BLEND_SRCALPHA		8

	#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
	#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)

	struct SoraVertex {
		float			x, y;		// screen position    
		float			z;			// Z-buffer depth 0..1
		uint32			col;		// color
		float			tx, ty;		// texture coordinates
		
		SoraVertex(): z(0.0f), col(0xFFFFFFFF) {}
	};
	
	struct SoraQuad {
		SoraVertex		v[4];
		SoraTexture*	tex;
		int				blend;
        
        SoraQuad(): tex(NULL) {}
	};
	
	struct SoraTriple {
		SoraVertex		v[3];
		SoraTexture*	tex;
		int				blend;
        
        SoraTriple(): tex(NULL) {}
	};

	class SORA_API SoraSprite: public SoraShaderEnabledObject {
	public:
        SoraSprite(HSORATEXTURE tex);
        SoraSprite(HSORATEXTURE tex, float32 x, float32 y, float32 width, float32 height);
		virtual ~SoraSprite();

        virtual void render();
        virtual void render(float32 x, float32 y);
        virtual void render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4);
		virtual void renderWithVertices(SoraVertex* vertices, uint32 size, int32 mode);

		virtual uint32 update(float32 dt);

		void    setTexture(HSORATEXTURE tex);
		void    setTextureRect(float32 x, float32 y, float32 width, float32 height);
		hgeRect getTextureRect() const;
		
		void    setColor(uint32 c, int32 i=-1);
		uint32  getColor(int32 i=0) const;

		void    setZ(float32 z, int32 i=-1);
		float32 getZ(int32 i=0) const;

		void    setCenter(float32 x, float32 y);
		void    getCenter(float32& x, float32& y);
		float32 getCenterX() const;
		float32 getCenterY() const;

		void    setFlip(bool hflag, bool vflag, bool bFlipCenter=true);
		bool    getHFlip() const;
		bool    getVFlip() const;

		void    setBlendMode(int32 mode);
		int32   getBlendMode() const;

		int32   getTextureWidth(bool bOriginal=true) const;
		int32   getTextureHeight(bool bOriginal=true) const;
		int32   getSpriteWidth() const;
		int32   getSpriteHeight() const;
		int32   getSpritePosX() const;
		int32   getSpritePosY() const;

		void    setScale(float32 h, float32 v);
		float32 getVScale() const;
		float32 getHScale() const;

		void    setRotation(float32 r);
		float32 getRotation() const;
		void    setRotationZ(float32 rz);
		float32 getRotationZ() const;

		uint32*         getPixelData() const;
        void            unlockPixelData();
		HSORATEXTURE    getTexture() const;
		
		void addEffect(SoraImageEffect* effect);
		void stopEffect(SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
        
        SoraRect getBoundingBox() const;
		
	protected:
		SoraSprite();
<<<<<<< HEAD
        void _init(SoraTexture* tex, float32 x, float32 y, float32 w, float32 h);
=======
>>>>>>> b309d05f510b64aedc21204c44a68455c68d0b5c

		void _initDefaults();
		SoraTexture* mTexture;
		
		hgeRect mTextureRect;
        int32 mSprWidth, mSprHeight;

		float32 mRotation, mRotationZ;
		float32 mCenterX, mCenterY;
		float32 mVScale, mHScale;

		bool bVFlip, bHFlip, bCFlip;
		
		typedef std::list<SoraImageEffect*> ImageEffectList;
		ImageEffectList vEffects;

		SoraCore* mSora;
		SoraQuad mQuad;
<<<<<<< HEAD
	
=======

>>>>>>> b309d05f510b64aedc21204c44a68455c68d0b5c
	private:
		SoraSprite(SoraSprite&);
    };

} // namespace sora

#endif