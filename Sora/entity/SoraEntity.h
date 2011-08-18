//
//  SoraEntity.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEntity_h
#define Sora_SoraEntity_h

#include "../SoraObject.h"
#include "../SoraSimpleFSM.h"
#include "../SoraPreDeclare.h"

namespace sora {
    
    class SORA_API SoraEntity: public SoraObject {
    public:
		SoraEntity() {}
		virtual ~SoraEntity() {}
        
        
    protected:
        NamedValueList mProperties;
    };
    
} // namespace sora


#endif
