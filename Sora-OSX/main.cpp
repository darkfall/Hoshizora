#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"

#include "SoraEventFactory.h"

#include "../UnitTest/TestCases/Test_EventFactory.h"
#include "../UnitTest/TestCases/Test_EventWorld.h"
#include "../UnitTest/UnitTestSuite.h"

#include "mainWindow.h"

#include "SoraTimestamp.h"

#include "message/SoraMessageRouter.h"
#include "../SoraParticleEditor/peMainWindow.h"

#include "SoraConfigUtil.h"

class consoleWindow: public sora::SoraWindowInfoBase {
public:
	consoleWindow() {}
	~consoleWindow() {}
	
	bool updateFunc() {}
	bool renderFunc() {}
	void init() {} 
	
	int32 getWindowWidth() { return 1024; }
	int32 getWindowHeight() { return 768; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Reflection"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
    
    bool isConsoleApp() { return true; }
};

#include "HashStringMap.h"
#include "SoraDirectoryIterator.h"
#include "debug/SoraAutoProfile.h"
#include "SoraFileUtility.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"

class initState: public sora::SoraGameState {
public:
    void onRender() {
        
    }
};

int main(int argc, char* argv[]) {    
    
    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    app.addState(new initState, "init");
    app.run("init");
    
    return 0;

}

void oldInit() {
    
    sora::InitAndCreateSoraCore(new mainWindow, 
                                sora::SoraCoreParameter(/* load plugins */
                                                        false,
                                                        /* use fullscreen buffer */
                                                        false,
                                                        /* post error using messagebox */
                                                        false,
                                                        /* seperate sound engine thread */
                                                        true,
                                                        /* debug rendering */
                                                        false))->start();
    

}