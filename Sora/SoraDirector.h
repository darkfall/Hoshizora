//
//  SoraDirector.h
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraDirector_h
#define SoraF_SoraDirector_h

#include "SoraPlatform.h"

namespace sora {
    
    class SORA_API SoraDirector {
    protected:
        SoraDirector();
        ~SoraDirector();
        
    public:
        static SoraDirector& Instance();
        
        
    };
    
}

#endif
