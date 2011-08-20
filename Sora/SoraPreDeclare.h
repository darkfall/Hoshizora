//
//  SoraPreDeclare.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraPreDeclare_h
#define Sora_SoraPreDeclare_h

#include "SoraPlatform.h"
#include "SoraAny.h"
#include "stringId.h"
#include <string>
#include <map>

namespace sora {
    
#ifdef USE_TR1_SHARED_PTR
    
#include <tr1/memory>
    #define SoraSharedPtr std::tr1::shared_ptr 
#else
    #define SoraSharedPtr SoraAutoPtr
#endif
    
    typedef std::map<std::string, SoraAny> NamedValueList;
    
    // cast a value in NamedValueList to T
    // would throw BadCastException if conversation fails
    template<typename T>
    static T GetTFromNamedValueList(const std::string& val, const NamedValueList& list) {
        NamedValueList::const_iterator it = list.find(val);
        if(it != list.end()) {
            return AnyCast<T>(it->second);
        }
    }
    
#define RT_FUNC_TO_ANY_RT_FUNC(func) \
    SoraAny SoraAnyFuncImpl0() { \
        return SoraAny(func()); \
    }
    
#define RT_FUNC_TO_ANY_RT_FUNC1(func, p1) \
    SoraAny SoraAnyFuncImpl1(p1 arg1) { \
        return SoraAny(func(arg1)); \
    }
    
#define RT_FUNC_TO_ANY_RT_FUNC2(func, p1, p2) \
    SoraAny SoraAnyFuncImpl2(p1 arg1, p2 arg2) { \
        return SoraAny(func(arg1, arg2)); \
    }
    
#ifdef SORA_USE_RTTI
    /**
     * RTTI Check 
     **/
    template<typename T1, typename T2>
    bool isClassSame(T1* t1, T2* t2) {
        return (&typeid(*t1) == &typeid(*t2) ||
                typeid(*t1) == typeid(t2));
    }
    template<typename T1, typename T2>
    bool isClassClass(T1* t1) {
        return (&typeid(*t1) == &typeid(T2) ||
                typeid(*t1) == typeid(T2));
    }
#endif
    
    // runtime def & not def
    
    static std::map<stringId, int> g_define_map;
    
    static void SoraDefine(const std::string& f, int val=0) {
        g_define_map.insert(std::make_pair(GetUniqueStringId(f), val));
    }
    
    static bool SoraIfDefined(const std::string& f) {
        std::map<stringId, int>::const_iterator it = g_define_map.find(GetUniqueStringId(f));
        if(it != g_define_map.end())
            return true;
        return false;
    }
    
#define SORA_DEFINE(name) \
    SoraDefine(#name)
    
#define SORA_IF_DEFINED(name) \
    SoraIfDefined(#name)
    
} // namespace sora


#endif
