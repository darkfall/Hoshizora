//
//  SoraTypeSerializer.h
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTypeSerializer_h
#define Sora_SoraTypeSerializer_h

#include "../SoraPlatform.h"
#include "../stringId.h"

#include <cstdio>
#include <iostream>

namespace sora {

    class SoraProperty;
    
    typedef SoraString PropertyIdent;
    
    template<typename T>
    class SoraTypeSerializer {
    public:
        static PropertyIdent toString();
        static void fromString(T& outValue);
        static SoraStringId getTypeIdentifier();
    };
    
    template<> 
    class SoraTypeSerializer<int32> {
    public:
        static PropertyIdent toString(const int32& val) {
            std::ostringstream str;
            str<<val;
            return str.str();
        }
        static void fromString(const PropertyIdent& prop, int32& val) {
            sscanf(prop.c_str(), "%d", val);
        } 
        static SoraStringId getTypeIdentifier() {
            return str2id("int32");
        } 
    };
    
    template<> 
    class SoraTypeSerializer<float> {
    public:
        static PropertyIdent toString(const float& val) {
            std::ostringstream str;
            str<<val;
            return str.str();
        }
        static void fromString(const PropertyIdent& prop, float& val) {
            sscanf(prop.c_str(), "%f", val);
        } 
        static SoraStringId getTypeIdentifier() {
            return str2id("float");
        } 
    };
    
    template<> 
    class SoraTypeSerializer<ulong32> {
    public:
        static PropertyIdent toString(const ulong32& val) {
            std::ostringstream str;
            str<<val;
            return str.str();
        }
        static void fromString(const PropertyIdent& prop, ulong32& val) {
            sscanf(prop.c_str(), "%ul", val);
        } 
        static SoraStringId getTypeIdentifier() {
            return str2id("ulong32");
        } 
    };
    
    template<> 
    class SoraTypeSerializer<double> {
    public:
        static PropertyIdent toString(const double& val) {
            std::ostringstream str;
            str<<val;
            return str.str();
        }
        static void fromString(const PropertyIdent& prop, double& val) {
            sscanf(prop.c_str(), "%f", val);
        } 
        static SoraStringId getTypeIdentifier() {
            return str2id("double");
        } 
    };
    
    
    
} // namespace sora



#endif
