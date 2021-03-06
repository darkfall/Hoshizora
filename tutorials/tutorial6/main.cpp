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
 *  Hoshizora Tutorial 6
 *  Scene, Layer & Modifier
 **/


#ifndef OS_WIN32

int main(int argv, char** argc) {
    registerComponents();
    startWithWindow(new MainWindow);
    shutdown();
}

#else

#pragma comment(lib, "Hoshizora.lib")

int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow) {
	registerComponents();
    startWithWindow(new MainWindow);
    shutdown();
}

#endif