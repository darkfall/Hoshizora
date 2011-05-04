#ifndef SORA_INTERNAL_CORE
#define SORA_INTERNAL_CORE

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraAutoPtr.h"
#include "SoraException.h"

#include "SoraRenderSystem.h"
#include "SoraResourceManager.h"
#include "SoraInput.h"
#include "SoraMiscTool.h"
#include "SoraPluginManager.h"
#include "SoraTimer.h"
#include "SoraFontManager.h"
#include "SoraSoundSystem.h"
#include "SoraResourceFileFinder.h"
#include "SoraShader.h"
#include "SoraFrameListener.h"

#include <map>

namespace sora {

	class SoraCore: public SoraSingleton<SoraCore> {
	protected:
		friend class SoraSingleton<SoraCore>;

		SoraCore();
		~SoraCore() { }

	public:
		void SORACALL start();
		void SORACALL shutDown();

		void SORACALL update();
		bool SORACALL isActive();

		void SORACALL registerRenderSystem	(SoraRenderSystem* pRenderSystem);
		void SORACALL registerResourceManager(SoraResourceManager* pResourceManager);
		void SORACALL registerInput			(SoraInput* pInput);
		void SORACALL registerMiscTool		(SoraMiscTool* pMiscTool);
		void SORACALL registerPluginManager  (SoraPluginManager* pPluginManager);
		void SORACALL registerTimer			(SoraTimer* pTimer);
		void SORACALL registerFontManager	(SoraFontManager* pFontManager);
		void SORACALL registerSoundSystem	(SoraSoundSystem* pSoundSystem);

		void SORACALL registerPlugin		(SoraPlugin* pPlugin);
		void SORACALL unistallPlugin		(SoraPlugin* pPlugin);
		void SORACALL unistallPluginS	(const SoraString& sPluginName);
		SoraPlugin* SORACALL getPlugin	(const SoraString& sPluginName);

		void	SORACALL setFPS(int32 fps);
		float32 SORACALL getFPS();
		float32 SORACALL getDelta();
		float32 SORACALL getTime();
		int32	SORACALL getFrameCount();
		void	SORACALL setTimeScale(float32 scale);
		float32 SORACALL getTimeScale();
		s_int64 SORACALL getCurrentSystemTime();

		// render system APIs
		void SORACALL beginScene(ulong32 c=0, ulong32 h=0);
		void SORACALL endScene();

		HSORATARGET SORACALL createTarget(int width, int height, bool zbuffer=true);
		void	SORACALL freeTarget(ulong32 t);
		HSORATEXTURE SORACALL getTargetTexture(ulong32 t);

		HSORATEXTURE SORACALL createTexture	(const SoraWString& sTexturePath, bool bCache=true, bool bMipmap=false);
		HSORATEXTURE SORACALL createTextureWH(int32 w, int32 h);
		HSORATEXTURE SORACALL createTextureFromRawData(uint32* data, int32 w, int32 h);
		HSORATEXTURE SORACALL createTextureFromMem(void* data, ulong32 size);
		ulong32*	 SORACALL textureLock(HSORATEXTURE);
        void		 SORACALL textureUnlock(HSORATEXTURE);
		void		 SORACALL releaseTexture(HSORATEXTURE pTexture);
        void         SORACALL clearTextureMap();

        SoraShaderContext* SORACALL createShaderContext();
		void SORACALL attachShaderContext(SoraShaderContext* context);
		void SORACALL detachShaderContext();
        SoraShader* SORACALL createShader(const SoraWString& file, const SoraString& entry, SORA_SHADER_TYPE type);

		SoraSprite* SORACALL createSprite (const SoraWString& sPath);
		SoraSprite* SORACALL createSpriteTex(HSORATEXTURE tex);

		void SORACALL renderQuad(SoraQuad& quad);
		void SORACALL renderTriple(SoraTriple& trip);

