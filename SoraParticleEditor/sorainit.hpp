/*
 * Code that sets up an HGE application with Guichan using the 
 * Guichan HGE back end.
 */
#include "SoraCore.h"

#include "MainWindow.h"

#include "SoraHGERenderer/SoraHGERenderer.h"
#include "SoraHGERenderer/SoraHGEInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraZipResourceManager/SoraZipResourcemanager.h"
#include "SoraGUIChan/guichansetup.h"

namespace soratest
{
    void run()
    {
        sora::SORA->start();
    }

    void init()
    {
#ifdef WIN32
		if(CoInitialize(0) != S_OK)
			sora::SORA->messageBox("Initialize COM failed", "Error", MB_OK | MB_ICONERROR);
#endif

		sora::SORA->registerRenderSystem(new sora::SoraHGERenderer);
		sora::SORA->registerInput(new sora::SoraHGEInput);
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->registerFontManager(new sora::SoraFTFontManager);
		
		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
		

		sora::SORA->createWindow(new MainWindow);
	}

    void halt()
    {
     
       
    }
}
