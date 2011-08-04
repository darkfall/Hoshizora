//
//  SoraInstantiator.h
//  Sora
//
//  Created by Ruiwei Bu on 8/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraInstantiator_h
#define Sora_SoraInstantiator_h

#include "SoraPlatform.h"
#include "uncopyable.h"

namespace sora {
    
    template<class BASE>
    class SORA_API SoraAbstractInstantiator: uncopyable {
    public:
        SoraAbstractInstantiator() {}
        virtual ~SoraAbstractInstantiator() {}
        
        virtual BASE* createInstance() const = 0;
    };
    
    template<class C, class BASE>
    class SORA_API SoraInstantiator: public SoraAbstractInstantiator<BASE> {
    public:
        SoraInstantiator();
        ~SoraInstantiator();
        
        BASE* createInstance() const {
            return new C;
        }
    }
    
} // namespace sora



#endif
