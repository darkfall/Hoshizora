//
//  Any.h
//  Sora
//
//  Created by Ruiwei Bu on 7/8/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_Any_h
#define Sora_Any_h

#include <algorithm>
#include <typeinfo>

#include "SoraException.h"

namespace sora {
    
    /**
     * A class that holds a general type
     * Code from Boost Library and Poco Library, Original Copyright by Kevlin Henney and Applied Infomatics
     * Modified for Hoshizora by Griffin Bu
     **/
    
    class SoraAny {
    public:
        SoraAny():
        mContent(NULL) {
        }
        
        template<typename ValueType>
        SoraAny(const ValueType& value):
        mContent(new Holder<ValueType>(value)) {
        }
        
        SoraAny(const SoraAny& other):
        mContent(other.mContent ? other.mContent->clone(): 0) {
        }
        
        ~SoraAny() {
            delete mContent;
        }
        
        SoraAny& swap(SoraAny& rhs) {
            std::swap(mContent, rhs.mContent);
            return *this;
        }
        
        template<typename ValueType>
        SoraAny& operator=(const ValueType& rhs) {
            SoraAny(rhs).swap(*this);
            return *this;
        }
        
        bool empty() const {
            return !mContent;
        }
        
        const std::type_info& type() const {
            return mContent ? mContent->type(): typeid(void);
        }
        
    private:
        class Placeholder {
        public:
            virtual ~Placeholder() {
                
            }
            
            virtual const std::type_info& type() const = 0;
            virtual Placeholder* clone() const = 0;
        };
        
        template<typename ValueType>
        class Holder: public Placeholder {
        public:
            Holder(const ValueType& value):
            mHeld(value) {
                
            }
            
            virtual const std::type_info& type() const {
                return typeid(ValueType);
            }
            
            virtual Placeholder* clone() const {
                return new Holder(mHeld);
            }
            
            ValueType mHeld;
        };
        
    private:
        template<typename ValueType>
        friend ValueType* AnyCast(SoraAny*);
        
        template<typename ValueType>
        friend ValueType* UnsafeAnyCast(SoraAny*);
        
        Placeholder* mContent;
    };
    
    template<typename ValueType>
    ValueType* AnyCast(SoraAny* operand) {
        return operand && operand->type() == typeid(ValueType) ?
        &static_cast<SoraAny::Holder<ValueType>*>(operand->mContent)->mHeld
        : 0;
    }
    
    template<typename ValueType>
    const ValueType* AnyCast(const SoraAny* operand) {
        return AnyCast<ValueType>(const_cast<SoraAny*>(operand));
    }
    
    template<typename ValueType>
    ValueType AnyCast(const SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(const_cast<SoraAny*>(&operand));
        if(!result)
            THROW_SORA_EXCEPTION("Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    ValueType AnyCast(SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(&operand);
        if(!result)
            THROW_SORA_EXCEPTION("Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    const ValueType& RefAnyCast(const SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(const_cast<SoraAny*>(&operand));
        if(!result)
            THROW_SORA_EXCEPTION("Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    ValueType* UnsafeAnyCast(SoraAny* operand) {
        return &static_cast<SoraAny::Holder<ValueType>*>(operand->mContent)->mHeld;
    }
    
    template<typename ValueType>
    const ValueType* UnsafeAnyCast(const SoraAny* operand) {
        return AnyCast<ValueType>(const_cast<SoraAny*>(operand));
    }
    
} // namespace sora

#endif
