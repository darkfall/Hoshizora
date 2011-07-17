//
//  SoraMainWindow.cpp
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#include "SoraMainWindow.h"
#include "SoraCore.h"

bool MainWindow::updateFunc() {
    return false;
}

bool MainWindow::renderFunc() {
    return false;
}

void MainWindow::init() {
    sora::SORA->setFPS(60);
}