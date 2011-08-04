//
//  SoraInputQueue.h
//  Sora
//
//  Created by Ruiwei Bu on 7/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraInputQueue_h
#define Sora_SoraInputQueue_h

#include "SoraKeyInfo.h"
#include "SoraHotkey.h"
#include "SoraInput.h"
#include "SoraInputListeners.h"
#include "SoraFrameListener.h"

#include <stack>

namespace sora {
        
    class SORA_API SoraKeyPool: public SoraFrameListener {
    private:
        SoraKeyPool();
        ~SoraKeyPool();
        void onFrameStart();
        void onFrameEnd();
        
        friend class SoraCore;
        
    public:        
        typedef std::stack<SoraKeyEvent> IteratorType;

        static void publishInputedKey(int32 key, int32 type, char keyChr);
        static bool getQueueEvent(SoraKeyEvent& ev);
        static void setQueueInput(SoraInput* input);
        static void clearInputQueue();
        
        static int32 addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler);
        static void delGlobalHotkey(int32 hid);
        static void setGlobalHotkey(int32 hid, const SoraHotkey& hotkey);
        static void clearGlobalHotkeys();
        
        static void addMouseListener(SoraMouseListener* mouseListener, int prio=0);
        static void addKeyListener(SoraKeyListener* keyListener, int prio=0);
        // joystick listener may not work now cause joystick support is not finished yet
        static void addJoystickListener(SoraJoystickListener* joyListener, int prio=0);
        
        static void delMouseListener(SoraMouseListener* mouseListener);
        static void delKeyListener(SoraKeyListener* keyListener);
        static void delJoystickListener(SoraJoystickListener* joyListener);
        
        static void pollListenerEvents();
    };
}


#endif
