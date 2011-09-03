//
//  UnitTestResult.h
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_UnitTestResult_h
#define Sora_UnitTestResult_h

#include "SoraUnit.h"
#include "UnitTestException.h"
#include "UnitTestFailure.h"

namespace SoraUnit {
    
    class Test;
    
    class SORA_API TestResult {
    public:
        TestResult():
        mStop(false),
        mRunTests(1) {
            
        }
        virtual void addError(Test* test, UnitTestException* e) {
            mErrors.push_back(new TestFailure(test, e));
        }
        virtual void addFailure(Test* test, UnitTestException* e) {
            mFailures.push_back(new TestFailure(test, e));
        }
        
        virtual void startTest(Test* test) {
            mRunTests++;
        }
        virtual void endTest(Test* test) {
            mRunTests--;
        }
        
        virtual int runTests() {
            return mRunTests;
        }
        virtual int testErrors() {
            return (int)mErrors.size();
        }
        virtual int testFailures() {
            return (int)mFailures.size();
        }
        
        virtual bool shouldStop() {
            return mStop;
        }
        
        virtual bool wasSuccessful() {
            return mFailures.size() == 0 && mErrors.size() == 0;
        }
        virtual void stop() {
            mStop = true;
        }
        
        virtual std::vector<TestFailure*>& errors() {
            return mErrors;
        }
        virtual std::vector<TestFailure*>& failures() {
            return mFailures;
        }
        
        virtual std::string toString() {
            std::string result;
            if(!wasSuccessful()) {
                for(int i=0; i<mErrors.size(); ++i) {
                    result += std::string("Error: ")+mErrors[i]->thrownException()->what();
                    result += "\n";
                }
                for(int i=0; i<mFailures.size(); ++i) {
                    result += std::string("Failure")+mFailures[i]->thrownException()->what();
                    result += "\n";
                }
            } else
                result += "Unit Test Succeed";
            return result;
        }
        
    protected:
        std::vector<TestFailure*> mErrors;
        std::vector<TestFailure*> mFailures;
        
        int mRunTests;
        bool mStop;        
    };
    
} // namespace sora


#endif
