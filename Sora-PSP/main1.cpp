#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <pspsdk.h> 

#include "Hoshizora.h"
#include "SoraSprite.h"
#include "SoraPSPGLRenderer.h"



PSP_MODULE_INFO("Sora", 0, 1, 1);

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
	sora::SORA->shutDown();
	
    sceKernelExitGame();
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
	int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;

    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0,

0);
    if(thid >= 0) {
         sceKernelStartThread(thid, 0, 0);
	}

    return thid;
}

sora::SoraPSPGLRenderer* renderer = new sora::SoraPSPGLRenderer;

#include "JGE.h"
#include "JRenderer.h"

class mainWindow: public sora::SoraWindowInfoBase {
public:
	mainWindow() { }
	~mainWindow() {}
	
	bool updateFunc() {
		
	}
	bool renderFunc() {
	//	sora::SORA->beginScene();
		JRenderer::GetInstance()->RenderQuad(quad, 0, 0);
		//JRenderer::GetInstance()->RenderQuad(quad2, 0, 0);

	//	sora::SORA->endScene();
	}
	void init() {
		jtex = JRenderer::GetInstance()->LoadTexture("sora.png");
		quad = new JQuad(jtex, 0, 0, 376, 272);
		
	}
	
	int32 getWindowWidth() { return 480; }
	int32 getWindowHeight() { return 272; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Reflection"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
		
private:
	JTexture* jtex ;
	JTexture* jtex2 ;
	JTexture* jtex3 ;
	JQuad* quad;
	JQuad* quad1;
	JQuad* quad2;
	sora::SoraCore* sora;
};

#include "Debug/SoraInternalLogger.h"

#include <pspdisplay.h>

#include "SoraJGEApp.h"
#include "jge.h"
int main() {
	//pspDebugScreenInit();
	SetupCallbacks();
	
	sora::SORA->registerRenderSystem(renderer);
	sora::SORA->createWindow(new mainWindow);
	sora::SORA->start();
	
	sora::SORA->shutDown();
//	sora::SORA->createWindow(new mainWindow);
	//sora::SORA->start();
	
	//printf("Hello World from Sora-PSP(fake)");
	sceKernelExitGame();
	return 0;
}