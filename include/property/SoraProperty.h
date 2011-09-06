//
//  SoraProperty.h
//  SoraProperty
//
//  Created by Ruiwei Bu on 8/27/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraProperty_SoraProperty_h
#define SoraProperty_SoraProperty_h

#include <string>

#include "SoraTypeSerializer.h"

namespace sora  {
    
    namespace property {
        
        template<typename T>
        struct ValueHolder {
            T mValue;
            std::string mName;
            
            ValueHolder():
            mValue(SoraTypeSerializer::defaultValue<T>()) {}
            
            ValueHolder(const std::string& name, const T& defaultValue):
            mName(name),
            mValue(defaultValue) {}
            
            ValueHolder<T>* clone() const {
                return new ValueHolder<T>(mName, mValue);
            }
        };
        
    } // namespace property
    
    class SoraPropertyBase {
	public:
        virtual ~SoraPropertyBase() {}
        
        virtual const std::string& getName() const = 0;
    
        virtual bool isNull() const = 0;
        
        virtual std::string toString() const =0;
        virtual void setFromString(const std::string& value) = 0;
        
        virtual int getTypeId() const = 0;
            
    private:
        std::string mName;
    };
    
    template<typename T, class S=SoraTypeSerializer>
    class SoraProperty: public SoraPropertyBase {
    public:
        typedef S Serializer;
        
        SoraProperty():
        mIsDefault(true) {
            
        }
        
        SoraProperty(const SoraProperty& rhs):
        mIsDefault(false),
        mData(rhs.mData) {
        }
        
        SoraProperty(const std::string& name, const T& defaultValue=SoraTypeSerializer::defaultValue<T>()):
        mIsDefault(true),
        mData(name, defaultValue) {            
        }
        
        virtual ~SoraProperty() {
        }
        
        void set(const T& val) {
            if(mData.mValue != val) {
                T oldVal = mData.mValue;
                mData.mValue = val;
                
                mIsDefault = false;
            }
        }
        
        const T& get() const {
            return mData.mValue;
        }
        
        virtual bool isNull() const {
            return mIsDefault;
        }
        
        virtual const std::string& getName() const {
            return mData.mName;
        }
        
        virtual std::string toString() const {
            return Serializer::toString(mData.mValue);
        }
        
        virtual void setFromString(const std::string& str) {
            mIsDefault = false;
            Serializer::fromString(str, &mData.mValue);
        }
        
        virtual int getTypeId() const {
            return Serializer::getTypeId(mData.mValue);
        }
        
        SoraProperty<T, S> operator=(const SoraProperty<T, S>& rhs);
        SoraProperty<T, S> operator=(const T& rhs);
        
        bool operator==(const SoraProperty<T, S>& rhs);
        bool operator==(const T& rhs);
        
        bool operator!=(const SoraProperty<T, S>& rhs);
        bool operator!=(const T& rhs);
        
        bool operator>(const SoraProperty<T, S>& rhs);
        bool operator>(const T& rhs);
        
        bool operator<(const SoraProperty<T, S>& rhs);
        bool operator<(const T& rhs);
        
        operator T() const {
            return mData->mValue;
        }
        
    private:
        bool mIsDefault;
        property::ValueHolder<T> mData;
    };
    
    template<typename T, class S>
    inline SoraProperty<T, S> SoraProperty<T, S>::operator=(const SoraProperty<T, S>& rhs) {
        mData = rhs.mData.clone();
        return *this;
    }
    
    template<typename T, class S>
    inline SoraProperty<T, S> SoraProperty<T, S>::operator=(const T& rhs) {
        set(rhs);
        return *this;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator==(const SoraProperty<T, S>& rhs) {
        return mData.mValue == rhs.mData.mValue;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator==(const T& rhs) {
        return mData.mValue == rhs;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator!=(const SoraProperty<T, S>& rhs) {
        return mData.mValue != rhs.mData.mValue;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator!=(const T& rhs) {
        return mData.mValue != rhs;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator>(const SoraProperty<T, S>& rhs) {
        return mData.mValue > rhs.mData.mValue;
    }
   
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator>(const T& rhs) {
        return mData.mValue > rhs.mValue;
    }
    
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator<(const SoraProperty<T, S>& rhs) {
        return mData.mValue < rhs.mData.mValue;
    }
 
    template<typename T, class S>
    inline bool SoraProperty<T, S>::operator<(const T& rhs) {
        return mData.mValue < rhs;
    }
    
} // namespace sora



#endif