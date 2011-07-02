/*
 *  SoraiOSFontManager.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_IOS_FONT_MANAGER_H_
#define SORA_IOS_FONT_MANAGER_H_

#include "SoraFontManager.h"
#include <map>

namespace sora {
	
	class SoraiOSFontManager: public SoraFontManager {
	public:
		SoraiOSFontManager() {}
		~SoraiOSFontManager();
		
		SoraFont* getFont(const char* filename, uint32 size, uint32 bsize=0, const char* pr=0);
		
        bool fontExists(const char* filename);
		
        void releaseFont(SoraFont* font);
        
	private:
		typedef std::map<SoraString, SoraFont*> FONT_MAP;
		FONT_MAP mFontMap;
	};
	
} // namespace sora


#endif