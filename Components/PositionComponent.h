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
            PositionComponent(SoraLightWeightEntity* ent):
            SoraComponent(ent, false) {
                
            }
            
            void onMessage(SoraMessageEvent* evt) {}
            
            SORA_DEF_COMPONENT(Position);
        };
        
    } // namespace component
    
} // namespace sora



#endif
