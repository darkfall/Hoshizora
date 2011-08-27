//
//  style.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/10/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "guichan/style.hpp"

namespace gcn {
    
    std::string StyleStringContainer::getStyleString(const std::string& key) const {
        StyleStringMap::const_iterator it = mStyleStrings.find(key);
        if(it != mStyleStrings.end())
            return it->second;
        return std::string();
    }
    
    void StyleStringContainer::setStyleString(const std::string& key, const std::string& val) {
        StyleStringMap::iterator it = mStyleStrings.find(key);
        if(it != mStyleStrings.end())
            it->second = val;
        else
            mStyleStrings.insert(std::make_pair(key, val));
    }
    
    Style* Style::DefaultStyle() {
        static Style global_style;
        return &global_style;
    }
    
    Style::Style():
    mGlobalBackgroundColor(0xffffff),
    mGlobalForegroundColor(0x000000),
    mGlobalBaseColor(0x808090),
    mGlobalSelectionColor(0xc3d9ff) {
        
    }
    
	void Style::setBackgroundColor(const Color& col) {
		mGlobalBackgroundColor = col;
	}
	
	void Style::setForegroundColor(const Color& col) {
		mGlobalForegroundColor = col;
	}
	
	void Style::setBaseColor(const Color& col) {
		mGlobalBaseColor = col;
	}
	
	void Style::setSelectionColor(const Color& col) {
		mGlobalSelectionColor = col;
	}
	
	Color Style::getBackgroundColor() {
		return mGlobalBackgroundColor;
	}
	
	Color Style::getForegroundColor() {
		return mGlobalForegroundColor;
	}
	
	Color Style::getBaseColor() {
		return mGlobalBaseColor;
	}
	
	Color Style::getSelectionColor() {
		return mGlobalSelectionColor;
	}
    
    std::string Style::getStyleString(const std::string& key) const {
        return mStyleStrings.getStyleString(key);
    }
    
    void Style::setStyleString(const std::string& key, const std::string& val) {
        mStyleStrings.setStyleString(key, val);
    }
    
    Color Style::getStyleColor(const std::string& col) const {
        StyleColorMap::const_iterator it = mStyleColors.find(col);
        if(it != mStyleColors.end())
            return it->second;
        return gcn::Color();
    }
    
    void Style::setStyleColor(const std::string& key, const gcn::Color& col) {
        mStyleColors[key] = col;
    }
    
} // namespace gcn
