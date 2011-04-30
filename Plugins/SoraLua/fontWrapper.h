/*
 *  fontWrapper.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef FONT_WRAPPER_H_
#define FONT_WRAPPER_H_

#include "SoraPlatform.h"
#include "SoraCore.h"
#include "SoraFont.h"

namespace fontWrapper {
	
	static ulong32 createFont(const std::wstring& fontName, int32 size) {
		return (ulong32)sora::SORA->createFont(fontName, size);
	}
	
	static void renderString(ulong32 font, float32 x, float32 y, int32 alignment, const std::wstring& str) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->render(x, y, alignment, str.c_str());
		}
	}
	
	static void setKerningWidth(ulong32 font, float32 kw) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setKerningWidth(kw);
		}
	}
	
	static void setKerningHeight(ulong32 font, float32 kh) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setKerningHeight(kh);
		}
	}
	
	static float32 getKerningWidth(ulong32 font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getKerningWidth();
		}
		return 0.f;
	}
	
	static float32 getKerningHeight(ulong32 font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getKerningHeight();
		}
		return 0.f;
	}
	
	static int32 getFontSize(ulong32 font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getFontSize();
		}
		return 0;
	}
	
	static void setColor(ulong32 font, ulong32 color, int32 i) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setColor(color, i);
		}
	}

	static ulong32 getColor(ulong32 font, int32 i) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getColor(i);
		}
		return 0;
	}
}

#endif