//
//  Test_EventWorld.h
//  Sora
//
//  Created by Ruiwei Bu on 8/6/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_Test_EventWorld_h
#define Sora_Test_EventWorld_h

#include "../UnitTest.h"
#include "../UnitTestException.h"
#include "../UnitTestResult.h"

#include "SoraCore.h"
#include "SoraEvent.h"
#include "SoraSystemEvents.h"
#include "event/SoraEventWorld.h"

namespace SoraUnit {
    
    class myEvent: public sora::SoraEvent {
    public:
        std::string getMssg() const {
            return "My Event";
        }
        
        SORA_EVENT_IDENTIFIER(sora::str2id("myEvent"))
    };
    
    class myEventHandler: public sora::SoraEventHandler {
    public:
        myEventHandler() {
            registerEventFunc(this, &myEventHandler::onSystemEvent);
            registerEventFunc(this, &myEventHandler::onMyEvent);
        }
        
        void onSystemEvent(sora::SoraSystemEvent* evt) {
            std::ostringstream msg;
            msg << "Received System Event with type " << evt->getType() << "\n";
            mEvent += msg.str();
        }
        
        void onMyEvent(myEvent* evt) {
            mEvent += std::string("Received custom event: ")+evt->getMssg() + "\n";
        }
        
        uint32 update(float32 dt) {
            mEvent += std::string("Updated\n");
        }
        
                                  
        std::string getMssg() const {
            return mEvent;
        }
    private:
        std::string mEvent;
    };
    
    class UnitTest_EventWorld: public Test {
        void run(TestResult* result) {
            sora::SoraCore* sora = sora::SoraCore::Instance();
            mResult = "failed";
            try {
                sora::SoraEventWorld world(sora::SoraEventWorld::Parameter(false, false));
                
                sora::SoraEventHandler* handler1 = new sora::SoraEventHandler;
                myEventHandler* handler2 = new myEventHandler;
                handler2->enableUpdate(true);
                
                world.enter(handler1);
                world.enter(handler2);
                
                world.update(0.1f);
                
                sora::SoraEventFactory::Instance()->registerEvent<myEvent>("MyEvent");
                sora::SoraEvent* myevt = sora::SoraEventFactory::Instance()->createEvent("MyEvent");
                myevt->setChannel(sora::GetSystemChannel<1>());
                if(!myevt) {
                    result->addError(this, new UnitTestException("Error creating event",
                                                           __LINE__,
                                                           __FILE__));
                }
                world.broadcasting(myevt);
                world.update(0.1f);
                
                world.update(0.1f);

                
                mResult = handler2->getMssg();
                
            } catch(sora::SoraException& e) {
                result->addError(this, new UnitTestException(e.what(),
                                                             __LINE__,
                                                             __FILE__));
            }
            mResult += result->toString();
            
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
    
} // namespace SoraUnit

#endif
