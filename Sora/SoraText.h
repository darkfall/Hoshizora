//
//  SoraText.h
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraText_h
#define Sora_SoraText_h

#include "SoraPlatform.h"
#include "SoraFont.h"
#include "SoraObject.h"
#include "SoraColor.h"
#include "SoraMath.h"

namespace sora {
    
    /**
     * A simple wrapper class for rendering texts
     * Can also render to a sprite and release itself
     **/
    
    class SoraSprite;
    
    class SORA_API SoraText: public SoraObject {
    public:
        SoraText();
        SoraText(const SoraWString& str, SoraFont* font);
        virtual ~SoraText();
        
        void set(const SoraWString& str, SoraFont* font);
        
        void setText(const SoraWString& str);
        void setFont(SoraFont* font);
        void setStyle(int32 style);
        void setColor(const SoraColorRGBA& col);
        void setRotation(float rotation);
        void setCharRotation(float crotation);

        int32 getStyle() const;
        SoraFont* getFont() const;
        const SoraWString& getText() const;
        const SoraColorRGBA& getColor() const;
        float getRotation() const;
        float getCharRotation() const;
        
        void render();
        void renderTo(float x, float y);
        
        SoraRect getBoundingBox() const;
        
        /**
         * Render to a sprite(if RenderTarget is allowed in the RenderSystem)
         * Not calling this during the normal frame rendering is suggested
         * Suggested for preinitiazing static text(initializing static font texts into sprites etc)
         * Notice the sprite would be released when the text is released(because the release of RenderTarget texture)
         * And after this call, the rendering of the text will change to static sprite rendering and set* function will not work
         * Though some functions would continue to work suchas fadeto etc
         **/
        SoraSprite* renderToSprite();
        
    private:
        void release();
        
        SoraFont* mFont;
        uint32 mFontSize;
        int32 mStyle;
        SoraColorRGBA mColor;
        float mRotation;
        float mCharRotation;
        SoraWString mText;
        
        SoraTargetHandle mTarget;
        SoraSprite* mTextSprite;
    };
    
} // namespace sora



#endif
