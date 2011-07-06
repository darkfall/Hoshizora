/*
 *  SoraSpriteFont.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/11/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraSpriteFont.h"

#include "../SoraCore.h"

namespace sora {

	SoraSpriteFont::SoraSpriteFont(const SoraWString& font, const SoraString& sequance, int32 width, int32 height) :
	mFontSprite(NULL),
	mKerningWidth(0.f), mKerningHeight(0.f),
	mRotation(0.f), mScale(0.f),
	mWidth(0), mHeight(0) {
		mFontSprite = SORA->createSprite(font);
		if(mFontSprite) {
			mWidth = width;
			mHeight = height;
			buildPosMap(sequance);
			memset(mCharPos, 0, sizeof(int32)*128);
		} else
			DebugPtr->log("Error creating font "+ws2s(font));
	}
	
	SoraSpriteFont::~SoraSpriteFont() {
		if(mFontSprite) {
			delete mFontSprite;
			mFontSprite = NULL;
		}
	}
	
	void SoraSpriteFont::buildPosMap(const SoraString& seq) {
		for(size_t i=0; i<seq.size(); ++i) {
			mCharPos[(int32)seq[i]] = mWidth*i+1;
		}
	}

	void SoraSpriteFont::render(float32 x, float32 y, const wchar_t* text, bool hcenter , bool vcenter) {
		if(!mFontSprite)
			return;
		
		if(hcenter)
			x -= static_cast<int32>(getStringWidth(text)) >> 1;
		if(vcenter)
			y -= mHeight >> 1;
		
		const wchar_t* p = text;
		while(*p) {
			if(*p == L'\n')
				y += mHeight + mKerningHeight;
			
			int32 index = static_cast<int32>(*p);
			if(index >= 0 && index < 128) {
				if(mCharPos[index]) {
					mFontSprite->setTextureRect(mCharPos[index], 0, mWidth, mHeight);
					mFontSprite->setRotation(mRotation);
					mFontSprite->setScale(mScale, mScale);
				
					mFontSprite->render(x, y);
				}
			}
			
			p++;
			x += mWidth + mKerningWidth;
		}
	}
	
	void SoraSpriteFont::print(float32 x, float32 y, int32 align, const wchar_t *format, ...) {
		va_list	ArgPtr;
#ifdef HAS_WSTRING
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		render(x, y, align, Message);

#else
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, ws2sfast(format).c_str(), ArgPtr);
		va_end(ArgPtr);
		render(x, y, align,  s2wsfast(Message).c_str());
#endif
		
	}
	
	void SoraSpriteFont::render(float32 x, float32 y, int32 align, const wchar_t* text) {
		if(align == FONT_ALIGNMENT_RIGHT)
			x -= getStringWidth(text);
		else if(align == FONT_ALIGNMENT_CENTER)
			x -= static_cast<int32>(getStringWidth(text)) >> 1;
		
		render(x, y, text, false, false);
	}
	
	void SoraSpriteFont::setColor(ulong32 dwColor, int32 i) {
		if(mFontSprite)
			mFontSprite->setColor(dwColor, i);
	}
	
	ulong32	SoraSpriteFont::getColor(int32 i) {
		if(mFontSprite)
			return mFontSprite->getColor(i);
		return 0;
	}
	
	void SoraSpriteFont::setKerningWidth(float32 kerning) {
		mKerningWidth = kerning;
	}
	
	void SoraSpriteFont::setKerningHeight(float32 kerning) {
		mKerningHeight = kerning;
	}
	
	float32	SoraSpriteFont::getKerningWidth() {
		return mKerningWidth;
	}
	
	float32	SoraSpriteFont::getKerningHeight() {
		return mKerningHeight;
	}
	
	float32	SoraSpriteFont::getStringWidth(const wchar_t* text) {
		const wchar_t* p = text;
		
		float32 w = 0.f;
		while(*p) {
			w += mWidth;
		}
		return w;
	}
	
	float32 SoraSpriteFont::getStringHeight(const wchar_t* text) {
		const wchar_t* p = text;
		
		float32 h = static_cast<float32>(mHeight);
		while(*p)
			if(*p == L'\n')
				h += mHeight;
		return h;
	}
	
	float32	SoraSpriteFont::getHeight() {
		return static_cast<float32>(mHeight);
	}
	
	uint32 SoraSpriteFont::getFontSize() {
		return static_cast<float32>(mWidth);
	}
	
	float32	SoraSpriteFont::getWidthFromCharacter(wchar_t c, bool original) {
		return static_cast<float32> (mWidth);
	}
	
	void SoraSpriteFont::setCharRotation(float32 rot) {
		mRotation = rot;
	}
	
	void SoraSpriteFont::setScale(float32 scale) {
		mScale = scale;
	}
	
} // namespace sora