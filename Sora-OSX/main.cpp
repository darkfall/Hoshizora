#include <iostream>


#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraRenderTarget.h"

#include "SoraFont.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraQTSoundSystem/SoraQTSoundSystem.h"

#include "testMainWindow.h"
#include "rfMainWindow.h"
#include "mainWindow.h"

#include "SoraLocalizer.h"


#include "TemplateTransformer.h"

#include "Experimental/state_test.h"
#include "SoraMemoryFile.h"

#include "vlcTestWindow.h"

#include "SoraParticleEditor/peMainWindow.h"

#include "SoraGUIChan/XmlGui.h"
#include "SoraGUIChan/Jsongui.h"

#include "Debug/SoraAutoProfile.h"

#include "../AnimationEditor/aeMainWindow.h"


#include "../ReflectionRev/rfMirror.h"

#include "GLFW3/SoraOGLInputGLFW3.h"
#include "GLFW3/SoraOGLRenderer.h"
#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"


int main(int argc, char* argv[]) {
//	sora::tcmalloc_test::test();
	sora::SoraCore* sora = sora::SoraCore::Instance();
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerInput(new sora::SoraOGLInput);
    sora->registerSoundSystem(new sora::SoraFMODSoundSystem);
	
	sora::HSORARESOURCE res = sora->loadResourcePack(L"./data.rfResource");
	if(!res) {
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONERROR);
		sora->shutDown();
	}
	sora->attachResourcePack(res);
    
	/*
	{
		sora::SoraMemoryFile file1(L"box.png");
		sora::SoraMemoryFile file2(L"box.png");
		sora::SoraMemoryFile file3(L"boxhl.png");
		sora::SoraMemoryFile file4(L"box2.png");
		sora::SoraMemoryFile file5(L"boxhl.png");
		sora::SoraMemoryFile file6(L"boxhl.png");
		
		sora::SoraMemoryBuffer serializeBuffer;
		// alloc 4kb data
		serializeBuffer.alloc(4096);
		file1.serialize(serializeBuffer);
		file2.serialize(serializeBuffer);
		file3.serialize(serializeBuffer);
		file4.serialize(serializeBuffer);
		file5.serialize(serializeBuffer);
		file6.serialize(serializeBuffer);
		
		serializeBuffer.writeToFile("test.serialized");
	}
	{
		ulong32 size;
		void* pdata = sora::SORA->getResourceFile(L"test.serialized", size);
		if(size && pdata) {
			sora::SoraMemoryBuffer serialzeData(pdata, size);
			sora::SoraMemoryFile file1; file1.unserialize(serialzeData);
			sora::SoraMemoryFile file2; file2.unserialize(serialzeData);
			sora::SoraMemoryFile file3; file3.unserialize(serialzeData);
			sora::SoraMemoryFile file4; file4.unserialize(serialzeData);
			sora::SoraMemoryFile file5; file5.unserialize(serialzeData);
			sora::SoraMemoryFile file6; file6.unserialize(serialzeData);

		}
	}*/
	
	/*
	sora::SoraLocalizer* localizer = sora::SoraLocalizer::Instance();
	localizer->addLocaleConf(L"en.locale");
	localizer->addLocaleConf(L"cn.locale");
	*/
	sora->setFPS(60.0);
	sora->createWindow(new mainWindow);
	sora->start();
		
	return 0;
}
