//
//  Starter.h
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Hoshizora_Starter_h
#define Hoshizora_Starter_h

#include "SoraCore.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"

static void registerComponents() {
    sora::SoraCore* sora = sora::SoraCore::Instance();
    
    sora->registerRenderSystem(new sora::SoraOGLRenderer);
    sora->registerInput(new sora::SoraOGLInput);
}

static void startWithWindow(sora::SoraWindowInfoBase* window) {
    sora::SoraCore* sora = sora::SoraCore::Instance();

    sora->createWindow(window);
    sora->start();
}

static void terminate() {
    sora::SoraCore::Instance()->shutDown();
}

#endif
