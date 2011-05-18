#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraRenderTarget.h"

#include "SoraFont.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"

#include "SoraHGERenderer/SoraHGERenderer.h"
#include "SoraHGERenderer/SoraHGEInput.h"

#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"


#define HGE_RENDERER

//#include "../Sora-OSX/mainWindow.h"
//#include "vlcTestWindow.h"

#include "../AnimationEditor/aeMainWindow.h"

#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

#ifdef OS_WIN32
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#endif

#include "../../mead/meadMainWindow.h"

#include "mainWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow) {
	sora::SoraCore* sora = sora::SoraCore::Instance();
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerSoundSystem(new sora::SoraAudiereSoundSystem);
	
	// remember to change USE_HGE_KEYMAP between USE_GLFW_KEYMAP in SoraPlatform.h when using different renderers 
#ifdef HGE_RENDERER
	sora->registerRenderSystem(new sora::SoraHGERenderer);
	sora->registerInput(new sora::SoraHGEInput);
#else
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerInput(new sora::SoraOGLInput);
#endif

//	sora->registerSoundSystem(new sora::SoraAudiereSoundSystem);
	
	sora->setFPS(60);
	
//	sora->createWindowLua(L"resource/scripts/luamaintest.lua");
	sora->createWindow(new mead::meadMainWindow);
	sora->start();
		
	return 0;
}
