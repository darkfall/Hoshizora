//
//  SoraAwesomium.h
//  SoraF
//
//  Created by Ruiwei Bu on 6/6/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef SoraF_SoraAwesomium_h
#define SoraF_SoraAwesomium_h

#include "SoraObject.h"
#include "SoraInputListeners.h"
#include "SoraAutoPtr.h"
#include "SoraAutoContainer.h"

#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>

namespace sora {
    
    class SoraSprite;
    
    class SoraAwesomium: public SoraObject, public SoraInputListener, public AutoListElement<SoraAwesomium> /* mutual exclusion */ {
    public:
        SoraAwesomium(int32 width, int32 height);
        
        virtual ~SoraAwesomium();
        
        void loadUrl(const SoraString& url);
        void loadHtml(const SoraString& content);
        void loadFile(const SoraString& file);
        
        // base url to solve relative links in html content
        void setBaseUrl(const SoraString& url);
        
        void executeJavaScript(const SoraString& js);
        
        bool isLoadingPage() const;
        
        // only focused Awesomium node will inject mouse & key events
        void focus();
        void unfocus();
        bool isFocused() const;
        
        int32 getWidth() const;
        int32 getHeight() const;
        
        Awesomium::WebView* getWebView() const;
        SoraSprite* getSprite() const;
        
    public:
        virtual int32 update(float dt);
        virtual void render();
        
    private:
        // only one webcore instance is allowed
        typedef SoraAutoPtr<Awesomium::WebCore> WebCorePtr;
        static WebCorePtr g_WebCore;
        
        Awesomium::WebView* mWebView;
        
        bool mIsFocused;
        SoraUniquePtr<SoraSprite> mSprite;
        
        // listen input events to inject key & mouse events to webpage
    protected:
        void injectKey(int kc);
        
        void onMouseMoved(SoraMouseEvent& event);
        void onMouseDragged(SoraMouseEvent& from, SoraMouseEvent& to);
        void onMouseWheelUp(SoraMouseEvent& event);
        void onMouseWheelDown(SoraMouseEvent& event);
        void onMouseClicked(SoraMouseEvent& event);
        void onMouseReleased(SoraMouseEvent& event);
        
        void onKeyPressed(SoraKeyEvent& event);
        void onKeyReleased(SoraKeyEvent& event);
        
        void onJoystickPressed(SoraJoystickEvent& event);
        void onJoystickRelesed(SoraJoystickEvent& event);
    };
    
}

#endif
