//
//  SoraPropertyHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraPropertyHolder_h
#define Sora_SoraPropertyHolder_h

#include "SoraDynRTTIClass.h"

namespace sora {
    
    struct SoraPropertyHolder {
        typedef std::map<std::string, SoraProperty*> PropertyMap;
        typedef PropertyMap::const_iterator ConstPropertyIterator;
        typedef PropertyMap::iterator PropertyIterator;

        SoraPropertyHolder() {
            
        }
        
        virtual ~SoraPropertyHolder() {}
        
        virtual SoraDynRTTIClass* getClass() const = 0;
        
        template<typename T>
        bool getValue(const std::string& name, T& retValue) const {
            PropertyIterator it = mProperties.find(name);
            if(it == mProperties.end()) {
                return false;
            }
            sora_assert(it->second);
            return it->second->getValue<T>(retValue);
        }
        
        template<typename T>
        bool setValue(const std::string& name, T& val) {
            PropertyIterator it = mProperties.find(name);
            if(it == mProperties.end())
                return false;
            sora_assert(it->second);
            return it->second->setValue<T>(val);
        }
        
        void init() {
            initProperties(*getClass(), mProperties);
        }
        
    private:
        void initProperties(const SoraDynRTTIClass& cls, PropertyMap& properties) {
            typedef SoraDynRTTIClass::ParentClassMap ParentClassMap;

            const ParentClassMap& parentClass = cls.getParents();
            ParentClassMap::const_iterator it = parentClass.begin();
            ParentClassMap::const_iterator end = parentClass.end();
            for(; it != end; ++it) {
                initProperties(*(it->second), properties);
            }
            
            typedef SoraDynRTTIClass::PropertyInfoMap PropertyInfoMap;
            
            const PropertyInfoMap& defs = cls.getProperties();
            PropertyInfoMap::const_iterator itDef = defs.begin();
            PropertyInfoMap::const_iterator defEnd = defs.end();
            for(; itDef != defEnd; ++itDef) {
                const SoraPropertyInfo& def = itDef->second;
                if(def.getName().empty()) {
                    log_warning("warning: empty property name in class "+cls.getName()+". skipping.");
                    continue;
                }
                
                ConstPropertyIterator itProp = mProperties.find(def.getName());
                if(itProp != mProperties.end()) {
                    mProperties[def.getName()] = def.create();
                } else {
                    const SoraPropertyInfo& baseDef = itProp->second->getInfo();
                    log_warning("warning: class  "+cls.getName()+" attempts to override inherited property "+def.getName());
                    if(def.getType() == baseDef.getType()) {
                        log_warning("warning: overriding accepted");
                        itProp->second->setValue(def.getDefaultValue());
                    } else {
                        log_warning("warning: type mismatch, cannot override default value");
                    }
                }
            }
        }
        
    private:
        PropertyMap mProperties;
    };
    
} // namespace sora

#endif
