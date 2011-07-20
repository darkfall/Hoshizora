#ifndef SORA_STRING_H
#define SORA_STRING_H

#include <string>
#include "SoraConfig.h"
#include "SoraStringConv.h"

namespace sora {

	class SoraString {
    public:
        SoraString();
        SoraString(const std::string& str, bool fastConvert=false);
        SoraString(const std::wstring& str, bool fastConvert=false);
        SoraString(const char* str, bool fastConvert=false);
        SoraString(const wchar_t* str, bool fastConvert=false);
        
        std::wstring get() const;
        std::string gets() const;
        
        const char* c_str() const;
        const wchar_t* wc_str() const;
        
        uint64 uniqieId() const;
        
        void set(const std::string& str, bool fastConvert=false);
        void set(const std::wstring& str, bool fastConvert=false);
        void set(const char* str, bool fastConvert=false);
        void set(const wchar_t* str, bool fastConvert=false);
        
        SoraString& operator=(const std::string& str);
        SoraString& operator=(const std::wstring& str);
        SoraString& operator=(const char* str);
        SoraString& operator=(const wchar_t* str);
        SoraString& operator=(const char* str);
        
        bool operator == (const std::string& str);
        bool operator == (const std::wstring& str);
        
        bool compare(const std::string& str);
        bool compare(const std::wstring& str);
        bool compare(const char* str);
        bool compare(const wchar_t* str);
        
        size_t find(const std::string& str);
        size_t find(const std::wstring& str);
        
        size_t find(const char* str);
        size_t find(const wchar_t* str);
        
        size_t find(char t);
        size_t find(wchar_t t);
        
        SoraString operator+(const std::string& str);
        SoraString operator+(const std::wstring& str);
        SoraString operator+(const char* str);
        SoraString operator+(const wchar_t* str);
        
        SoraString& operator+=(const std::string& str);
        SoraString& operator+=(const std::wstring& str);
        SoraString& operator+=(const char* str);
        SoraString& operator+=(const wchar_t* str);
        
        operator std::string() {
            return ws2s(mString);
        }
        operator std::wstring() {
            return mString;
        }
        operator const char*() {
            return ws2s(mString).c_str();
        }
        operator const wchar_t*() {
            return mString.c_str();
        }
        
        char operator[](size_t index) const;
        char operator[](int index) const;
        
        char at(size_t index);
        wchar_t wat(size_t index);
        
        size_t size();
        
    protected:
        std::wstring mString;
    };
    
    std::string operator+(const char* str, const char* sorastr) {
        std::string stdstr(str);
        return stdstr + sorastr.gets();
    }
} // namespace sora

#endif // SORA_STRING_H