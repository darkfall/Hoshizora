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

namespace sora {
    
    namespace keypoll {
        void publishInputedKey(int32 key, int32 type, char keyChr);
        bool getQueueEvent(SoraKeyEvent& ev);
        void setQueueInput(SoraInput* input);
        void clearInputQueue();
        
        int32 addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler);
        void delGlobalHotkey(int32 hid);
        void setGlobalHotkey(int32 hid, const SoraHotkey& hotkey);
        void clearGlobalHotkeys();
    }
}


#endif
