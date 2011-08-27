//
//  SoraPropertyHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPropertyHolder_h
#define Sora_SoraPropertyHolder_h

#include "SoraProperty.h"
#include "../SoraIteratorHelper.h"

#include <map>

namespace sora {
    
    template<typename KeyType>
    struct SoraPropertyHolder {
        typedef KeyType PropertyKeyType;
        typedef std::map<PropertyKeyType, SoraPropertyInfo*> PropertyMap;
        typedef typename PropertyMap::const_iterator ConstPropertyIterator;
        typedef typename PropertyMap::iterator PropertyIterator;

        SoraPropertyHolder() {
            
        }
        
        virtual ~SoraPropertyHolder() {}
        
        SoraPropertyInfo* get(const PropertyKeyType& name) const {
            ConstPropertyIterator it = mProperties.find(name);
            if(it == mProperties.end()) {
                return NULL;
            }
            sora_assert(it->second);
            return it->second;
        }
        
        bool add(const PropertyKeyType& name, SoraPropertyInfo* prop) {
            PropertyIterator it = mProperties.find(name);
            if(it == mProperties.end()) {
                mProperties[name] = prop;
                return true;
            }
            return false;
        }
        
        SoraPropertyInfo* remove(const PropertyKeyType& name) {
            PropertyIterator it = mProperties.find(name);
            if(it != mProperties.end()) {
                SoraPropertyInfo* prop = it->second;
                mProperties.erase(it);
                return prop;
            }
            return NULL;
        }
        
        size_t size() const {
            return mProperties.size();
        }
        
        SORA_ITERABLE(typename PropertyMap, mProperties)
        
    private:
        PropertyMap mProperties;
    };
    
} // namespace sora

#endif
