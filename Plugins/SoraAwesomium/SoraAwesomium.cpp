//
//  SoraAwesomium.cpp
//  SoraF
//
//  Created by Ruiwei Bu on 6/6/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "SoraAwesomium.h"
#include "SoraSprite.h"
#include "SoraTexture.h"
#include "SoraCore.h"

namespace sora {
    
    SoraAwesomium::WebCorePtr SoraAwesomium::g_WebCore;
    
    SoraAwesomium::SoraAwesomium(int32 width, int32 height):
    mSprite(new SoraSprite(SoraTexture::CreateEmpty(width, height))){
        if(!g_WebCore)
            g_WebCore = new Awesomium::WebCore;
        mWebView = g_WebCore->createWebView(width, height);
    }
    
    SoraAwesomium::~SoraAwesomium() {
        if(mWebView)
            mWebView->destroy();
    }
    
    void SoraAwesomium::loadUrl(const SoraString& url) {
        if(mWebView)
            mWebView->loadURL(url);
    }
    
    void SoraAwesomium::loadHtml(const SoraString& content) {
        if(mWebView)
            mWebView->loadHTML(content);
    }
    
    void SoraAwesomium::loadFile(const SoraString& file) {
        if(mWebView)
            mWebView->loadFile(file);
    }
    
    void SoraAwesomium::setBaseUrl(const SoraString& url) {
        g_WebCore->setBaseDirectory(url);
    }
    
    void SoraAwesomium::executeJavaScript(const SoraString& js) {
        if(mWebView)
            mWebView->executeJavascript(js);
    }
    
    bool SoraAwesomium::isLoadingPage() const {
        if(mWebView)
            return mWebView->isLoadingPage();
        return false;
    }
    
    void SoraAwesomium::focus() {
        if(isFocused())
            return;
                /* ensure only one webview is alive at one time */
        for(AutoListElement<SoraAwesomium>::iterator it = this->members.begin();
            it != this->members.end();
            ++it) {
            (*it)->unfocus();
        }
        
        if(mWebView)
            mWebView->focus();
        mIsFocused = true;
        

        // to do with 
        SoraCore::Instance()->addInputListener(this);
        
    }
    
    void SoraAwesomium::unfocus() {
        if(mWebView)
            mWebView->unfocus();
        mIsFocused = false;
        
        SoraCore::Instance()->delInputListener(this);
    }
    
    bool SoraAwesomium::isFocused() const {
        return mIsFocused;
    }
    
    Awesomium::WebView* SoraAwesomium::getWebView() const {
        return mWebView;
    }
    
    SoraSprite* SoraAwesomium::getSprite() const {
        return mSprite.get();
    }
    
    void SoraAwesomium::onMouseMoved(SoraMouseEvent& event) {
        if(mWebView && mIsFocused) {
            mWebView->injectMouseMove(event.x, event.y);
        }
    }
    
    void SoraAwesomium::onMouseDragged(SoraMouseEvent& from, SoraMouseEvent& to) {
        // pass
    }
    
    void SoraAwesomium::onMouseWheelUp(SoraMouseEvent& event) {
        if(mWebView && mIsFocused) {
            mWebView->injectMouseWheel(event.wheel);
        }
    }
    
    void SoraAwesomium::onMouseWheelDown(SoraMouseEvent& event) {
        if(mWebView && mIsFocused) {
            mWebView->injectMouseWheel(event.wheel);
        }
    }
    
    void SoraAwesomium::onMouseClicked(SoraMouseEvent& event) {
        if(mWebView && mIsFocused) {
            mWebView->injectMouseDown(event.leftBtnDown ? Awesomium::LEFT_MOUSE_BTN :
                                      (event.rightBtnDown ? Awesomium::RIGHT_MOUSE_BTN :
                                       event.middleBtnDown ? Awesomium::MIDDLE_MOUSE_BTN : Awesomium::LEFT_MOUSE_BTN));
        }
    }
    
    void SoraAwesomium::onMouseReleased(SoraMouseEvent& event) {
        if(mWebView && mIsFocused) {
            mWebView->injectMouseUp(event.leftBtnDown ? Awesomium::LEFT_MOUSE_BTN :
                                      (event.rightBtnDown ? Awesomium::RIGHT_MOUSE_BTN :
                                       event.middleBtnDown ? Awesomium::MIDDLE_MOUSE_BTN : Awesomium::LEFT_MOUSE_BTN));
        }
    }
    
