//
//  SoraComponent.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraComponent_h
#define Sora_SoraComponent_h

#include "../property/SoraDynRTTIClass.h"

namespace sora {
    
    /**
     * Helper macro to define a component class
     * Must be used within a class that derives from SoraComponent
     **/
#define SORA_DEF_COMPONENT(class, description) \
    SORA_DEF_DYN_RTTI_CLASS(class, description) \
    public: \
        static class* cast(::sora::SoraComponent*);
    
#define SORA_IMPL_COMPONENT(class) \
    SORA_IMPL_DYN_RTTI_CLASS(class)
    
#define SORA_IMPL_COMPONENT_1(class, parent) \
    SORA_IMPL_DYN_RTTI_CLASS_1(class, parent)
    
    struct SORA_API SoraComponent: uncopyable {
        virtual ~SoraComponent() {}
        
        SORA_DEF_COMPONENT(SoraComponent, "Component Base")
    };
    
} // namespace sora

#endif
