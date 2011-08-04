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
#include "SoraAutoPtr.h"
#include "SoraAny.h"

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
    
} // namespace sora


#endif
