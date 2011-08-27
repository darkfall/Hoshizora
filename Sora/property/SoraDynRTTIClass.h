//
//  SoraRTTIClass.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDynRTTIClass_h
#define Sora_SoraDynRTTIClass_h

/**
 * Dynamic RTTI class with properties and can be added and deleted in runtime
 **/

#include "SoraPropertyHolder.h"

#include "../SoraException.h"

#include <map>

namespace sora {
    
    typedef std::string DynRttiClassKeyType;

    class SoraDynRTTIClass;
    
    struct SoraDynRTTIClassManager {
        typedef std::map<std::string, SoraDynRTTIClass*> ClassPtrMap;
        
        static void insert(SoraDynRTTIClass* cls);
        static SoraDynRTTIClass* get(const std::string& name);
        static void erase(SoraDynRTTIClass* cls);
        
    private:
        static ClassPtrMap mClasses;
    };

    class SoraDynRTTIClass {
    public:        
        typedef std::map<std::string, SoraDynRTTIClass*> ParentClassMap;

        typedef std::vector<std::string> StringList;
    
        SoraDynRTTIClass(const std::string& name, const std::string& parent=std::string()):
        mName(name) {
            addParent(parent);
        }
        
        SoraDynRTTIClass(const std::string& name, const SoraDynRTTIClass& parent):
        mName(name),
        mParents(parent.mParents) {
            
        }
        
        const std::string& getName() const {
            return mName;
        }
        
        const ParentClassMap& getParents() const {
            return mParents;
        }
        
        void addParent(const std::string& name) {
            if(!name.empty())
                mParents[name] = SoraDynRTTIClassManager::get(name);
        }
        
        void delParent(const std::string& name) {
            if(!name.empty()) {
                ParentClassMap::iterator it = mParents.find(name);
                if(it != mParents.end())
                    mParents.erase(it);
            }
        }
        
        /**
         * May throw a NullPointerException
         **/
        virtual SoraPropertyPtr getProperty(const DynRttiClassKeyType& key) const {
            SoraPropertyInfo* prop = mProperties.get(key);
            if(prop == NULL) {
                ParentClassMap::const_iterator itParent = mParents.begin();
                for(itParent; itParent != mParents.end(); ++itParent) {
                    SoraPropertyPtr propPtr = itParent->second->getProperty(key);
                    if(propPtr.valid()) 
                        return propPtr;
                }
            }
            return SoraPropertyPtr();
        }
        
        virtual bool addProperty(const DynRttiClassKeyType& key, SoraPropertyInfo* prop) {
            return mProperties.add(key, prop);
        }
        
        virtual bool addProperty(const DynRttiClassKeyType& key, SoraPropertyPtr prop) {
            return mProperties.add(key, prop.get());
        }
        
        virtual bool hasProperty(const DynRttiClassKeyType& key) {
            return getProperty(key).valid();
        }
        
        virtual SoraPropertyInfo* removeProperty(const DynRttiClassKeyType& key) {
            return mProperties.remove(key);
        }
        
        size_t getPropertySize() const {
            return mProperties.size();
        }
        
    private:
        std::string mName;
        SoraPropertyHolder<DynRttiClassKeyType> mProperties;
        
        ParentClassMap mParents;
        
    private:
        friend struct SoraDynRTTIClassManager;
        void setParent(const std::string& name, SoraDynRTTIClass* cls) {
            mParents[name] = cls;
        }
    };

        
    /**
     * Helper macro to insert rtti class definition into a class
     * Must be used within a class
     * And Must Use with SORA_IMPL_DYN_RTTI_CLASS or SORA_IMPL_DYN_RTTI_CLASS_1 in pair
     **/
    
#define SORA_DEF_DYN_RTTI_CLASS(class, description) \
    public: \
        static std::string getClassName() { \
            return std::string(#class); \
        } \
        static std::string getClassDescription() { \
            return std::string(description); \
        } \
        static sora::SoraDynRTTIClass* getRttiClass() { \
            return &rtti_cls; \
        } \
    private: \
        static sora::SoraDynRTTIClass rtti_cls; \
    public: 

#define SORA_IMPL_DYN_RTTI_CLASS(class) \
    ::sora::SoraDynRTTIClass class::rtti_cls(class::getClassName());
    
#define SORA_IMPL_DYN_RTTI_CLASS_1(class, parent) \
    ::sora::SoraDynRTTIClass class::rtti_cls(class::getClassName(), parent);
    
    
} // namespace sora


#endif
