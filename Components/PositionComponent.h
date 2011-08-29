//
//  PositionComponent.h
//  Sora
//
//  Created by Robert Bu on 8/27/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_PositionComponent_h
#define Sora_PositionComponent_h

#include "SoraMovable.h"
#include "entity/SoraComponent.h"

namespace sora {
    
    namespace component {
        
        class PositionComponent: public SoraComponent {
        public:            
            PositionComponent(float x=0.f, float y=0.f, float z=0.f):
            SoraComponent(getName(), true) {
                addProperty("x", x);
                addProperty("y", y);
                addProperty("z", z);
            }
            
            void onMessage(SoraMessageEvent* evt) {}
            
            SORA_IMPL_COMPONENT(Position);
        };
        
    } // namespace component
    
} // namespace sora



#endif
