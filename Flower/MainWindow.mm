//
//  MainWindow.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "MainWindow.h"

#include "StateMenu.h"
#include "SoraCore.h"

#include "controller/ThemeController.h"
#include "controller/ObjectController.h"
#include "controller/ConfigController.h"
#include "controller/PlayerController.h"
#include "controller/HudController.h"

#include "SoraiOSGLRenderer/SoraiOSInitializer.h"

namespace flower {
    
    void MainWindow::init() {
        mFSManager.defState(new StateMenu, "menu");
        
        sora::SoraiOSInitializer::Instance()->setOrientation(sora::ORIENTATION_LANDSCAPE_LEFT);
        sora::SoraiOSInitializer::Instance()->enableOrientationChange(false);
        
        sora::SoraiOSInitializer::Instance()->enableAccelerometer(true);
        
        if(!ThemeController::Instance()->loadConfig(L"ThemeConfig.xml")) {
            sora::SoraCore::Instance()->messageBox("error loading theme confog", "error", MB_OK);
        }
        if(!ObjectController::Instance()->loadConfig(L"ItemConfig.xml")) {
            sora::SoraCore::Instance()->messageBox("error loading item config", "error", MB_OK);
        }
        ThemeController::Instance()->setCurrentTheme("basic");
        
        ConfigController::LoadConfig(L"config.xml");
        PlayerController::Instance()->loadConfig(L"player.xml");
        
        HudController::Instance()->setFont(L"Courier", 20);
        
        mFSManager.switchToState("menu");

    }
    
    bool MainWindow::renderFunc() {
        mFSManager.onRender();
        return false;
    }
    
    bool MainWindow::updateFunc() {
        mFSManager.onUpdate(sora::SoraCore::Instance()->getDelta());
        return false;
    }
    
    void MainWindow::applicationWillResignActive() {
        
    }
    
    void MainWindow::applicationDidBecomeActive() {
        
    }
    
} // namespace flower