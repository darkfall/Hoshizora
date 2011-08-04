//
//  SoraDynamicFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraDynamicFactory_h
#define Sora_SoraDynamicFactory_h

#include "../SoraPlatform.h"
#include "../SoraException.h"
#include "../SoraInstantiator.h"

namespace sora {
    
    /**
     * A helper that manages allow users to create a class instance by it's name
     * Users must register class before instantiate it
     * And the Class must have a default ctor to be constructed
     * Unlike SoraRTTI, no any constructor support
     **/    
    template<class BASE>
    class SORA_API SoraDynamicFactory {
    public:
        typedef SoraAbstracteInstantiator<BASE> AbstractFactory;
        
        template<class C>
        void registerClass(const std::string& name) {
            registerClass(name, new SoraInstantiator<BASE, C>());
        }
        
        template<class C>
        void registerClass(const std::string& name, AbstractFactory* factory) {
            FactoryMap::iterator itFactory = mFactory.find(name);
            
            if(itFactory == mFactory.end()) {
                mFactory[name] = factory;
            } else 
                THROW_SORA_EXCEPTION(ExistsException, "Class already exists");
        }
        
        void unregisterClass(const std::string& name) {
            FactoryMap::iterator itFactory = mFactory.find(name);
        
            if(itFactory != mFactory.end()) {
                delete itFactory->second;
                mFactory.erase(itFactory);
            }
        }
        
        bool isClass(const std::string& name) {
            FactoryMap::iterator itFactory = mFactory.find(name);
            return itFactory != mFactory.end();
        }
        
        BASE* createInstance(const std::string& name) {
            FactoryMap::iterator itFactory = mFactory.find(name);
            if(itFactory != mFactory.end()) {
                return itFactory->second->createInstance();
            }
            THROW_SORA_EXCEPTION(NotFoundException, vamssg("Factory not found for class %s", name.c_str()));
            return NULL;
        }
        
    private:
        typedef std::map<std::string, AbstractFactory*> FactoryMap;
        FactoryMap mFactory;
    };
    
    
} // namespace sora



#endif
