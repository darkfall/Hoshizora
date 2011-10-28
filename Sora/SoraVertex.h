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

#include "SoraMatrix4.h"

namespace sora {
    
	struct SORA_API SoraVertex {
		float			x, y;		// screen position    
		float			z;			// Z-buffer depth 0..1
		uint32			col;		// color
		float			tx, ty;		// texture coordinates
		
		SoraVertex(): z(0.0f), col(0xFFFFFFFF) {}
        SoraVertex(float _x, float _y, float _tx, float _ty): x(_x), y(_y), tx(_tx), ty(_ty) {}
        
        SoraVertex operator*(const SoraMatrix4& rhs) const {
            SoraVector4 vec = rhs * SoraVector4(x, y, z, 1.0);
            
            SoraVertex tmp = *this;
            tmp.x = vec.x;
            tmp.y = vec.y;
            tmp.z = vec.z;
            return tmp;
        }
        SoraVertex& operator*=(const SoraMatrix4& rhs) {
            SoraVector4 vec = rhs * SoraVector4(x, y, z, 1.0);
            x = vec.x;
            y = vec.y;
            z = vec.z;
            
            return *this;
        }
	};
	
	struct SORA_API SoraQuad {
		SoraVertex		v[4];
		SoraTexture*	tex;
		int				blend;
        
        SoraQuad(): tex(NULL) {}
        
        SoraQuad operator*(const SoraMatrix4& rhs) const {
            SoraQuad tmp = *this;
            for(int i=0; i<4; ++i)
                tmp.v[i] *= rhs;
            return tmp;
        }
        SoraQuad& operator*=(const SoraMatrix4& rhs) {
            for(int i=0; i<4; ++i)
                v[i] *= rhs;
            return *this;
        }
	};
	
	struct SORA_API SoraTriple {
		SoraVertex		v[3];
		SoraTexture*	tex;
		int				blend;
        
        SoraTriple(): tex(NULL) {}
	};
    
}


#endif
