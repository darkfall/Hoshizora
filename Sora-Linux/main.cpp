#include "SoraCore.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraZipResourceManager/SoraZipResourceManager.h"

#include "mainWindow.h"

class lmainWindow: public sora::SoraWindowInfoBase {
public:
    bool updateFunc() {
        return false;
    }
	bool renderFunc() {
	    sora::SORA->beginScene();

        pSpr->render(0.f, 0.f);
        if(pFont) {
            SoraWString videoInfo = sora::SORA->videoInfo();
            pFont->render(getWindowWidth()-pFont->getStringWidth(videoInfo.c_str()),
                          getWindowHeight()-pFont->getHeight()-10,
                          sora::FONT_ALIGNMENT_LEFT,
                          videoInfo.c_str());
        }
	    sora::SORA->endScene();
        return false;
    }
	void init() {
        pSpr = sora::SORA->createSprite(L"titlebg2.png");
        pFont = sora::SORA->createFont(L"FreeMono.ttf", 20);
        if(!pFont)
            sora::SORA->log("Error creating font!!!!!!");
        else
            pFont->setColor(sora::COLOR_RED.GetHWColor());
	}

	int32 getWindowWidth() { return 800; }
	int32 getWindowHeight() { return 600; }

	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }

	SoraString getWindowName() { return "HoshiNoSora"; }
	SoraString getWindowId() { return "MainWindow"; }

	bool isWindowSubWindow() { return false; }
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }

private:
	sora::SoraSprite* pSpr;
	sora::SoraFont* pFont;
};

int main() {
    sora::SORA->registerRenderSystem(new sora::SoraOGLRenderer);
    sora::SORA->registerInput(new sora::SoraOGLInput);
    sora::SORA->registerFontManager(new sora::SoraFTFontManager);
    sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);

    ulong32 resource = sora::SORA->loadResourcePack(L"data.rfResource");
    sora::SORA->attachResourcePack(resource);

    sora::SORA->setFPS(60.f);
    sora::SORA->createWindow(new mainWindow);

    sora::SORA->start();
}
