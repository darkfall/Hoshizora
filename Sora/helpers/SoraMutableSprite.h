/*
 *  SoraMutableSprite.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MUTABLE_SPRITE_H_
#define SORA_MUTABLE_SPRITE_H_

#include "../SoraSprite.h"

namespace sora {
	
	class SoraMutableSprite: public SoraSprite {
	public:
		SoraMutableSprite(HSORATEXTURE tex);
		
		uint32 getPixel(int32 x, int32 y);
		void   putPixel(int32 x, int32 y, uint32 col);
		
		void putPixelArray(int32 xstart, int32 ystart, int32 width, int32 height, uint32* col);
		
		virtual void render();
		virtual void render(float32 x, float32 y);
		virtual void render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4);
		virtual void renderWithVertices(SoraVertex* vertices, uint32 size, int32 mode);
		
	private:
		inline void applyChange();
		
		uint32* mPixelData;
		bool mChanged;
	};
	
} // namespace sora


#endif // SORA_MUTABLE_SPRITE_H_