//
//  SoraGameApp.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGameApp.h"
#include "SoraCore.h"
#include "SoraGameState.h"

namespace sora {
    
    SoraGameApp::GameAppWindow::GameAppWindow(const SoraGameAppDef& def, SoraFSMManager& manager):
    mDef(def),
    mFSMManager(manager) {
        
    }
    
    bool SoraGameApp::GameAppWindow::updateFunc() {
        float32 dt = SoraCore::Instance()->getDelta();
        mFSMManager.onUpdate(dt);
        return false;
    }
    
    bool SoraGameApp::GameAppWindow::renderFunc() {
        mFSMManager.onRender();
        return false;
    }
    
    void SoraGameApp::GameAppWindow::init() {
        
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowWidth() { 
        return mDef.WindowWidth;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowHeight() { 
        return mDef.WindowHeight;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowPosX() { 
        return mDef.WindowPosX;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowPosY() { 
        return mDef.WindowPosY;
    }
    
    SoraString SoraGameApp::GameAppWindow::getWindowName() { 
        return mDef.WindowTitle;
    }
    
    SoraString SoraGameApp::GameAppWindow::getWindowId() { 
        return mDef.WindowId; 
    }
    
    bool SoraGameApp::GameAppWindow::isWindowed() { 
        return mDef.WindowMode; 
    }
    
    bool SoraGameApp::GameAppWindow::hideMouse() { 
        return mDef.HideMouse;
    }
    
    SoraGameApp::SoraGameApp(const SoraGameAppDef& def) {
        if(def.Window == 0)
            mWindow = new SoraGameApp::GameAppWindow(def, mFSMManager);
        else
            mWindow = def.Window;
    }
    
    void SoraGameApp::run(const std::string& initState) {
        GameStatePtr state = getState(initState);
        if(state) {
            // first create main window
            SoraCore::Instance()->createWindow(mWindow);
            
            // then init the first state
            mFSMManager.switchToState(initState);
            
            // app starts!
            SoraCore::Instance()->start();
        } else
            THROW_SORA_EXCEPTION(SystemException, "Cannot find initial state");
    }
    
    void SoraGameApp::addState(GameStatePtr state, const std::string& tag) {
        state->setGameApp(this);
        mFSMManager.defState(state, tag);
    }
    
    GameStatePtr SoraGameApp::getState(const std::string& tag) const {
        GameStatePtr state = static_cast<SoraGameState*>(mFSMManager[tag]);
        return state;
    }
    
    SoraWindow* SoraGameApp::getWindow() const {
        return mWindow;
    }
    
    void SoraGameApp::setState(const std::string& tag) {
        mFSMManager.switchToState(tag);
    }
    
    void SoraGameApp::defStateTrans(const std::string& state1, const SoraFSMManager::EventType& type, const std::string& state2) {
        mFSMManager.defTrans(state1, type, state2);
    }
    
    void SoraGameApp::postEvent(const SoraFSMManager::EventType& evt) {
        mFSMManager.postEvent(evt);
    }
    
    void SoraGameApp::BeginScene(uint32 c, SoraTargetHandle h, bool clear) {
        SoraCore::Instance()->beginScene(c, h, clear);
    }
    
    void SoraGameApp::EndScene() {
        SoraCore::Instance()->endScene();
    }
    
} // namespace sora