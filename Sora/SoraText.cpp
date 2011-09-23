//
//  SoraText.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraText.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "SoraFont.h"

namespace sora {
    
    SoraText::SoraText():
    mFont(0),
    mFontSize(20),
    mStyle(FONT_ALIGNMENT_LEFT),
    mColor(SoraColorRGBA()),
    mRotation(0.f),
    mCharRotation(0.f),
    mText(StringType()),
    mTarget(0),
    mTextSprite(0) {
        mFont = SoraCore::Instance()->getSystemFont();
        if(mFont)
            mFontSize = mFont->getFontSize();
    }
    
    SoraText::SoraText(const SoraWString& str, SoraFont* font):
    mFont(font),
    mFontSize(font->getFontSize()),
    mStyle(FONT_ALIGNMENT_LEFT),
    mColor(SoraColorRGBA()),
    mRotation(0.f),
    mCharRotation(0.f),
    mText(str),
    mTarget(0),
    mTextSprite(0) {
        sora_assert(font);
    }
    
    SoraText::~SoraText() {
        release();
    }
    
    void SoraText::set(const SoraWString& str, SoraFont* font) {
        mText = str;
        mFont = font;
    }
    
    void SoraText::setText(const SoraWString& str) {
        mText = str;
    }
    
    void SoraText::setFont(SoraFont* font) {
        mFont = font;
    }
    
    void SoraText::setStyle(int32 style) {
        mStyle = style;
    }
    
    void SoraText::setColor(const SoraColorRGBA& col) {
        mColor = col;
    }
    
    void SoraText::setRotation(float rotation) {
        mRotation = rotation;
    }
    
    void SoraText::setCharRotation(float rotation) {
        mCharRotation = rotation;
    }
    
    float SoraText::getRotation() const {
        return mRotation;
    }
    
    float SoraText::getCharRotation() const {
        return mCharRotation;
    }
    
    const SoraColorRGBA& SoraText::getColor() const {
        return mColor;
    }
    
    int32 SoraText::getStyle() const {
        return mStyle;
    }
    
    const SoraWString& SoraText::getText() const {
        return mText;
    }
    
    SoraFont* SoraText::getFont() const {
        return mFont;
    }
    
    void SoraText::render() {
        if(mTarget && mTextSprite) {
            mTextSprite->render(getPositionX(), getPositionY());
        }
        else if(mFont) {
            mFont->setCharRotation(mCharRotation);
            mFont->setLineRotation(mRotation);
            mFont->setColor(mColor.GetHWColor());
            mFont->print(getPositionX(), getPositionY(), mStyle, mText.c_str());
        }
    }
    
    void SoraText::renderTo(float x, float y) {
        if(mTarget && mTextSprite) {
            mTextSprite->render(x, y);
        }
        else if(mFont) {
            mFont->setCharRotation(mCharRotation);
            mFont->setLineRotation(mRotation);
            mFont->setColor(mColor.GetHWColor());
            mFont->print(x, y, mStyle, mText.c_str());
        }
    }
    
    void SoraText::release() {
        if(mTarget) {
            SoraCore::Instance()->freeTarget(mTarget);
        }
        if(mTextSprite) {
            delete mTextSprite;
        }
    }
    
    SoraSprite* SoraText::renderToSprite() {
        if(mTarget != 0 || mTextSprite != 0) {
            release();
        }
        
        if(mFont && !mText.empty()) {
            mTarget = SoraCore::Instance()->createTarget(mFont->getStringWidth(mText.c_str()), mFont->getHeight());
            if(mTarget) {
                // render the text
                SoraCore::Instance()->beginScene(0, mTarget);
                
                render();
                
                SoraCore::Instance()->endScene();
                
                mTextSprite = new SoraSprite(SoraCore::Instance()->getTargetTexture(mTarget));
                
                return mTextSprite;
            }
        }
        return 0;
    }
    
    SoraRect SoraText::getBoundingBox() const {
        if(mTarget != 0 && mTextSprite != 0) {
            return SoraRect(getPositionX(),
                            getPositionY(),
                            getPositionX()+mTextSprite->getSpriteWidth(),
                            getPositionY()+mTextSprite->getSpriteHeight());
        } else if(mFont) {
            return SoraRect(getPositionX(), 
                            getPositionY(),
                            mFont->getStringWidth(mText.c_str()),
                            mFont->getHeight());
        }
        return SoraRect();
                      
    }
    
    void SoraText::Render(const SoraWString& str, float x, float y) {
        SoraFont* font = SoraCore::Instance()->getSystemFont();
        if(font != 0) {
            font->render(x, y, str.c_str());
        }
    }
    
} // namespace sora