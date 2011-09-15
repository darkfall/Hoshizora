/*
 * Code that sets up an HGE application with Guichan using the 
 * Guichan HGE back end.
 */
#include "SoraCore.h"

#include "SoraHGERenderer/SoraHGERenderer.h"
#include "SoraHGERenderer/SoraHGEInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraZipResourceManager/SoraZipResourcemanager.h"
#include "SoraGUIChan/guichansetup.h"
#include "../../SoraParticleEditor/peMainWindow.h"

namespace soratest
{
	void myQueryFunc(float w, float h) {
		sora::log_mssg(sora::vamssg("resop: %f, %f", w, h));
	}
    void run()
    {
        sora::SORA->start();
    }

    void init()
    {
		sora::SORA->registerRenderSystem(new sora::SoraHGERenderer);
		sora::SORA->registerInput(new sora::SoraHGEInput);
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->registerFontManager(new sora::SoraFTFontManager);
		
		float w,  h;
		sora::SORA->getDesktopResolution(&w, &h);
		sora::log_mssg(sora::vamssg("reso: %f, %f", w, h));
		sora::SORA->setQueryVideoModeCallback(myQueryFunc);

		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
		

		peMainWindowLoader::Instance()->loadConfig();
		sora::SORA->createWindow(new peMainWindow);
	}

    void halt()
    {
     
       
    }
}
