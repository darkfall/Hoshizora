//
//  SoraTypeSerializer.h
//  SoraProperty
//
//  Created by Ruiwei Bu on 8/27/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraProperty_SoraTypeSerializer_h
#define SoraProperty_SoraTypeSerializer_h

#include <iostream>
#include <string>
#include <sstream>
#include "../SoraPlatform.h"
#include "../SoraStringConv.h"

namespace sora {
    
    class SoraPropertyBase;
    
  
    class SoraTypeSerializer {
    public:
        enum TypeId {
            TYPE_UNKNOWN = -1,
            
            TYPE_UINT = 1,
            TYPE_INT,
            
            TYPE_ULONG,
            TYPE_LONG,
            
            TYPE_UCHAR,
            TYPE_CHAR,
            
            TYPE_FLOAT,
            TYPE_DOUBLE,
            
            TYPE_UINT64,
            TYPE_INT64,
            
            TYPE_STRING,
            TYPE_WSTRING,
            
            TYPE_PTR,
        };
        
    public:
        static SoraPropertyBase* CreateProperty(TypeId tid, const std::string& name);
        
    public:
        template<typename T>
        static std::string toString(const T& val);
        
        template<typename T>
        static void fromString(const std::string& val, T* outValue);
        
        template<typename T>
        static int getTypeId(const T& val);
        
        template<typename T>
        static T defaultValue();
        
        template<typename T>
        static int getTypeId();
    };

    namespace property {
        
        template<typename T>
        struct Serializer;
        
        template<>
        struct Serializer<unsigned int> {
            static std::string toString(const unsigned int& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, unsigned int* outValue) {
                sscanf(val.c_str(), "%u", outValue);
            }
            static int getTypeId(const unsigned int& val) {
                return SoraTypeSerializer::TYPE_UINT;
            }
            static unsigned int defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<int> {
            static std::string toString(const int& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, int* outValue) {
                sscanf(val.c_str(), "%d", outValue);
            }
            static int getTypeId(const int& val) {
                return SoraTypeSerializer::TYPE_INT;
            }
            static int defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<float> {
            static std::string toString(const float& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, float* outValue) {
                sscanf(val.c_str(), "%f", outValue);
            }
            static int getTypeId(const float& val) {
                return SoraTypeSerializer::TYPE_FLOAT;
            }
            static float defaultValue() {
                return 0.f;
            }
        };
        
        template<>
        struct Serializer<double> {
            static std::string toString(const double& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, double* outValue) {
                sscanf(val.c_str(), "%lf", outValue);
            }
            static int getTypeId(const double& val) {
                return SoraTypeSerializer::TYPE_DOUBLE;
            }
            static double defaultValue() {
                return 0.0;
            }
        };
        
        template<>
        struct Serializer<char> {
            static std::string toString(const char& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, char* outValue) {
                sscanf(val.c_str(), "%c", outValue);
            }
            static int getTypeId(const char& val) {
                return SoraTypeSerializer::TYPE_CHAR;
            }
            static char defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<unsigned char> {
            static std::string toString(const unsigned char& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, unsigned char* outValue) {
                sscanf(val.c_str(), "%c", outValue);
            }
            static int getTypeId(const unsigned char& val) {
                return SoraTypeSerializer::TYPE_UCHAR;
            }
            static unsigned char defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<long> {
            static std::string toString(const long& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, long* outValue) {
                sscanf(val.c_str(), "%ld", outValue);
            }
            static int getTypeId(const long& val) {
                return SoraTypeSerializer::TYPE_LONG;
            }
            static long defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<unsigned long> {
            static std::string toString(const unsigned long& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, unsigned long* outValue) {
                sscanf(val.c_str(), "%lu", outValue);
            }
            static int getTypeId(const unsigned long& val) {
                return SoraTypeSerializer::TYPE_ULONG;
            }
            static unsigned long defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<int64> {
            static std::string toString(const int64& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, int64* outValue) {
                sscanf(val.c_str(), "%lld", outValue);
            }
            static int getTypeId(const int64& val) {
                return SoraTypeSerializer::TYPE_INT64;
            }
            static int64 defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<uint64> {
            static std::string toString(const uint64& val) {
                std::ostringstream sstr;
                sstr << val;
                return sstr.str();
            }
            static void fromString(const std::string& val, uint64* outValue) {
                sscanf(val.c_str(), "%llu", outValue);
            }
            static int getTypeId(const uint64& val) {
                return SoraTypeSerializer::TYPE_UINT64;
            }
            static uint64 defaultValue() {
                return 0;
            }
        };
        
        template<>
        struct Serializer<std::string> {
            static std::string toString(const std::string& val) {
                return val;
            }
            static void fromString(const std::string& val, std::string* outValue) {
                *outValue = val;
            }
            static int getTypeId(const std::string& val) {
                return SoraTypeSerializer::TYPE_STRING;
            }
            static std::string defaultValue() {
                return std::string();
            }
        };
        
        template<>
        struct Serializer<std::wstring> {
            static std::string toString(const std::wstring& val) {
                return ws2s(val);
            }
            static void fromString(const std::string& val, std::wstring* outValue) {
                *outValue = s2ws(val);
            }
            
            static int getTypeId(const std::wstring& val) {
                return SoraTypeSerializer::TYPE_WSTRING;
            }
            static std::wstring defaultValue() {
                return std::wstring();
            }
        };
        
        template<typename T>
        struct Serializer<T*> {
            static std::string toString(const T* val) {
                return Serializer<unsigned long>::toString((unsigned long)val);
            }
            static void fromString(const std::string& val, T** outValue) {
                unsigned long ptr;
                Serializer<unsigned long>::fromString(val, &ptr);
                *outValue = (T*)(ptr);
            }
            static int getTypeId(const T* val) {
                return SoraTypeSerializer::TYPE_PTR;
            }
            static T* defaultValue() {
                return 0;
            }
        };
        
    };

    template<typename T>
    inline std::string SoraTypeSerializer::toString(const T& val) {
        return property::Serializer<T>::toString(val);
    }
    
    template<typename T>
    inline void SoraTypeSerializer::fromString(const std::string& val, T* outValue) {
        property::Serializer<T>::fromString(val, outValue);
    }
    
    template<typename T>
    inline int SoraTypeSerializer::getTypeId(const T& val) {
        return property::Serializer<T>::getTypeId(val);
    }
    
    template<typename T>
    inline T SoraTypeSerializer::defaultValue() {
        return property::Serializer<T>::defaultValue();
    }
    
    template<typename T>
    inline int SoraTypeSerializer::getTypeId() {
        return property::Serializer<T>::getTypeId(defaultValue<T>());
    }
        
} // namespace sora

#endif
