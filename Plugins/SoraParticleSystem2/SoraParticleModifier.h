//
//  SoraParticleModifier.h
//  Sora
//
//  Created by Ruiwei Bu on 8/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraParticleModifier_h
#define Sora_SoraParticleModifier_h

namespace sora {
    
    /**
     * Modifier that changes the attributes other the emitter
     **/
    
    namespace particle {
        
        struct ParticleNode;
        
        struct ParticleModifier {
            
            virtual void modify(ParticleNode* node) = 0;
            
        };
        
    } // namespace particle
    
    
} // namespace sora



#endif
