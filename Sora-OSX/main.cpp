#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraZipResourceManager/SoraZipResourceManager.h"

#include "SoraEventFactory.h"

#include "../UnitTest/TestCases/Test_EventFactory.h"
#include "../UnitTest/TestCases/Test_EventWorld.h"
#include "../UnitTest/UnitTestSuite.h"

#include "mainWindow.h"

#include "SoraTimestamp.h"

void myFunc() throw() {
    throw std::bad_cast();
}

void test() {
    try {
        myFunc();
    } catch(...) {
        sora::SORA->messageBox("test", "test", MB_OK);
    }
}

int main(int argc, char* argv[]) {    
    test();
    
    sora::InitAndCreateSoraCore(new mainWindow, 
                                sora::SoraCoreParameter(/* load plugins */
                                                        false,
                                                        /* use fullscreen buffer */
                                                        false,
                                                        /* post error using messagebox */
                                                        false))->start();
		
	return 0;
}
