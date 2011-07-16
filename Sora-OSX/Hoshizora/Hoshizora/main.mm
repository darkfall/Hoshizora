//
//  main.m
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SoraCore.h"
#include "Starter.h"
#include "SoraMainWindow.h"

int main(int argc, char *argv[])
{
    registerComponents();
    startWithWindow(new MainWindow);
    terminate();
}
