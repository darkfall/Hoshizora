//
//  SoraString.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraString.h"
#include "SoraException.h"
#include "SoraStringConv.h"
#include "stringId.h"

namespace sora {
    
    SoraString::SoraString() {
        
    }
    
    SoraString::SoraString(const std::string& str, bool fastConvert) {
        if(fastConvert)
            mString = s2wsfast(str);
        else 
            mString = s2ws(str);
    }
    
    SoraString::SoraString(const std::wstring& str, bool fastConvert):
    mString(str) {
        
    }
    
    SoraString::SoraString(const char* str, bool fastConvert) {
        if(fastConvert)
            mString = s2wsfast(str);
        else 
            mString = s2ws(str);
    }
    
    SoraString::SoraString(const wchar_t* str, bool fastConvert):
    mString(str) {
        
    }
    
    inline std::wstring SoraString::get() const {
        return mString;
    }
    
    inline std::string SoraString::gets() const {
        return ws2s(mString);
    }
    
    inline const char* SoraString::c_str() const {
        return ws2s(mString).c_str();
    }
    
    inline const wchar_t* SoraString::wc_str() const {
        return mString.c_str();
    }
    
    inline uint64 SoraString::uniqieId() const {
        return GetUniqueStringId(mString);
    }
    
    inline void SoraString::set(const std::string& str, bool fastConvert) {
        if(fastConvert)
            mString = s2wsfast(str);
        else 
            mString = s2ws(str);
    }
    
    inline void SoraString::set(const std::wstring& str, bool fastConvert) {
        mString = str; 
    }
    
    inline void SoraString::set(const char* str, bool fastConvert) {
        if(fastConvert)
            mString = s2wsfast(str);
        else 
            mString = s2ws(str);
    }
    
    inline void SoraString::set(const wchar_t* str, bool fastConvert) {
        mString = str;
    }
    
    inline SoraString& SoraString::operator=(const std::string& str) {
        mString = s2ws(str);
        return *this;
    }
    
    inline SoraString& SoraString::operator=(const std::wstring& str) {
        mString = str;
        return *this;
    }
    
    inline SoraString& SoraString::operator=(const char* str) {
        mString = s2ws(str);
        return *this;
    }
    
    inline SoraString& SoraString::operator=(const wchar_t* str) {
        mString = str;
        return *this;
    }
    
    inline SoraString& SoraString::operator=(const SoraString& str) {
        mString = str.mString;
        return *this;
    }
    
    inline bool SoraString::operator == (const std::string& str) {
        return mString.compare(s2ws(str)) == 0;
    }
    
    inline bool SoraString::operator == (const std::wstring& str) {
        return mString.compare(str) == 0;
    }
    
    inline bool SoraString::compare(const char* str) {
        return mString.compare(s2ws(str)) == 0;
    }
    
    inline bool SoraString::compare(const wchar_t* str) {
        return mString.compare(str) == 0;
    }

    inline size_t SoraString::find(const std::string& str) {
        return mString.find(s2ws(str));
    }
    
    inline size_t SoraString::find(const std::wstring& str) {
        return mString.find(str);
    }
    
    inline size_t SoraString::find(const char* str) {
        return mString.find(s2ws(str));
    }
    
    inline size_t SoraString::find(const wchar_t* str) {
        return mString.find(str);
    }
    
    inline size_t SoraString::find(char t) {
        wchar_t tc = static_cast<wchar_t>(t);
        return mString.find(tc);
    }
    
    inline size_t SoraString::find(wchar_t t) {
        return mString.find(t);
    }
    
    SoraString SoraString::operator+(const std::string& str) {
        mString += s2ws(str);
        return *this;
    }
    
    SoraString SoraString::operator+(const std::wstring& str) {
        mString += str;
        return *this;
    }
    
    SoraString& SoraString::operator+=(const std::string& str) {
        mString += s2ws(str);
        return *this;
    }
    
    SoraString& SoraString::operator+=(const std::wstring& str) {
        mString += str;
        return *this;
    }
    
    SoraString SoraString::operator+(const char* str) {
        mString += s2ws(str);
        return *this;
    }
    
    SoraString SoraString::operator+(const wchar_t* str) {
        mString += str;
        return *this;
    }
    
    SoraString& SoraString::operator+=(const char* str) {
        mString += s2ws(str);
        return *this;
    }
    
    SoraString& SoraString::operator+=(const wchar_t* str) {
        mString += str;
        return *this;
    }
    
    char SoraString::operator[](size_t index) const {
        return static_cast<char>(mString[index]);
    }
    
    char SoraString::operator[](int index) const {
        return static_cast<char>(mString[index]);
    }
    
    char SoraString::at(size_t index) {
        return static_cast<char>(mString[index]); 
    }
    
    wchar_t SoraString::wat(size_t index) {
        return mString[index];
    }
    
    size_t SoraString::size() {
        return mString.size();
    }
    
} // namespace sora