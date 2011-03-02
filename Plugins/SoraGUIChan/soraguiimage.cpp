#include "soraguiimage.hpp"

#include "guichan/exception.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{

    SoraGUIImage::SoraGUIImage(sora::HSORATEXTURE texture)
        : mTexture(texture)
    {
        sora = sora::SoraCore::Instance();
    }

    SoraGUIImage::~SoraGUIImage()
    {
        free();
	}

    void SoraGUIImage::free()
    {
        if (mSoraSprite != NULL)
        {
            delete mSoraSprite;
            mSoraSprite =  NULL;
        }
	}

    sora::SoraSprite *SoraGUIImage::getSprite() const
    {
        return mSoraSprite;
    }

    int SoraGUIImage::getWidth() const
    {
		if(mSoraSprite)
			return mSoraSprite->getTextureWidth();
		return 0;
    }

    int SoraGUIImage::getHeight() const
    {
		if(mSoraSprite)
			return mSoraSprite->getTextureHeight();
		return 0;
    }

    Color SoraGUIImage::getPixel(int x, int y)
    {
        /*DWORD *pLockPtr = mSoraSprite->getPixelData();

        if (pLockPtr == NULL)
        {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        DWORD color = pLockPtr[x + y * getWidth()];

        mSoraSprite->unlockPixelData();
        return Color(CGETR(color),
                     CGETG(color), 
                     CGETB(color), 
                     CGETA(color ));*/
		return Color();
    }

    void  SoraGUIImage::putPixel(int x, int y, const Color& color)
    {
        //DWORD hardwareColor = CARGB(color.a, color.r, color.g, color.b);

        /*DWORD *pLockPtr = mSoraSprite->getPixelData();

        if (pLockPtr == NULL)
        {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        pLockPtr[x + y * getWidth()] = hardwareColor;

        mSoraSprite->unlockPixelData();*/
    }
	
	void SoraGUIImage::setTextureRect(const Rectangle& rect) {
		if(mSoraSprite)
			mSoraSprite->setTextureRect(rect.x, rect.y, rect.width, rect.height);
	}

    void SoraGUIImage::convertToDisplayFormat()
    {
        /*DWORD *pLockPtr = sora->textureLock(mTexture);

        if (pLockPtr == NULL)
        {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        int i;
        int end = ((sora::SoraTexture*)mTexture)->mOriginalWidth * ((sora::SoraTexture*)mTexture)->mOriginalHeight;

        for (i = 0; i < end; i++)
        {
            DWORD color = pLockPtr[i];
            if (CGETR(color) == 0xFF
                && CGETG(color) == 0x00
                && CGETB(color) == 0xFF)
            {
                pLockPtr[i] = CARGB(0x00, 0x00, 0x00, 0x00);
            }
        }

        sora->textureUnlock(mTexture);
		 */
        mSoraSprite = (sora::SoraSprite*)sora->createSpriteTex(mTexture);
		mSoraSprite->setZ(GUI_Z);
		mSoraSprite->setBlendMode(BLEND_DEFAULT_Z);
    }
}
