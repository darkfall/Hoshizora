//
//  SoraVertex.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraVertex_h
#define Sora_SoraVertex_h

#include "SoraPlatform.h"
#include "SoraTexture.h"

namespace sora {
    
	struct SORA_API SoraVertex {
		float			x, y;		// screen position    
		float			z;			// Z-buffer depth 0..1
		uint32			col;		// color
		float			tx, ty;		// texture coordinates
		
		SoraVertex(): z(0.0f), col(0xFFFFFFFF) {}
        SoraVertex(float _x, float _y, float _tx, float _ty): x(_x), y(_y), tx(_tx), ty(_ty) {}
	};
	
	struct SORA_API SoraQuad {
		SoraVertex		v[4];
		SoraTexture*	tex;
		int				blend;
        
        SoraQuad(): tex(NULL) {}
	};
	
	struct SORA_API SoraTriple {
		SoraVertex		v[3];
		SoraTexture*	tex;
		int				blend;
        
        SoraTriple(): tex(NULL) {}
	};
    
}


#endif
