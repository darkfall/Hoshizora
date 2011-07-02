/*
 *  SoraiOSFontManager.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#include "SoraiOSFontManager.h"
#include "SoraiOSFont.h"

namespace sora {
	
	SoraiOSFontManager::~SoraiOSFontManager() {
		FONT_MAP::iterator itFont = mFontMap.begin();
		while(itFont != mFontMap.end()) {
			delete itFont->second;
			++itFont;
		}
		mFontMap.clear();
	}
	
	SoraFont* SoraiOSFontManager::getFont(const char* filename, uint32 size, uint32 bsize, const char* pr) {
		if(!filename) return NULL;
		
		FONT_MAP::iterator itFont = mFontMap.find(filename);
		if(itFont != mFontMap.end() && itFont->second->getFontSize() == size) 
			return itFont->second;
		
		SoraiOSFont* pfont = new SoraiOSFont(filename, size);
		mFontMap[filename] = pfont;
		return pfont;
	}
    
    bool SoraiOSFontManager::fontExists(const char* filename) {
        FONT_MAP::iterator itFont = mFontMap.find(filename);
        if(itFont != mFontMap.end())
            return true;
        return false;
    }
    
    void SoraiOSFontManager::releaseFont(SoraFont* font) {
        FONT_MAP::iterator itFont = mFontMap.begin();
        while(itFont != mFontMap.end()) {
            if(itFont->second == font) {
                delete font;
                itFont->second = NULL;
                mFontMap.erase(itFont);
                break;
            }
        }
    }

} // namespace sora