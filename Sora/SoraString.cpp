//
//  String.cpp
//  Sora
//
//  Created by Robert Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraString.h"
#include "SoraException.h"
#include "stringId.h"

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
        
        
        uint64 String::uniqueId() const {
            return GetUniqueStringId(mString);
        }
        
        char String::operator[](size_t index) const {
            return static_cast<char>(mString[index]);
        }
        
        char String::at(size_t index) const {
            return static_cast<char>(mString[index]); 
        }
        
        wchar_t String::wat(size_t index) const {
            return mString[index];
        }
        
        size_t String::size() const {
            return mString.size();
        }
        
        
    }
    
} // namespace sora