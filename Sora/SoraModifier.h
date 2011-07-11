//
//  SoraModifier.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraModifier_h
#define Sora_SoraModifier_h

#include "SoraAutoPtr.h"

namespace sora {
    
    /**
     *  Base class for Modifiers
     **/
    enum {
        ModifierUpdateEnd = -1
    };
        
    template<class MT>
    class SORA_API SoraModifier {
    public:
        typedef SoraAutoPtr<SoraModifier> Ptr;
        
        virtual int32 update(float32 dt) = 0;
        virtual void modify(MT* object) = 0;
        virtual SoraModifier<MT>* clone() = 0;
    };
    
} // namespace sora

#endif
