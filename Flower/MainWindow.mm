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
            
        sora::SoraiOSInitializer::Instance()->enableAccelerometer(true);
        
        if(!ThemeController::Instance()->loadConfig("ThemeConfig.xml")) {
            sora::SoraCore::Instance()->messageBox("error loading theme confog", "error", MB_OK);
        }
        if(!ObjectController::Instance()->loadConfig("ItemConfig.xml")) {
            sora::SoraCore::Instance()->messageBox("error loading item config", "error", MB_OK);
        }
        ThemeController::Instance()->setCurrentTheme("basic");
        
        ConfigController::LoadConfig("config.xml");
        PlayerController::Instance()->loadConfig("player.xml");
        
        HudController::Instance()->setFont("STHeitiSC-Medium", 20);
        
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