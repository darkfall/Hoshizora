//
//  SoraProperty.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraProperty_h
#define Sora_SoraProperty_h

#include "../SoraAny.h"
#include "../SoraTypeInfo.h"
#include "../SoraLogger.h"

namespace sora {
    
    struct SoraProperty;
    
    struct SoraPropertyInfo {
        SoraPropertyInfo(bool readonly=false):
        mReadOnly(readonly) { }
        
        SoraPropertyInfo& operator=(const SoraPropertyInfo& rhs) {
            if(this != &rhs) {
                mName = rhs.mName;
                mDescription = rhs.mDescription;
                mType = rhs.mType;
                mDefaultValue = rhs.mDefaultValue;
                return *this;
            }
        }
        
        template<typename T>
        SoraPropertyInfo(const std::string& name, const T& defaultValue, const std::string& description=std::string(), bool readonly=false):
        mName(name),
        mDescription(description),
        mType(typeid(T).name()),
        mDefaultValue(defaultValue),
        mReadOnly(readonly) {
            
        }
        
        std::string getName() const {
            return mName;
        }
        
        std::string getDescription() const {
            return mDescription;
        }
        
        std::string getType() const {
            return mType;
        }
        
        SoraAny getDefaultValue() const {
            return mDefaultValue;
        }
        
        SoraProperty* create() const;
        
    private:
        std::string mName;
        std::string mDescription;
        std::string mType;
        
        SoraAny mDefaultValue;
        
        bool mReadOnly;
    };
    
    struct SoraProperty {
        friend class SoraPropertyInfo;
        
        template<typename T>
        bool setValue(const T& v) {
            if(mReadOnly)
                return false;
            
            try {
                AnyCast<T>(mValue);
                mValue = v;
                return true;
            } catch(BadCastException&) {
                log_error("SoraProperty::setValue(): type missmatch"+mInfo.getType()+" expected, got "+typeid(v).name());
                return false;
            }
        }
        
        template<typename T>
        bool getValue(T& v) const {
            try {
                v = AnyCast<T>(mValue);
            } catch (BadCastException&) {
                return false;
            }
        }
        
        void setValue(const SoraAny& v) {
            mValue = v;
        }
        
        const SoraPropertyInfo& getInfo() const {
            return mInfo;
        }
        
    private:
        SoraProperty(const SoraPropertyInfo& info, const SoraAny& defaultVal, const bool readonly):
        mValue(defaultVal),
        mInfo(info),
        mReadOnly(readonly) {
            
        }
        
    private:
        SoraAny mValue;
        const SoraPropertyInfo& mInfo;
        bool mReadOnly;
    };
    
    
    inline SoraProperty* SoraPropertyInfo::create() const {
        return new SoraProperty(*this, mDefaultValue, mReadOnly);
    }

} // namespace sora



#endif
