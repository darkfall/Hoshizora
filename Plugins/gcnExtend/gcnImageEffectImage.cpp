/*
 *  gcnImageEffectImage.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnImageEffectImage.h"
#include "SoraCore.h"

namespace gcn {
	
	ImageEffectImage::~ImageEffectImage() {
		clearEffects();
	}

	void ImageEffectImage::addEffect(sora::SoraImageEffect* effect) {
		imageEffects.push_back(effect);
	}
	
	void ImageEffectImage::stopEffect(sora::SoraImageEffect* _eff) {
		IMAGE_EFFECT_LIST::iterator eff = imageEffects.begin();
		while(eff != imageEffects.end()) {
			if((*eff) == _eff) {
				delete _eff;
				_eff = 0;
				eff = imageEffects.erase(eff);
				return;
			} else
				++eff;
		}
	}
	
	void ImageEffectImage::clearEffects() {
		IMAGE_EFFECT_LIST::iterator eff = imageEffects.begin();
		while(eff != imageEffects.end()) {
			delete (*eff);
			(*eff) = 0;
			++eff;
		}
		imageEffects.clear();
	}
	
	void ImageEffectImage::logic() {
		float32 dt = sora::SORA->getDelta();
		if(!imageEffects.empty()) {
			IMAGE_EFFECT_LIST::iterator eff = imageEffects.begin();
			while(eff != imageEffects.end()) {
				uint32 result = (*eff)->update(dt);
				switch((*eff)->getType()) {
					case sora::IMAGE_EFFECT_FADEIN:
					case sora::IMAGE_EFFECT_FADEOUT: {
						Color col = getColor();
						col.a = (*eff)->get1st();
						setColor(col);
						break;
					}
						
					case sora::IMAGE_EFFECT_COLOR_TRANSITION: {
						ulong32 hardwareColor = sora::SoraColorRGBA::GetHWColor((*eff)->get1st(), (*eff)->get2nd(), (*eff)->get3rd(), (*eff)->get4th());
						setColor(gcn::Color(CGETR(hardwareColor),
											CGETG(hardwareColor),
											CGETB(hardwareColor),
											CGETA(hardwareColor)));	
					}
					break;
						
					case sora::IMAGE_EFFECT_ROTATE:
					case sora::IMAGE_EFFECT_ROTATE_Z:
						setRotation((*eff)->get1st());
						break;
				}
				
				if(result == sora::IMAGE_EFFECT_END) {
					delete (*eff);
					(*eff) = 0;
					eff = imageEffects.erase(eff);
				} else
					++eff;
			}
		}
	}
	
	
} // namespace gcn