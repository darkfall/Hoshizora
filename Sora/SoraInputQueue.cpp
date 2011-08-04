//
//  SoraInputQueue.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraInputQueue.h"
#include "SoraCore.h"
#include "SoraEventManager.h"

#include <stack>
#include <queue>

namespace sora {
    
    static SoraInput* g_input = NULL;
    
    typedef std::vector<SoraKeyEvent> KeyPool;
    static KeyPool g_keyPool;
    
    struct HotkeyNode {
        SoraHotkey mHotkey;
        int32 mId;
        
        SoraEventHandler* mHandler;
        
        HotkeyNode(const SoraHotkey& node, int32 id, SoraEventHandler* handler):
        mHotkey(node),
        mId(id),
        mHandler(handler) {}
    };
    
    typedef std::vector<HotkeyNode> HotkeyCont;
    static HotkeyCont g_hotkeys;
    static int32 g_next_hotkeyid = 0;
    
    float32 g_mousex = 0.f, g_mousey = 0.f;
    bool g_lbuttondown = false, g_rbuttondown = false, g_mbuttondown = false;
    
    typedef std::queue<SoraMouseEvent> MouseEventCont;
    MouseEventCont g_mouseEvents;
    
    typedef std::map<int32, SoraMouseListener*> MouseListenerMap;
    typedef std::map<int32, SoraKeyListener*> KeyListenerMap;
    typedef std::map<int32, SoraJoystickListener*> JoystickListenerMap;
    
    MouseListenerMap g_mouseListeners;
    KeyListenerMap g_keyListeners;
    JoystickListenerMap g_joystickListeners;
    
    void SoraKeyPool::setQueueInput(SoraInput* input) {
        g_input = input;
    }
    
    SoraKeyPool::SoraKeyPool() {
        SoraCore::Instance()->addFrameListener(this);
    }
    
    SoraKeyPool::~SoraKeyPool() {
        SoraCore::Instance()->delFrameListener(this);
    }
    
    void SoraKeyPool::clearInputQueue() {
        g_keyPool.clear();
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
    
    void SoraKeyPool::publishInputedKey(int32 key, int32 type, char keyChr) {
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
        
        // key listeners
        KeyListenerMap::iterator itListener = g_keyListeners.begin();
        while(itListener != g_keyListeners.end()) {
            if(ev.isKeyDown())
                itListener->second->keyPressed(ev);
            else 
                itListener->second->keyReleased(ev);
            ++itListener;
        }

        if(!ev.isConsumed())
            g_keyPool.push_back(ev);
    }
    
    bool SoraKeyPool::getQueueEvent(SoraKeyEvent& event) {
        if(g_keyPool.size() == 0)
            return false;
        
        event = g_keyPool.back();
        g_keyPool.pop_back();
        
        return true;
    }
    
    int32 SoraKeyPool::addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler) {
        g_hotkeys.push_back(HotkeyNode(hotkey, g_next_hotkeyid, handler));
        ++g_next_hotkeyid;
        return g_next_hotkeyid-1;
    }
    
    void SoraKeyPool::delGlobalHotkey(int32 hid) {
        for(int i = 0; i<g_hotkeys.size(); ++i)
            if(g_hotkeys[i].mId == hid) {
                g_hotkeys.erase(g_hotkeys.begin()+i);
                break;
            }
    }
    
    void SoraKeyPool::setGlobalHotkey(int32 hid, const SoraHotkey& hotkey) {
        for(int i = 0; i<g_hotkeys.size(); ++i)
            if(g_hotkeys[i].mId == hid) {
                g_hotkeys[i].mHotkey = hotkey;
                break;
            }
    }
    
    void SoraKeyPool::clearGlobalHotkeys() {
        g_hotkeys.clear();
        g_next_hotkeyid = 0;
    }
    
    void pollMouseInput() {
        float32 curMouseX, curMouseY;
        int mouseWheel;
        bool leftBtn, rightBtn, middleBtn;
        
        g_input->getMousePos(&curMouseX, &curMouseY);
        mouseWheel = g_input->getMouseWheel();
        
        leftBtn = g_input->getKeyState(SORA_KEY_LBUTTON)==SORA_INPUT_KEYDOWN;
        rightBtn = g_input->getKeyState(SORA_KEY_RBUTTON)==SORA_INPUT_KEYDOWN;
        middleBtn = g_input->getKeyState(SORA_KEY_MBUTTON)==SORA_INPUT_KEYDOWN;
        
        if(g_mousex != curMouseX ||
           g_mousey != curMouseY) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::MOVED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            g_mousex = curMouseX;
            g_mousey = curMouseY;
        }
        
