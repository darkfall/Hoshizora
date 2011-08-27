//
//  SoraComponentFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentFactory_h
#define Sora_SoraComponentFactory_h

#include "../factory/SoraFactory.h"

#include "SoraComponent.h"
#include "SoraEntity.h"

namespace sora {
    
    typedef SoraAbstractFactory<SoraComponent*, SoraComponent*(SoraEntity&)> SoraComponentFactory;
    
    template<typename T>
    static void RegisterComponent(const SoraString& name, const T& fn) {
        SoraComponentFactory::Instance()->reg(name, fn);
    }
    
    static SoraComponent* CreateComponent(const SoraString& name, SoraEntity& entity) {
        SoraComponentFactory::Instance()->createInstance(name, entity);
    }
    
    static void DestroyComponent(SoraComponent* comp) {
        delete comp;
    }
    
} // namespace sora



#endif
