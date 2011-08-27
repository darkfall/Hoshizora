//
//  UnitTestSuite.h
//  Sora
//
//  Created by Ruiwei Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_UnitTestSuite_h
#define Sora_UnitTestSuite_h

#include "SoraUnit.h"
#include "UnitTest.h"
#include "UnitTestResult.h"

#include <vector>
#include <string>

namespace SoraUnit {
    
    
    class TestSuite: public Test {
    public:
        TestSuite(const std::string& name = ""):
        mName(name) {
            
        }
        ~TestSuite() {
            deleteContents();
        }
        
        void run(TestResult* result) {
            for(int i=0; i<mTests.size(); ++i) {
                if(result->shouldStop()) {
                    break;
                }
                
                mTests[i]->run(result);
            }
        }
        int countTestCases() {
            int count = 0;
            for(int i=0; i<mTests.size(); ++i) {
                count += mTests[i]->countTestCases();
            }
            return count;
        }
        
        void addTest(Test* test) {
            mTests.push_back(test);
        }
        std::string toString() {
            std::string result = "UnitTestSuite " + mName;
            for(int i=0; i<mTests.size(); ++i) {
                result += mTests[i]->toString() + "\n";
            }
            return result;
        }
        
        virtual void deleteContents() {
            for(int i=0; i<mTests.size(); ++i) {
                delete mTests[i];
            }
            mTests.clear();
        }
        
        const std::vector<Test*> tests() const {
            return mTests;
        }
        
    private:
        std::vector<Test*> mTests;
        const std::string mName;
    };
    
}

#define SET_AND_RUN_TEST(test, result) \
    SoraUnit::TestSuite testSuite; \
    testSuite.addTest(test); \
    testSuite.run(result); \
    printf("%s\n", testSuite.toString().c_str()); 
    

#endif
