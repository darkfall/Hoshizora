//
//  UnitTest.h
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_UnitTest_h
#define Sora_UnitTest_h

#include "SoraUnit.h"
#include <string>

namespace SoraUnit {

    class TestResult;
    
    class SORA_API Test {
    public:
        virtual ~Test() = 0;
        virtual void run(TestResult* result) = 0;
        virtual int countTestCases() = 0;
        virtual std::string toString() = 0;
    };
    
    inline Test::~Test() {
        
    }
    
    inline void Test::run(TestResult* result) {
        
    }
    
    inline int Test::countTestCases() {
        return 0;
    }

    inline std::string Test::toString() {
        return "";
    }
} // namespace SoraUnit


#endif
