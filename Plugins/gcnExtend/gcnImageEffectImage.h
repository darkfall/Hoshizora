/*
 *  gcnImageEffectAdapter.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_IMAGE_EFFECT_ADAPTER_H_
#define GCN_IMAGE_EFFECT_ADAPTER_H_

#include "SoraGUIChan/guichan.hpp"
#include "SoraImageEffect.h"

namespace gcn {
	
	/*
	 a class for applying SoraImageEffect to gcn image
	 you can use this to create widget animation
	 */
	class ImageEffectImage: public Image {
	public:
		ImageEffectImage() {}
		~ImageEffectImage();
		
		virtual void logic();
		
		void addEffect(sora::SoraImageEffect* effect);
		void stopEffect(sora::SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
	
	private:
		typedef std::list<sora::SoraImageEffect*> IMAGE_EFFECT_LIST;
		IMAGE_EFFECT_LIST imageEffects;
	};
}


#endif