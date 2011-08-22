//
//  SoraComponentHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraComponentHolder_h
#define Sora_SoraComponentHolder_h

#include "SoraComponent.h"
#include "uncopyable.h"

namespace sora {
    
    struct SORA_API SoraComponentHolder: uncopyable {
        SoraComponentHolder() {}
        
        /**
         * Add a component, may throw a ExistsExceptio
         **/
        void addComponent(const std::string& tag, SoraComponent* cop);
        
        SoraComponent* removeComponent(const std::string& tag);
        SoraComponent* removeComponent(const SoraComponent*);
        
        SoraComponent* getComponent(const std::string& tag);
        
    private:
        typedef std::map<std::string, SoraComponent*> ComponentMap;
        ComponentMap mComponents;
    };
    
} // namespace sora


#endif
