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
#include "../SoraException.h"
#include "../signal/SoraSignal.h"

#include "../uncopyable.h"

namespace sora {
    /*    
    class SoraProperty;
        
    struct SoraPropertyBase {
        virtual const PropertyIdent& getName() const = 0;
        
        virtual SoraStringId getTypeIdentifier() const = 0;
        
        virtual void fromString(const PropertyIdent& str) = 0;
        virtual PropertyIdent toString() = 0;
    };
        
    namespace property {
    
        template<typename T>
        struct ValueHolder {
            ValueHolder(const T& val, const PropertyIdent& name):
            mValue(val),
            mName(name) { }

            T mValue;
            PropertyIdent mName;
            
            typedef ::sora::SoraSignal<void(const SoraProperty&, const T&)> ValueChangedSignalType;
            ValueChangedSignalType mValueChangedSignal;
        };
    } // namespace property*/
    
    struct SoraPropertyInfo: uncopyable {
        virtual ~SoraPropertyInfo() {
            
        }
        
        std::string getName() const {
            return mName;
        }
        
        std::string getDescription() const {
            return mDescription;
        }
        
        const std::type_info& getType() const {
            return mType;
        }
        
        bool isReadOnly() const {
            return mReadOnly;
        }
        
        SoraPropertyInfo(const std::string& name, const std::type_info& type, const std::string& description=std::string(), bool readonly=false):
        mName(name),
        mDescription(description),
        mType(type),
        mReadOnly(readonly) {
            
        }
                
    private:
        std::string mName;
        std::string mDescription;
                
        const std::type_info& mType;
        bool mReadOnly;
    };
    
    template<typename T>
    struct SoraProperty: public SoraPropertyInfo {
        friend class SoraPropertyInfo;
        
        SoraProperty(const std::string& name, const T& defaultValue, const std::string& description=std::string(), bool readonly=false):
        SoraPropertyInfo(name, typeid(T), description, readonly),
        mValue(defaultValue) {
            
        }

        bool setValue(const T& v) {
            if(isReadOnly())
                return false;
            
            mValue = v;
            return true;
        }
        
        T getValue() const {
            return AnyCast<T>(mValue);
        }
        
        void setValue(const SoraAny& v) {
            mValue = v;
        }
        
    private:
        SoraAny mValue;
    };
    
    template<typename T>
    SoraProperty<T>& TryCastProperty(SoraPropertyInfo& prop) {
        if(typeid(T) != prop.getType()) {
            THROW_SORA_EXCEPTION(BadCastException, std::string("Invalid cast between ")+typeid(T).name()+" "+prop.getType().name());
        }
        return static_cast<SoraProperty<T>& >(prop);
    }
    
    template<typename T>
    T TryGetProperty(const SoraPropertyInfo& prop) {
        try {
            SoraProperty<T>& prop = TryCastProperty<T>(prop);
            return prop.getValue();
        } catch(BadCastException) {
            
        }
    }
    
    template<typename T>
    bool TrySetProperty(SoraPropertyInfo& prop, const T& newVal) {
        try {
            SoraProperty<T>& prop = TryCastProperty<T>(prop);
            prop.setValue(newVal);
            return true;
        } catch(BadCastException) {
            return false;
        }
    }
    
    struct SoraPropertyPtr {
        SoraPropertyPtr():
        mProperty(0) {
            
        }
        
        SoraPropertyPtr(SoraPropertyInfo* prop):
        mProperty(prop) {
            
        }
        
        bool valid() {
            return mProperty != NULL;
        }
        
        SoraPropertyInfo* get() {
            return mProperty;
        }
        
        template<typename T>
        bool is() {
            if(!valid()) {
                return false;
            }
            
            try {
                TryCastProperty<T>(*mProperty);
                return true;
            } catch(BadCastException) {
                return false;
            }
        }
        
        template<typename T>
        T to() {
            if(!valid()) {
                THROW_SORA_EXCEPTION(NullPointerException, "Ptr not valid");
            }
            
            if(is<T>()) {
                return TryGetProperty<T>(*mProperty);
            }
        }
        
        template<typename T>
        T unsafeTo() {
            if(!valid()) {
                THROW_SORA_EXCEPTION(NullPointerException, "Ptr not valid");
            }
            
            return static_cast<SoraProperty<T>&>(*mProperty).getValue();
        }
        
        template<typename T>
        void set(const T& val) {
            if(!valid()) {
                THROW_SORA_EXCEPTION(NullPointerException, "Ptr not valid");
            }
            
            if(is<T>()) {
                TrySetProperty<T>(*mProperty, val);
            }
        }
        
        template<typename T>
        void unsafeSet(const T& val) {
            if(!valid()) {
                THROW_SORA_EXCEPTION(NullPointerException, "Ptr not valid");
            }
            
            SoraProperty<T>& prop = static_cast<SoraProperty<T>&>(*mProperty);
            prop.setValue(val);
        }
        
        SoraPropertyPtr& operator=(const SoraPropertyPtr& rhs) {
            if(this != &rhs) {
                mProperty = rhs.mProperty;
            }
            return *this;
        }
        
        SoraPropertyPtr(const SoraPropertyPtr& rhs):
        mProperty(rhs.mProperty) {
            
        }
        
    private:
        SoraPropertyInfo* mProperty;
    };
    
    template<typename T>
    SoraProperty<T>* MakePointerProperty(const std::string& name, T* t) {
        return new SoraProperty<T*>(name, *t);
    }
    
    template<typename T>
    SoraProperty<T>* MakeValueProperty(const std::string& name, const T& t) {
        return new SoraProperty<T>(name, t);
    }
    
    template<typename T>
    SoraProperty<T>* MakeFunctionProperty(const std::string& name, const T& t) {
        return new SoraProperty<T>(name, t);
    }

} // namespace sora



#endif
