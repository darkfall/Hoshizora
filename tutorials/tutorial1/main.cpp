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
 *  Hoshizora Tutorial 1
 *  Create a simple window
 **/

int main(int argv, char** argc) {
    registerComponents();
    startWithWindow(new MainWindow);
    terminate();
}
