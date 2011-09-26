#ifndef SORA_SPRITE_H_
#define SORA_SPRITE_H_

#include "SoraTexture.h"
#include "SoraColor.h"
#include "SoraMath.h"
#include "SoraImageEffect.h"
#include "SoraObject.h"
#include "SoraShaderEnabled.h"

#include <list>

namespace sora {

	class SoraCore;
	
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
        SoraVertex(float _x, float _y, float _tx, float _ty): x(_x), y(_y), tx(_tx), ty(_ty) {}
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
        SoraSprite();
        SoraSprite(SoraTextureHandle tex);
        SoraSprite(SoraTextureHandle tex, float x, float y, float width, float height);
		virtual ~SoraSprite();

        virtual void render();
        virtual void render(float x, float y);
        virtual void renderInBox(float x1, float y1, float x2, float y2);
        virtual void render4V(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
		virtual void renderWithVertices(SoraVertex* vertices, uint32 size, int32 mode);

		virtual int32 update(float dt);

		void    setTexture(SoraTextureHandle tex);
		void    setTextureRect(float x, float y, float width, float height);
		SoraRect getTextureRect() const;
		
		void    setColor(uint32 c, int32 i=-1);
		uint32  getColor(int32 i=0) const;

		void    setZ(float z, int32 i=-1);
		float   getZ(int32 i=0) const;
        
        void    setPosition(float x, float y);

		void    setCenter(float x, float y);
		void    getCenter(float& x, float& y);
		float   getCenterX() const;
		float   getCenterY() const;

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

		void    setScale(float h, float v);
		float   getVScale() const;
		float   getHScale() const;

		void    setRotation(float r);
		float   getRotation() const;
		
		uint32*         getPixelData() const;
        void            unlockPixelData();
		SoraTextureHandle    getTexture() const;
		
		void addEffect(SoraImageEffect* effect);
		void stopEffect(SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
        
        void fadeTo(float to, float t);
        void rotateTo(float to, float t);
        void scaleTo(float h, float v, float t);
        
        typedef SoraFunction<void(SoraSprite*)> NotificationFunc;
        void fadeToAndNotify(float to, float t, const NotificationFunc& func);
        void rotateToAndNotify(float to, float t, const NotificationFunc& func);
        void scaleToAndNotify(float h, float v, float t, const NotificationFunc& func);
        
        SoraRect getBoundingBox() const;
        
        static SoraSprite* LoadFromFile(const StringType& file);
        static SoraSprite* LoadFromRawData(uint32* data, int32 w, int32 h);
        static SoraSprite* LoadFromMemory(uint32* data, ulong32 size);
        static void Render(const StringType& file, float x, float y, float r=0.f, float sh=1.f, float sv=1.f);
		
	protected:        
        inline void _initDefaults();
        inline void _init(SoraTexture* tex, float x, float y, float w, float h);

        inline void buildQuad(float x, float y);
		
		SoraRect mTextureRect;
        int32   mSprWidth, mSprHeight;

		float mRotation;
		float mCenterX, mCenterY;
		float mVScale, mHScale;

		bool bVFlip, bHFlip, bCFlip;
        bool bPropChanged;
		
		typedef std::list<SoraImageEffect*> ImageEffectList;
		ImageEffectList vEffects;

		SoraQuad mQuad;

        static SoraCore* mSora;
        
	private:
		SoraSprite(SoraSprite&);
    };

} // namespace sora

#endif