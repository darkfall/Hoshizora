//
//  SoraModifier.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraModifier_h
#define Sora_SoraModifier_h

namespace sora {
    
    /**
     *  Base class for Modifiers
     **/
    
    template<class MT>
    class SoraModifier {
    public:
        virtual void modify(MT* object) = 0;
    };
    
} // namespace sora

#endif
