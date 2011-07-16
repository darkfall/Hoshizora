//
//  main.cpp
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#include "SoraCore.h"
#include "Starter.h"
#include "SoraMainWindow.h"

/**
 *  Hoshizora Tutorial 3
 *  Font Rendering
 **/

int main(int argv, char** argc) {
    try {
        registerComponents();
        startWithWindow(new MainWindow);
        terminate();
    } catch(const sora::SoraException& exp) {
        sora::SORA->messageBox(exp.what(), "Fatal Error :(", MB_OK | MB_ICONERROR);
    }
}
