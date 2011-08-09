//
//  style.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/10/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "guichan/style.h"

namespace gcn {
    
	Color Style::mGlobalBackgroundColor(0xffffff);
	Color Style::mGlobalForegroundColor(0x000000);
	Color Style::mGlobalBaseColor(0x808090);
	Color Style::mGlobalSelectionColor(0xc3d9ff);
    
	void Style::setGlobalBackgroundColor(const Color& col) {
		mGlobalBackgroundColor = col;
	}
	
	void Style::setGlobalForegroundColor(const Color& col) {
		mGlobalForegroundColor = col;
	}
	
	void Style::setGlobalBaseColor(const Color& col) {
		mGlobalBaseColor = col;
	}
	
	void Style::setGlobalSelectionColor(const Color& col) {
		mGlobalSelectionColor = col;
	}
	
	Color Style::getGlobalBackgroundColor() {
		return mGlobalBackgroundColor;
	}
	
	Color Style::getGlobalForegroundColor() {
		return mGlobalForegroundColor;
	}
	
	Color Style::getGlobalBaseColor() {
		return mGlobalBaseColor;
	}
	
	Color Style::getGlobalSelectionColor() {
		return mGlobalSelectionColor;
	}
    
    
} // namespace gcn
