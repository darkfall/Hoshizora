//
//  SoraPointTemplte.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPointTemplate_h
#define Sora_SoraPointTemplate_h

namespace sora {
    
    template<int Count, typename Type>
    struct SoraPointTemplate {
        
        typedef SoraPointTemplate<Count, Type> SelfType;
        typedef Type PointType;
        
        enum {
            PointCount = Count,
        };
        
        sora_static_assert(Count > 1);
        
        template<typename T2>
        SelfType operator+(const SoraPointTemplate<Count, T2>& rhs) const {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                tmp.value[i] += static_cast<Type>(rhs.value[i]);
            }
            return tmp;
        }
        
        template<typename T2>
        SelfType operator-(const SoraPointTemplate<Count, T2>& rhs) const {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                tmp.value[i] -= static_cast<Type>(rhs.value[i]);
            }
            return tmp;
        }
        
        template<typename T2>
        SelfType& operator=(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                this->value[i] = static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        template<typename T2>
        bool operator==(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                if(this->value[i] != static_cast<Type>(rhs.value[i]))
                    return false;
            }
            return true;
        }
        
        template<typename T2>
        bool operator!=(const SoraPointTemplate<Count, T2>& rhs) {
            return !(*this == rhs);
        }
        
        template<typename T2>
        SelfType& operator+=(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                this->value[i] += static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        template<typename T2>
        SelfType& operator-=(const SoraPointTemplate<Count, T2>& rhs) {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                this->value[i] -= static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        Type& operator[](uint32 index) {
            sora_assert(index >= 0 && index < PointCount);
            return this->value[index];
        }
        
        const Type& operator[](uint32 index) const {
            sora_assert(index >= 0 && index < PointCount);
            return this->value[index];
        }
        
        
        Type value[Count];
    };
    
    typedef SoraPointTemplate<2, float> float2;
    typedef SoraPointTemplate<3, float> float3;
    typedef SoraPointTemplate<4, float> float4;
    
} // namespace sora

#endif