        if(mouseWheel > 0) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::WHEEL_MOVED_UP,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        if(mouseWheel < 0) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::WHEEL_MOVED_DOWN,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(!g_lbuttondown && leftBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_LBUTTON,
                                              SoraMouseEvent::PRESSED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(!g_rbuttondown && rightBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_RBUTTON,
                                              SoraMouseEvent::PRESSED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(!g_mbuttondown && middleBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_MBUTTON,
                                              SoraMouseEvent::PRESSED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(g_lbuttondown && !leftBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_LBUTTON,
                                              SoraMouseEvent::RELEASED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(g_rbuttondown && !rightBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_RBUTTON,
                                              SoraMouseEvent::RELEASED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(g_mbuttondown && !middleBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_MBUTTON,
                                              SoraMouseEvent::RELEASED,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
                               
        g_lbuttondown = leftBtn;
        g_rbuttondown = rightBtn;
        g_mbuttondown = middleBtn;
    }

    void SoraKeyPool::addMouseListener(SoraMouseListener* mouseListener, int prio) {
        g_mouseListeners.insert(std::make_pair(prio, mouseListener));
    }
    
    void SoraKeyPool::addKeyListener(SoraKeyListener* keyListener, int prio) {
        g_keyListeners.insert(std::make_pair(prio, keyListener));
    }
    
    void SoraKeyPool::addJoystickListener(SoraJoystickListener* joyListener, int prio) {
        g_joystickListeners.insert(std::make_pair(prio, joyListener));
    }
    
    void SoraKeyPool::delMouseListener(SoraMouseListener* mouseListener) {
        MouseListenerMap::iterator itListener = g_mouseListeners.begin();
        while(itListener != g_mouseListeners.end()) {
            if(itListener->second == mouseListener) {
                g_mouseListeners.erase(itListener);
                break;
            }
            ++itListener;
        }
    }
    
    void SoraKeyPool::delKeyListener(SoraKeyListener* keyListener) {
        KeyListenerMap::iterator itListener = g_keyListeners.begin();
        while(itListener != g_keyListeners.end()) {
            if(itListener->second == keyListener) {
                g_keyListeners.erase(itListener);
                break;
            }
            ++itListener;
        }
    }
    
    void SoraKeyPool::delJoystickListener(SoraJoystickListener* joyListener) {
        JoystickListenerMap::iterator itListener = g_joystickListeners.begin();
        while(itListener != g_joystickListeners.end()) {
            if(itListener->second == joyListener) {
                g_joystickListeners.erase(itListener);
                break;
            }
            ++itListener;
        }
    }
    
    
    void SoraKeyPool::onFrameStart() {
        
    }
    
    void SoraKeyPool::onFrameEnd() {
        pollMouseInput();
        pollListenerEvents();
        clearInputQueue();
    }
    
    void SoraKeyPool::pollListenerEvents() {
        while(!g_mouseEvents.empty()) {
            SoraMouseEvent mevent = g_mouseEvents.front();
            g_mouseEvents.pop();
            
            MouseListenerMap::iterator itListener = g_mouseListeners.begin();
            while(itListener != g_mouseListeners.end()) {
                int type = mevent.type;
                switch(type) {
                    case SoraMouseEvent::MOVED:
                        if(!g_lbuttondown)
                            itListener->second->mouseMoved(mevent);
                        else {
                            mevent.type = SoraMouseEvent::DRAGGED;
                            itListener->second->mouseDragged(mevent);
                        }
                        break;
                    case SoraMouseEvent::PRESSED:
                        itListener->second->mouseClicked(mevent); break;
                    case SoraMouseEvent::RELEASED:
                        itListener->second->mouseReleased(mevent); break;
                    case SoraMouseEvent::WHEEL_MOVED_UP:
                        itListener->second->mouseWheelUp(mevent); break;
                    case SoraMouseEvent::WHEEL_MOVED_DOWN:
                        itListener->second->mouseWheelDown(mevent); break;
                    default:
                        break;
                }
                ++itListener;
            }
        }
        // to do joystick event
    }
}