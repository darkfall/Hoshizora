#ifndef SORA_STRING_H
#define SORA_STRING_H

#include <string>
#include "SoraConfig.h"
#include "SoraStringConv.h"

namespace sora {
    
    namespace util {

        class String {
        public:
            String();
            String(const std::string& str);
            String(const std::wstring& str);
            String(const char* str);
            String(const wchar_t* str);
            
            std::string get() const;
            std::wstring getw() const;
            
            const char* c_str() const;
            const wchar_t* wc_str() const;
            
            uint64 uniqieId() const;
            
            void set(const std::string& str);
            void set(const std::wstring& str);
            void set(const char* str);
            void set(const wchar_t* str);
            
            String& operator=(const std::string& str);
            String& operator=(const std::wstring& str);
            String& operator=(const char* str);
            String& operator=(const wchar_t* str);
            String& operator=(const String& str);
            
            String operator+(const std::string& str) const;
            String operator+(const std::wstring& str) const;
            String operator+(const char* str) const;
            String operator+(const wchar_t* str) const;
            
            String& operator+=(const std::string& str);
            String& operator+=(const std::wstring& str);
            String& operator+=(const char* str);
            String& operator+=(const wchar_t* str);
            
            operator std::string() {
                return mString;
            }
            operator std::wstring() {
                return s2ws(mString);
            }
            operator const char*() {
                return mString.c_str();
            }
            operator const wchar_t*() {
                return s2ws(mString).c_str();
            }
            
            char operator[](size_t index) const;
            char operator[](int index) const;
            
            char at(size_t index);
            wchar_t wat(size_t index);
            
            size_t size();
            
            int asInt() const;
            bool asBool() const;
            float asFloat() const;
            
            
        protected:
            std::string mString;
        };
        
        inline std::string operator+(const char* str, const String& sorastr) {
            std::string stdstr(str);
            return stdstr + sorastr.get();
        }
        
        inline int String::asInt() const {
            return atoi(mString.c_str());
        }
        
        inline bool String::asBool() const {
            static const char* bools[] = {
                "no", "yes", "off", "on", "false", "true", 0
            };
            int i = 0;
            while(bools[i] != 0) {
                if(mString == bools[i]) {
                    return 1 == (i & 1);
                }
                ++i;
            }
            return false;
        }
        
        inline float String::asFloat() const {
            return float(atof(mString.c_str()));
        }
        
    }
} // namespace sora

#endif // SORA_STRING_H