    void SoraAwesomium::onKeyPressed(SoraKeyEvent& event) {
       /* if(mWebView && mIsFocused) {
            
            if(event.isPrintable()) {
                Awesomium::WebKeyboardEvent aevent;
                
                if(event.isShiftFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_SHIFT_KEY;
                if(event.isAltFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_ALT_KEY;
                if(event.isCtrlFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_CONTROL_KEY;
                
                aevent.unmodifiedText[0] = event.key;
                aevent.text[0] = event.key;
                aevent.type = Awesomium::WebKeyboardEvent::TYPE_KEY_DOWN;
                aevent.virtualKeyCode = event.key;
                aevent.nativeKeyCode = event.key;
                mWebView->injectKeyboardEvent(aevent);
            }
        }*/
    }
    
    void SoraAwesomium::injectKey(int keyCode) {
        Awesomium::WebKeyboardEvent keyEvent;
        
        char* buf = new char[20];
        keyEvent.virtualKeyCode = keyCode;
        Awesomium::getKeyIdentifierFromVirtualKeyCode(keyEvent.virtualKeyCode, 
                                                      &buf);
        strcpy(keyEvent.keyIdentifier, buf);
        delete[] buf;
        
        keyEvent.modifiers = 0;
        keyEvent.nativeKeyCode = 0;
        keyEvent.type = Awesomium::WebKeyboardEvent::TYPE_KEY_DOWN;
        
        mWebView->injectKeyboardEvent(keyEvent);
        
        keyEvent.type = Awesomium::WebKeyboardEvent::TYPE_KEY_UP;
        
        mWebView->injectKeyboardEvent(keyEvent);
    }

    
    void SoraAwesomium::onKeyReleased(SoraKeyEvent& event) {
        if(mWebView && mIsFocused) {
            
            if(event.isPrintable()) {
                Awesomium::WebKeyboardEvent aevent;
                
                if(event.isShiftFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_SHIFT_KEY;
                if(event.isAltFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_ALT_KEY;
                if(event.isCtrlFlag())
                    aevent.modifiers = aevent.modifiers | Awesomium::WebKeyboardEvent::MOD_CONTROL_KEY;
                
                char realkey = toasciiWithFlag(event.key, event.flags);
                aevent.unmodifiedText[0] = realkey;
                aevent.text[0] = realkey;
                aevent.type = Awesomium::WebKeyboardEvent::TYPE_CHAR;
                aevent.virtualKeyCode = realkey;
                aevent.nativeKeyCode = realkey;
                mWebView->injectKeyboardEvent(aevent);
            } else {
                switch(event.key) {
                    case sora::key::Backspace: injectKey(Awesomium::KeyCodes::AK_BACK); break;
                    case sora::key::PageDown: injectKey(Awesomium::KeyCodes::AK_NEXT); break;
                    case sora::key::PageUp: injectKey(Awesomium::KeyCodes::AK_PRIOR); break;
                    case sora::key::Home: injectKey(Awesomium::KeyCodes::AK_HOME); break;
                    case sora::key::End: injectKey(Awesomium::KeyCodes::AK_END); break;
                    case sora::key::Up: injectKey(Awesomium::KeyCodes::AK_UP); break;
                    case sora::key::Down: injectKey(Awesomium::KeyCodes::AK_DOWN); break;
                    case sora::key::Left: injectKey(Awesomium::KeyCodes::AK_LEFT); break;
                    case sora::key::Right: injectKey(Awesomium::KeyCodes::AK_RIGHT); break;
                }
            }
        }
    }
    
    void SoraAwesomium::onJoystickPressed(SoraJoystickEvent& event) {
        
    }
    
    void SoraAwesomium::onJoystickRelesed(SoraJoystickEvent& event) {
        
    }
    
    int32 SoraAwesomium::update(float dt) {
        g_WebCore->update();
        return 0;
    }
    
    void SoraAwesomium::render() {
        if(mWebView && mSprite) {
            if(this->is3DEnabled()) {
                mSprite->enable3D(true);
            }
            mSprite->setTransform(this->getTransform());

            if(mWebView->isDirty()) {
                const Awesomium::RenderBuffer* buffer = mWebView->render();
                if(buffer) {
                    void* textureData = mSprite->getPixelData();
                    if(textureData) {
                        memcpy(textureData, buffer->buffer, mSprite->getTextureWidth() * mSprite->getTextureHeight() * 4);
                    }
                    mSprite->unlockPixelData();
                }
            }
            mSprite->render();
        }
    }
}