#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"

#include "SoraHGERenderer/SoraHGERenderer.h"
#include "SoraHGERenderer/SoraHGEInput.h"

#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"

#include "function/SoraFunction.h"


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

#include "SoraPath.h"
#include "SoraDirectoryIterator.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow) {


	sora::SoraCore* sora = sora::InitAndCreateSoraCore(new mead::meadMainWindow,
														sora::SoraCoreParameter(/* load plugins */
																				false,
																				/* render to buffer */
																				false,
																				/* message box error post */
																				false));
	sora->setFPS(60);
	sora->start();
		
	return 0;
}
