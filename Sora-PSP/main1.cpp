#include <pspkernel.h>
#include <pspdebug.h>

PSP_MODULE_INFO("Hello World", 0, 1, 1);


/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
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

#include "Hoshizora.h"
#include "SoraSprite.h"
#include "SoraPSPGLRenderer.h"

sora::SoraPSPGLRenderer* renderer = new sora::SoraPSPGLRenderer;
sora::SoraSprite* pspr;

int main() {
	//pspDebugScreenInit();
	SetupCallbacks();
	
	/*FILE* texfile = fopen("./test.png", "rb");
	fseek(texfile, 0, SEEK_END);
	ulong32 size = ftell(texfile);
	fseek(texfile, 0, SEEK_SET);
	
	uint8* texdata = new uint8[size];
	if(texdata)
		fread(texdata, size, 1, texfile);
	fclose(texfile);
	
	ulong32 mytex = renderer->createTextureFromMem(texdata, size);
	*/
	pspr = new sora::SoraSprite((sora::HSORATEXTURE)renderer->createTextureWH(100, 100));
	while(1) {
		if(pspr) {
			renderer->beginFrame();
			renderer->beginScene();
			pspr->render(0.f, 0.f);
			renderer->endScene();
			renderer->endFrame();
		}
	}
	
	//printf("Hello World from Sora-PSP(fake)");
	sceKernelSleepThread();
	return 0;
}