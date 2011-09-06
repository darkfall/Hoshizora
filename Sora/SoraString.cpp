//
//  String.cpp
//  Sora
//
//  Created by Robert Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "util::String.h"
#include "SoraException.h"
#include "SoraStringId.h"

namespace sora {
    
    namespace util {
        
        String::String() {
            
        }
        
        String::String(const std::string& str):
        mString(str) {
        }
        
        String::String(const std::wstring& str):
        mString(ws2s(str)) {
            
        }
        
        String::String(const char* str):
        mString(str) {
            
        }
        
        String::String(const wchar_t* str):
        mString(ws2s(str)) {
            
        }
        
        inline std::string String::get() const {
            return mString;
        }
        
        inline std::wstring String::getw() const {
            return s2ws(mString);
        }
        
        inline const char* String::c_str() const {
            return mString.c_str();
        }
        
        inline const wchar_t* String::wc_str() const {
            return s2ws(mString).c_str();
        }
        
        inline uint64 String::uniqieId() const {
            return GetUniqueStringId(mString);
        }
        
        size_t String::length() const {
            return mString.size();
        }
        
        inline void String::set(const std::string& str) {
            mString = str;
        }
        
        inline void String::set(const std::wstring& str) {
            mString = ws2s(str); 
        }
        
        inline void String::set(const char* str) {
            mString = str;
        }
        
        inline void String::set(const wchar_t* str) {
            mString = ws2s(str);
        }
        
        inline SoraStringTokenlizer String::tokenlize() const {
            return SoraStringTokenlizer(mString);
        }
        
        inline String& String::operator=(const std::string& str) {
             mString = str;
            return *this;
        }
        
        inline String& String::operator=(const std::wstring& str) {
            mString = ws2s(str);
            return *this;
        }
        
        inline String& String::operator=(const char* str) {
            mString = str;
            return *this;
        }
        
        inline String& String::operator=(const wchar_t* str) {
            mString = ws2s(str);
            return *this;
        }
        
        inline String& String::operator=(const String& str) {
            mString = str.mString;
            return *this;
        }
        
        String String::operator+(const std::string& str) const {
            return mString + str;
        }
        
        String String::operator+(const std::wstring& str) const {
            return mString + ws2s(str);
        }
        
        String& String::operator+=(const std::string& str) {
            mString += str;
            return *this;
        }
        
        String& String::operator+=(const std::wstring& str) {
            mString += ws2s(str);
            return *this;
        }
        
        String String::operator+(const char* str) const {
            return mString + str;
        }
        
        String String::operator+(const wchar_t* str) const {
            return mString + ws2s(str);
        }
        
        String& String::operator+=(const char* str) {
            mString += str;
            return *this;
        }
        
        String& String::operator+=(const wchar_t* str) {
            mString += ws2s(str);
            return *this;
        }
        
        char String::operator[](size_t index) const {
            return static_cast<char>(mString[index]);
        }
        
        char String::operator[](int index) const {
            return static_cast<char>(mString[index]);
        }
        
        char String::at(size_t index) {
            return static_cast<char>(mString[index]); 
        }
        
        wchar_t String::wat(size_t index) {
            return mString[index];
        }
        
        size_t String::size() {
            return mString.size();
        }
        
    }
    
} // namespace sora