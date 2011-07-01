//
//  SoraHotkey.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraHotkey_h
#define Sora_SoraHotkey_h

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

#include "SoraEvent.h"

namespace sora {
    
    class SoraHotkey;
    
    class SoraHotkeyEvent {
    public:  
        SoraHotkeyEvent(): mHotkey(NULL) {}
        SoraHotkeyEvent(SoraHotkey* hkey): mHotkey(hkey) {}
        
        SoraHotkey* getHotkey() const {
            return mHotkey;
        }
        
        SORA_EVENT_IDENTIFIER(str2id("SoraHotkeyEvent"));
        
    private:
        SoraHotkey* mHotkey;
    };
    
    class SoraHotkey {
    public:
        SoraHotkey();
        SoraHotkey(int32 key, bool ctrlFlag=false, bool altFlag=false, bool shiftFlag=false);
        
        /**
         *  if this flag equals true,
         *  then when test a hotkey, it will test whether the event is keypress event
         *  otherwise both keypress or keyrelease is ok
         *  you can take this advantage to make something like press to show, release to hide
         *  default true
         **/
        void setRequirePress(bool flag);
        
        bool test();
        bool test(SoraKeyEvent* kev);
        
        void set(int32 key, bool ctrlFlag=false, bool altFlag=false, bool shiftFlag=false);
        
        bool isCtrlFlag() const;
        bool isAltFlag() const;
        bool isShiftFlag() const;
        int32 getKey() const;
        
    private:
        int32 mKey;
        bool mRequirePress;
        
        bool mCtrlFlag, mAltFlag, mShiftFlag;
    };
    
} // namespace sora

#endif
