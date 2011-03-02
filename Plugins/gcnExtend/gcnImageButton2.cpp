/*
 *  gcnImageButton2.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "gcnImageButton2.h"

namespace gcn {
	
	ImageButton2::ImageButton2() {
		mImage = 0;
		
		mInternalImage = false;
		
		setFrameSize(0);
	}
	
	ImageButton2::~ImageButton2() {
		if(mInternalImage) {
			if(mImage) {
				delete mImage;
				mImage = 0;
			}
		}
	}
	
	ImageButton2::ImageButton2(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed) {
		mImage = Image::load(sprNormal);
		
		normalTex = normal;
		pushedTex = pushed;
		//unavailTex = unavail;
		
		setWidth(normalTex.width * 1.5);
		setHeight(normalTex.height * 1.5);
		
		setFrameSize(0);
	}
	
	void ImageButton2::setImage(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed) {
		if(mInternalImage && mImage)
			delete mImage;
		mImage = Image::load(sprNormal);
		
		normalTex = normal;
		pushedTex = pushed;
		//unavailTex = unavail;
		
		setWidth(normalTex.width * 1.5);
		setHeight(normalTex.height * 1.5);
	}

	void ImageButton2::draw(Graphics* graphics) {
        const int textX = (getWidth() - (mImage ? normalTex.width: 0) ) / 2;
        const int textY = (getHeight() - (mImage ? normalTex.height : 0) ) / 2;

        if (isPressed()) {
            if(mImage) {
                graphics->drawImage(mImage, pushedTex.x, pushedTex.y, textX, textY, pushedTex.width, pushedTex.height);
			}
        }
        else {
			if(mImage) {
				graphics->drawImage(mImage, normalTex.x, normalTex.y, textX, textY, normalTex.width, normalTex.height);
			}
        }
		
	}
	
} // namespace gcn