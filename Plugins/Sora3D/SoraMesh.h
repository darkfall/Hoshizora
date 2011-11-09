//
//  SoraMesh.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMesh_h
#define Sora_SoraMesh_h

#include "SoraPlatform.h"

#include "SoraVertex.h"
#include "util/SoraArray.h"

namespace sora {
    
    class SoraMesh {
    public:
        
        
    private:
        typedef SoraArray<SoraVertex> VertexList;
        VertexList mVertexList;
        
    };
    
} // namespace sora


#endif
