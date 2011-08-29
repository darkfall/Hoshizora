//
//  Test_EventFactory.h
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_Test_EventFactory_h
#define Sora_Test_EventFactory_h

#include "../UnitTest.h"
#include "../UnitTestException.h"
#include "../UnitTestResult.h"

#include "SoraCore.h"
#include "event/SoraEventFactory.h"

namespace SoraUnit {
    
    class UnitTest_Event: public Test {
    public:
        void run(TestResult* result) {            
            sora::SoraEvent* pbEvent = sora::SoraEventFactory::Instance()->createEvent("PlaybackEvent");
            sora::SoraEvent* timerEvent = sora::SoraEventFactory::Instance()->createEvent("TimerEvent");
            sora::SoraEvent* fcEvent = sora::SoraEventFactory::Instance()->createEvent("FileChangeEvent");
            sora::SoraEvent* keyEvent = sora::SoraEventFactory::Instance()->createEvent("KeyEvent");
            sora::SoraEvent* consoleEvent = sora::SoraEventFactory::Instance()->createEvent("ConsoleEvent");
            sora::SoraEvent* menubarEvent = sora::SoraEventFactory::Instance()->createEvent("MenubarEvent");
            sora::SoraEvent* hotkeyEvent = sora::SoraEventFactory::Instance()->createEvent("HotkeyEvent");
            
            if(pbEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating PlaybackEvent",
                                                               __LINE__,
                                                               __FILE__));
            if(timerEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating TimerEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            if(fcEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating FileChangeEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            if(keyEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating KeyEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            if(consoleEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating ConsoleEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            if(menubarEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating MenubarEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            if(hotkeyEvent = NULL)
                result->addFailure(this, new UnitTestException("Error creating HotkeyEvent",
                                                               __LINE__,
                                                               __FILE__));
            
            
            mResult = result->toString();

        }
        int countTestCases() {
            return 1;
        }
        std::string toString() {
            return "UnitTest_Event\nResult:"+mResult;
        }
        
    private:
        std::string mResult;
    };
    
}



#endif