		void SORACALL renderRect		(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth=1.f, DWORD color=0xFFFFFFFF, float32 z=0.0f);
		void SORACALL setClipping	(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void SORACALL setTransform	(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f);
		void SORACALL setTransformWindowSize (float32 w, float32 h);

		ulong32 SORACALL getMainWindowHandle();
		SoraWindowInfoBase* SORACALL getMainWindow();

		void SORACALL enableMessageBoxErrorPost(bool bFlag) { bMessageBoxErrorPost = bFlag; }

		void	SORACALL setRandomSeed(int32 seed);
		int32	SORACALL getRandomSeed();
		int32	SORACALL randomInt	(int32 min, int32 max);
		float32 SORACALL randomFloat	(float32 min, float32 max);
		/* random number generators using SFMT */
		int32	SORACALL _SFMTRandomInt(int32 min, int32 max);
		float32 SORACALL _SFMTRandomFloat(float32 min, float32 max);
		/*generates a int32 random number using SFMT*/
		int32	SORACALL _SFMTRandomInt32();
		/*generates a float32 random number range [0, 1] using SFMT*/
		float32 SORACALL _SFMTRandomFloat32();

		int32 SORACALL getScreenWidth();
		int32 SORACALL getScreenHeight();

		HSORARESOURCE SORACALL loadResourcePack	(const SoraWString& file);
		void	SORACALL attachResourcePack		(HSORARESOURCE pfile);
		void	SORACALL detachResourcePack		(HSORARESOURCE handle);
		void*	SORACALL getResourceFile		(const SoraWString& file, ulong32& size);
		void*	SORACALL readResourceFile		(const SoraWString& file, ulong32 size);
		ulong32 SORACALL getResourceFileSize	(const SoraWString& file);
		void	SORACALL freeResourceFile		(void* p);
		void	SORACALL enumFilesInFolder		(std::vector<SoraWString>& cont, const SoraWString& folder);

		bool SORACALL isMainWindowSet() { return bMainWindowSet; }
		ulong32 SORACALL createWindow(SoraWindowInfoBase* info);
		void SORACALL setWindowSize(int32 w, int32 h);
		void SORACALL setWindowTitle(const SoraWString& title);
		void SORACALL setWindowPos(int32 px, int32 py);
		void SORACALL setFullscreen(bool flag);
		bool SORACALL isFullscreen();

		// inputs
		void	SORACALL getMousePos(float32 *x, float32 *y);
		float32 SORACALL getMousePosX();
		float32 SORACALL getMousePosY();
		void	SORACALL setMousePos(float32 x, float32 y);
		int		SORACALL getMouseWheel();
		bool	SORACALL isMouseOver();
		bool	SORACALL keyDown(int32 key);
		bool	SORACALL keyUp(int32 key);
		int32	SORACALL getKeyState(int32 key);
		char*	SORACALL getKeyName(int32 key);
		bool	SORACALL getKeyEvent(SoraKeyEvent& ev);
		bool	SORACALL joyKeyPressed(int32 key);
		bool	SORACALL joyKeyDown(int32 key);
		bool	SORACALL joyKeyUp(int32 key);
		bool	SORACALL joyKeyState(int32 key, unsigned char state);
		bool	SORACALL setJoyKey(int32 key);
		bool	SORACALL hasJoy();

		int32	SORACALL messageBox	(const SoraString& sMssg, const SoraString& sTitle, int32 iCode);
		int32	SORACALL messageBoxW (const SoraWString& sMssg, const SoraWString& sTitle, int32 iCode);
		void	SORACALL log		(const SoraString& sMssg);
		void	SORACALL logw		(const SoraWString& sMssg);
		void	SORACALL logf		(const char* str, ...);

		SoraFont* SORACALL createFont(const SoraWString& fontName, int size);
		
		SoraMusicFile* SORACALL createMusicFile(const SoraWString& musicName, bool bStream=true);
		SoraSoundEffectFile* SORACALL createSoundEffectFile(const SoraWString& se);
        SoraMusicFile* SORACALL createMusicFile(bool bStream=false);
        SoraSoundEffectFile* SORACALL createSoundEffectFile();
        
        void SORACALL setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f);
        void SORACALL execute(const SoraString& appPath, const SoraString& args);
        void SORACALL snapshot(const SoraString& path);
		/*
		 for directX, return (ulong32)(pD3DDevice)
		 for ogl, just return (ulong32)(this)
		 */
		ulong32		SORACALL getVideoDeviceHandle();
		SoraWString SORACALL videoInfo();
		void		SORACALL flush();

		void SORACALL postError	(const SoraString& sMssg);

		/*
		 if enabled
		 getDelta would always return 1.f
		 which suggest all caculation based on delta would sync by frame
		 */
		void SORACALL setFrameSync(bool flag);

		/* frame listener functions */
		void SORACALL addFrameListener(SoraFrameListener* listener);
		void SORACALL delFrameListener(SoraFrameListener* listener);

	private:

		inline void _checkCoreComponents();
		inline void _postError(const SoraString& str);
		inline void _logInternalLog();
		inline void _initializeTimer();
		inline void _initializeMiscTool();

		SoraMiscTool*			pMiscTool;
		SoraRenderSystem*		pRenderSystem;
		SoraResourceFileFinder*	pResourceFileFinder;
		SoraInput*				pInput;
		SoraPluginManager*		pPluginManager;
		SoraTimer*				pTimer;
		SoraFontManager*		pFontManager;
		SoraSoundSystem*		pSoundSystem;

		bool bMainWindowSet;
		bool bMessageBoxErrorPost;
		bool bInitialized;
		bool bHasInput;

		int32 iScreenWidth, iScreenHeight;

        bool bMainScene;
		bool bFrameSync;
		float32 time;

		SoraWindowInfoBase* mainWindow;

		typedef std::list<SoraFrameListener*> FRAME_LISTENER_CONT;
		FRAME_LISTENER_CONT frameListeners;

		inline void _frameListenerStart();
		inline void _frameListenerEnd();
        
        SoraShaderContext* shaderContext;
	};


#define SORA SoraCore::Instance()

} // namespace sora

#endif
