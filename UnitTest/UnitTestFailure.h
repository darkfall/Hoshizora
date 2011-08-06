//
//  UnitTestFailure.h
//  Sora
//
//  Created by Ruiwei Bu on 8/6/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_UnitTestFailure_h
#define Sora_UnitTestFailure_h

#include "SoraUnit.h"
#include "UnitTestException.h"

namespace SoraUnit {
    
    class Test;
    
    class TestFailure {
    public:
        TestFailure(Test* test, UnitTestException* exp):
        mFailedTest(test),
        mThrownException(exp)
        {
            
        }
        
        ~TestFailure() {
            delete mThrownException;
        }
        
        Test* failedTest() {
            return mFailedTest;
        }
        
        UnitTestException* thrownException() {
            return mThrownException;
        }
        
        std::string toString() {
            return mThrownException->what();
        }
        
    protected:
        Test* mFailedTest;
        UnitTestException* mThrownException;
    };
    
} // namespace sora


#endif
