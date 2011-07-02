//
//  SoraInputQueue.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraInputQueue.h"

#include "SoraEventManager.h"

#include <stack>

namespace sora {
    
    namespace keypoll {
        static SoraInput* g_input = NULL;
        
        typedef std::stack<SoraKeyEvent> KEY_POLL;
        static KEY_POLL g_keyPool;
        
        struct HotkeyNode {
            SoraHotkey mHotkey;
            int32 mId;
            
            SoraEventHandler* mHandler;
            
            HotkeyNode(const SoraHotkey& node, int32 id, SoraEventHandler* handler):
            mHotkey(node),
            mId(id),
            mHandler(handler) {}
        };
        
        typedef std::vector<HotkeyNode> HOTKEY_CONT;
        static HOTKEY_CONT g_hotkeys;
        static int32 g_next_hotkeyid = 0;
        
        void setQueueInput(SoraInput* input) {
            g_input = input;
        }
        
        void clearInputQueue() {
            while(!g_keyPool.empty())
                g_keyPool.pop();
        }
        
        void checkHotkey(SoraKeyEvent* kev) {
            for(int i=0; i<g_hotkeys.size(); ++i) {
                if(g_hotkeys[i].mHotkey.test(kev)) {
                    SoraHotkeyEvent hev(&g_hotkeys[i].mHotkey, g_hotkeys[i].mId);
                    
                    g_hotkeys[i].mHandler->handleEvent(&hev);
                    kev->consume();
                }
            }
        }
        
        void publishInputedKey(int32 key, int32 type, char keyChr) {
            if(!g_input)
                return;
            
            SoraKeyEvent ev;
            
            ev.key = key;
            ev.type = type;
            
            ev.flags = 0;
            if(g_input->keyDown(SORA_KEY_SHIFT))
                ev.flags |= SORA_INPUT_FLAG_SHIFT;
            
            if(g_input->keyDown(SORA_KEY_ALT))
                ev.flags |= SORA_INPUT_FLAG_ALT;
            
            if(g_input->keyDown(SORA_KEY_CTRL))
                ev.flags |= SORA_INPUT_FLAG_CTRL;
            
            if(g_input->keyDown(SORA_KEY_CAPSLOCK))
                ev.flags |= SORA_INPUT_FLAG_CAPSLOCK;
            
            if(g_input->keyDown(SORA_KEY_SCROLLLOCK))
                ev.flags |= SORA_INPUT_FLAG_SCROLLLOCK;
            
            ev.chr = keyChr;
            
            ev.wheel = g_input->getMouseWheel();
            ev.x = g_input->getMousePosX();
            ev.y = g_input->getMousePosY();
            
            checkHotkey(&ev);
            
            if(!ev.isConsumed())
                SORA_EVENT_MANAGER->publishInputEvent(&ev);
            if(!ev.isConsumed())
                g_keyPool.push(ev);
        }
        
        bool getQueueEvent(SoraKeyEvent& event) {
            if(g_keyPool.size() == 0)
                return false;
            
            event = g_keyPool.top();
            g_keyPool.pop();
            
            return true;
        }
        
        
        int32 addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler) {
            g_hotkeys.push_back(HotkeyNode(hotkey, g_next_hotkeyid, handler));
            ++g_next_hotkeyid;
            return g_next_hotkeyid-1;
        }
        
        void delGlobalHotkey(int32 hid) {
            for(int i = 0; i<g_hotkeys.size(); ++i)
                if(g_hotkeys[i].mId == hid) {
                    g_hotkeys.erase(g_hotkeys.begin()+i);
                    break;
                }
        }
        
        void setGlobalHotkey(int32 hid, const SoraHotkey& hotkey) {
            for(int i = 0; i<g_hotkeys.size(); ++i)
                if(g_hotkeys[i].mId == hid) {
                    g_hotkeys[i].mHotkey = hotkey;
                    break;
                }
        }
        
        void clearGlobalHotkeys() {
            g_hotkeys.clear();
            g_next_hotkeyid = 0;
        }
    }
    
}