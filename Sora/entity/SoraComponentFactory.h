//
//  SoraComponentFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentFFactory_h
#define Sora_SoraComponentFFactory_h

#include "../factory/SoraFactory.h"
#include "SoraComponent.h"

namespace sora {
    
    typedef SoraAbstractFactory<SoraComponent, SoraComponent*(void)> SoraComponentFactory;
    
    template<typename T>
    static void RegisterComponent(const SoraString& name, const T& fn) {
        SoraComponentFactory::Instance()->reg(name, fn);
    }
    
    template<typename T>
    static void RegisterComponent(const SoraString& name) {
        SoraComponentFactory::Instance()->reg_ctor<T>(name);
    }
    
    static SoraComponent* CreateComponent(const SoraString& name) {
        SoraComponentFactory::Instance()->createInstance(name);
    }
    
    static void DestroyComponent(SoraComponent* comp) {
        delete comp;
    }
    
} // namespace sora



#endif
