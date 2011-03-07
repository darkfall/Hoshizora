/*
 *  gcnImageButton2.h
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef GCN_IMAGE_BUTTON_2_
#define GCN_ImAGE_BUTTON_2_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	class ImageButton2: public Button {
	public:
		ImageButton2();
		virtual ~ImageButton2();
		
		ImageButton2(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed);
		
		void setImage(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed);
        void draw(Graphics* graphics);
		
	private:
		Image* mImage;
		
		Rectangle normalTex;
		Rectangle pushedTex;
		Rectangle unavailTex;
		
		bool mInternalImage;
	};
} // namespace gcn

#endif