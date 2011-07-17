//
//  Starter.h
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#ifndef Hoshizora_Starter_h
#define Hoshizora_Starter_h

#include "SoraCore.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"

#include "SoraFreetypeFont/SoraFTFontManager.h"

#ifdef OS_WIN32
#include "SoraHGERenderer/SoraHGERenderer.h"
#include "SoraHGERenderer/SoraHGEInput.h"

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcprt.lib")
#endif

static void registerComponents() {
    sora::SoraCore* sora = sora::SoraCore::Instance();
    
#ifndef OS_WIN32
    sora->registerRenderSystem(new sora::SoraOGLRenderer);
    sora->registerInput(new sora::SoraOGLInput);
#else
    sora->registerRenderSystem(new sora::SoraHGERenderer);
    sora->registerInput(new sora::SoraHGEInput);
#endif
	sora->registerFontManager(new sora::SoraFTFontManager);
}

static void startWithWindow(sora::SoraWindowInfoBase* window) {
    sora::SoraCore* sora = sora::SoraCore::Instance();

    sora->createWindow(window);
    sora->start();
}

static void shutdown() {
    sora::SoraCore::Instance()->shutDown();
}

#endif
