#ifndef SORA_SPRITE_H_
#define SORA_SPRITE_H_

#include "SoraTexture.h"
#include "SoraColor.h"

#include "hgerect.h"
#include "SoraImageEffect.h"
#include "SoraObject.h"
#include "SoraShader.h"
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

	#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
	#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)

	struct SoraVertex {
		float			x, y;		// screen position    
		float			z;			// Z-buffer depth 0..1
		ulong32			col;		// color
		float			tx, ty;		// texture coordinates
	};
	
	struct SoraQuad {
		SoraVertex		v[4];
		SoraTexture*	tex;
		int				blend;
	};
	
	struct SoraTriple {
		SoraVertex		v[3];
		SoraTexture*	tex;
		int				blend;
	};

	class SoraSprite: public SoraObject {
	public:
        SoraSprite(HSORATEXTURE tex);
        SoraSprite(HSORATEXTURE tex, float32 x, float32 y, float32 width, float32 height);
		SoraSprite(SoraTexture* tex);
		SoraSprite(SoraTexture* tex, float32 x, float32 y, float32 width, float32 height);
		virtual ~SoraSprite();

        void render();
        void render(float32 x, float32 y);
        void render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4);

		void setTexture(SoraTexture* tex);
		
		void setTextureRect(float32 x, float32 y, float32 width, float32 height);
		const hgeRect& getTextureRect() const;
		
		void setColor(ulong32 c, int32 i=-1);
		ulong32 getColor(int32 i=0) const;

		void setZ(float32 z, int32 i=-1);
		float32 getZ(int32 i=0) const;

		void setCenter(float32 x, float32 y);
		void getCenter(float32& x, float32& y);
		float32 getCenterX() const;
		float32 getCenterY() const;

		void setFlip(bool hflag, bool vflag);
		bool getHFlip() const;
		bool getVFlip() const;

		void setBlendMode(int32 mode);
		int32  getBlendMode() const;

		int32 getTextureWidth() const;
		int32 getTextureHeight() const;
		int32 getSpriteWidth() const;
		int32 getSpriteHeight() const;
		int32 getSpritePosX() const;
		int32 getSpritePosY() const;

		void setScale(float32 h, float32 v);
		float32 getVScale() const;
		float32 getHScale() const;

		void setRotation(float32 r);
		float32 getRotation() const;
		void setRotationZ(float32 rz);
		float32 getRotationZ() const;

		ulong32* getPixelData() const;
        void unlockPixelData();
		SoraTexture* getTexture() const { return texture; }
		
		uint32 update(float32 dt);
		void addEffect(SoraImageEffect* effect);
		void stopEffect(SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
        
        void attachShader(SoraShader*);
        void detachShader(SoraShader*);
        SoraShader* attachShader(const SoraWString& shaderPath, const SoraString& entry, SORA_SHADER_TYPE type);
        bool hasShader() const;
        void clearShader();
        
	private:
        void _init(SoraTexture* tex, float32 x, float32 y, float32 w, float32 h);
        
		SoraSprite() { texture=0; _initDefaults(); }
		SoraSprite(SoraSprite&) { }

		void _initDefaults();
		SoraTexture* texture;
		
		hgeRect textureRect;

		float32 rot, rotZ;
		float32 centerX, centerY;
		float32 vscale, hscale;

		bool bVFlip, bHFlip;
		
		typedef std::list<SoraImageEffect*> IMAGE_EFFECT_LIST;
		IMAGE_EFFECT_LIST vEffects;

		SoraCore* sora;
		SoraQuad quad;
        
        SoraShaderContext* shaderContext;
	};

} // namespace sora

#endif