/*
 *  SoraRenderSystem.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/1/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraRenderSystem.h"
#include "Rect4V.h"

namespace sora {

	void SoraRenderSystem::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, uint32 color, float32 z) {
		Rect4V rect;
		
		if(fWidth != y2-y1 && fWidth != x2-x1) {
			float rotAng = atan2f(y2-y1, x2-x1)-F_PI_4;
			
			rect.x1 = x1; rect.y1 = y1;
			rect.x2 = x1+fWidth*cosf(rotAng); rect.y2 = y1+fWidth*sinf(rotAng);
			rect.x4 = x2; rect.y4 = y2;
			rect.x3 = x2+fWidth*cosf(rotAng); rect.y3 = y2+fWidth*sinf(rotAng);
		} else {
			rect.x1 = x1; rect.y1 = y1;
			rect.x2 = x2; rect.y2 = y1;
			rect.x3 = x2; rect.y3 = y2;
			rect.x4 = x1; rect.y4 = y2;
		}
		sora::SoraQuad quad;
		
		quad.tex = NULL;
		
		quad.v[0].x   = rect.x1;
		quad.v[0].y   = rect.y1;
		quad.v[0].col = color;
		
		quad.v[1].x   = rect.x2;
		quad.v[1].y   = rect.y2;
		quad.v[1].col = color;
		
		quad.v[2].x   = rect.x3;
		quad.v[2].y   = rect.y3;
		quad.v[2].col = color;
		
		quad.v[3].x   = rect.x4;
		quad.v[3].y   = rect.y4;
		quad.v[3].col = color;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}
	
	void SoraRenderSystem::renderBox(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float32 z) {
		renderRect(x1, y1, x2, y1+1.f, 1.f, color, z);
		renderRect(x2, y1, x2+1.f, y2, 1.f, color, z);
		renderRect(x2, y2, x1, y2+1.f, 1.f, color, z);
		renderRect(x1, y2, x1+1.f, y1, 1.f, color, z);
	}

} // namespace sora