//
//  ComponentIds.h
//  Sora
//
//  Created by Robert Bu on 8/27/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_ComponentIds_h
#define Sora_ComponentIds_h

namespace sora {
    
    namespace component {
        
        // here defines the ids for components
        
#define DEF_COP_ID(name, id) \
    static std::string name = id;
        
        DEF_COP_ID(CID_SpriteRenderer, "SpriteRenderer")
        DEF_COP_ID(CID_Position, "Position")
        DEF_COP_ID(CID_Movement, "Movement")
        
    } // namespace component
    
} // namespace sora 


#endif
