//
//  SoraRTTIFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraRTTIFactory_h
#define Sora_SoraRTTIFactory_h

#include "SoraFactory.h"
#include "../SoraException.h"
#include "../Debug/SoraInternalLogger.h"
#include "../RTTI/SoraRTTIClassManager.h"

namespace sora {
    
    /**
     * Factory that produce it's products based on SoraRTTI information
     **/
    class SORA_API SoraRTTIFactory: public SoraAbstractFactory<void> {
    public:
        virtual const std::string& getTypeName() const {
            static std::string type("RTTI");
            return type;
        }
        
        virtual PointerType createInstance(const std::string& instanceName, ParameterType parameter=NULL) {
            void** paramArray = new void*[parameter->size()];
            ParameterIterator itParam = parameter->begin();
            int index = 0;
            while(itParam != parameter->end()) {
                paramArray[index++] = UnsafeAnyCast<void*>(&itParam->second);
            }
            
            void* product = SoraRTTIClassManager::Instance()->constructClass(instanceName, paramArray, parameter->size(), NULL);
            if(product == NULL) {
                THROW_SORA_EXCEPTION(RuntimeException, vamssg("No class with name [%s] found with RTTI information", instanceName.c_str()));
            }
            return product;
        }
    };
    
} // namespace sora

#endif
