//
//  SoraPathImpliOS.mm
//  Sora
//
//  Created by Ruiwei Bu on 7/9/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraPathImpliOS.h"

#ifdef OS_IOS

#include <Foundation/Foundation.h>

namespace sora {
    
    inline std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	inline NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
    
    static std::string SoraPathImpl::getEnvImpl() {
        const char* val = getenv(name.c_str());
        if (val)
            return std::string(val);
        else
            THROW_SORA_EXCEPTION("Cannot get env string");
    }
    static std::string SoraPathImpl::currentImpl() {
        return [[[NSBundle mainBundle] bundlePath] UTF8String];
    }
    static std::string SoraPathImpl::homeImpl() {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		return (NSString2String(basePath)+"/");
    }
    static std::string SoraPathImpl::tempImpl() {
        return homeImpl();
    }
    static std::string SoraPathImpl::nullImpl() {
        return currentImpl();
    }
    static std::string SoraPathImpl::expandImpl(const std::string path) {
        std::string result;
        std::string::const_iterator it  = path.begin();
        std::string::const_iterator end = path.end();
        if (it != end && *it == '~') {
            ++it;
            if (it != end && *it == '/') {
                result += homeImpl(); ++it;
            }
            else result += '~';
        }
        while (it != end) {
            if (*it == '$') {
                std::string var;
                ++it;
                if (it != end && *it == '{') {
                    ++it;
                    while (it != end && *it != '}') var += *it++;
                    if (it != end) ++it;
                }
                else {
                    while (it != end && (Ascii::isAlphaNumeric(*it) || *it == '_')) var += *it++;
                }
                char* val = getenv(var.c_str());
                if (val) result += val;
            }
            else result += *it++;
        }
        return result;
    }
    static void SoraPathImpl::listRootsImpl(std::vector<std::string>& roots) {
        roots.clear();
        roots.push_back(currentImpl());
    }
} // namespace sora

#endif // OS_IOS