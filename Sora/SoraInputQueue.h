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
        void SORA_API publishInputedKey(int32 key, int32 type, char keyChr);
        bool SORA_API getQueueEvent(SoraKeyEvent& ev);
        void SORA_API setQueueInput(SoraInput* input);
        void SORA_API clearInputQueue();
        
        int32 SORA_API addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler);
        void SORA_API delGlobalHotkey(int32 hid);
        void SORA_API setGlobalHotkey(int32 hid, const SoraHotkey& hotkey);
        void SORA_API clearGlobalHotkeys();
    }
}


#endif
