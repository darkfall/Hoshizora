//
//  SoraRTTIClass.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraDynRTTIClass_h
#define Sora_SoraDynRTTIClass_h

/**
 * Dynamic RTTI class with properties and can be added and deleted in runtime
 **/

#include "SoraProperty.h"
#include "../SoraException.h"

#include <map>

namespace sora {
    
    class SoraDynRTTIClass {
    public:
        typedef std::map<std::string, SoraPropertyInfo> PropertyInfoMap;
        typedef std::map<std::string, SoraDynRTTIClass*> ParentClassMap;

        typedef std::vector<std::string> StringList;
    
        SoraDynRTTIClass(const std::string& name, const std::string& parent=std::string()):
        mName(name) {
            addParent(parent);
        }
        
        SoraDynRTTIClass(const std::string& name, const SoraDynRTTIClass& parent):
        mName(name),
        mProperties(parent.mProperties),
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
                mParents[name] = NULL;
        }
        
        const PropertyInfoMap& getProperties() const {
            return mProperties;
        }
        
        void addProperty(const SoraPropertyInfo& prop) {
            mProperties[prop.getName()] = prop;
        }
        
    private:
        std::string mName;
        PropertyInfoMap mProperties;
        
        ParentClassMap mParents;
        
    private:
        friend struct SoraDynRTTIClassManager;
        void setParent(const std::string& name, SoraDynRTTIClass* cls) {
            mParents[name] = cls;
        }
    };
    
    struct SoraDynRTTIClassManager {
        typedef std::map<std::string, SoraDynRTTIClass*> ClassPtrMap;

        void insert(SoraDynRTTIClass* cls) {
            sora_assert(cls);
            
            ClassPtrMap::const_iterator it = mClasses.find(cls->getName());
            if(it != mClasses.end())
                THROW_SORA_EXCEPTION(RttiException, "Class already registered");
            
            const SoraDynRTTIClass::ParentClassMap& parents = cls->getParents();
            if(!parents.empty()) {
                typedef SoraDynRTTIClass::ParentClassMap::const_iterator iterator;
                iterator it = parents.begin();
                while(it != parents.end()) {
                    SoraDynRTTIClass* parentCls = get(it->first);
                    if(parentCls != NULL)
                        cls->setParent(it->first, parentCls);
                    else
                        THROW_SORA_EXCEPTION(RttiException, "Parent class not found in Class Manager");
                    
                    ++it;
                }
            }
            
            mClasses.insert(std::make_pair(cls->getName(), cls));
        }
        
        SoraDynRTTIClass* get(const std::string& name) const {
            ClassPtrMap::const_iterator it = mClasses.find(name);
            if(it != mClasses.end()) 
                return it->second;
            return NULL;
        }
        
        void erase(SoraDynRTTIClass* cls) {
            sora_assert(cls);
            ClassPtrMap::const_iterator it = mClasses.find(cls->getName());
            if(it != mClasses.end())
                mClasses.erase(cls->getName());
        }
        
    private:
        ClassPtrMap mClasses;
    };
    
    /**
     * Helper macro to insert rtti class definition into a class
     * Must be used within a class
     * And Must Use with SORA_IMPL_DYN_RTTI_CLASS or SORA_IMPL_DYN_RTTI_CLASS_1 in pair
     **/
    
#define SORA_DEF_DYN_RTTI_CLASS(class, description) \
    public: \
        static std::string className() { \
            return std::string(#class); \
        } \
        static std::string classDescription() { \
            return std::string(description); \
        } \
        static sora::SoraDynRTTIClass* cls() { \
            return &rtti_cls; \
        } \
    private: \
        static sora::SoraDynRTTIClass rtti_cls; \
    public: \

#define SORA_IMPL_DYN_RTTI_CLASS(class) \
    ::sora::SoraDynRTTIClass class::rtti_cls(class::className());
    
#define SORA_IMPL_DYN_RTTI_CLASS_1(class, parent) \
    ::sora::SoraDynRTTIClass class::rtti_cls(class::className(), parent);
    
    
} // namespace sora


#endif
