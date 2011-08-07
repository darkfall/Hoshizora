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
#include "SoraEnvValues.h"
#include "SoraResourceFile.h"
#include "SoraDelegate.h"
#include "SoraInputListeners.h"
#include "SoraHotkey.h"

#include "timer/ITimerManager.h"
#include "Debug/SoraInternalLogger.h"

#include <map>

namespace sora {
		
	class SORA_API SoraCore: public SoraEventHandler {
	protected:
		SoraCore();
		~SoraCore() { }

	public:
        static SoraCore* Instance();
        static void Destroy();
        
        class Parameter {
        public:
            Parameter(bool loadPlugins=true, bool renderToBuffer=false, bool postErrorByMessageBox=false):
            mLoadPlugins(loadPlugins),
            mRenderToBuffer(renderToBuffer),
            mMesageBoxErrorPost(postErrorByMessageBox) {
                
            }
            
            void setLoadPlugin(bool flag) { mLoadPlugins = flag; }
            void setRenderToBuffer(bool flag) { mRenderToBuffer = flag; }
            void setPostErrorByMessageBox(bool flag) { mMesageBoxErrorPost = flag; }
            
            bool isLoadPlugins() const { return mLoadPlugins; }
            bool isRenderToBuffer() const { return mRenderToBuffer; }
            bool isPostErrorByMessageBox() const { return mMesageBoxErrorPost; }
            
            
        private:
            bool mLoadPlugins;
            bool mRenderToBuffer;
            bool mMesageBoxErrorPost;
        };
        
        void init(const Parameter& param = Parameter());
        
		void start();
		void shutDown();

		void update();
		bool isActive();
		
		void pause(bool bPauseRender=true, bool bPauseSound=true);
		void resume();
		bool isPaused();

		void registerRenderSystem	(SoraRenderSystem* pRenderSystem);
		void registerResourceManager(SoraResourceManager* pResourceManager);
		void registerInput			(SoraInput* pInput);
		void registerFontManager	(SoraFontManager* pFontManager);
		void registerSoundSystem	(SoraSoundSystem* pSoundSystem);
        
		void registerMiscTool		(SoraMiscTool* pMiscTool);
		void registerPluginManager  (SoraPluginManager* pPluginManager);
		void registerTimer			(SoraTimer* pTimer);
            
        SoraRenderSystem*   getRenderSystem() const;
        SoraInput*          getInput() const;
        SoraMiscTool*       getMiscTool() const;
        SoraPluginManager*  getPluginManager() const;
        SoraTimer*          getTimer() const;
        SoraFontManager*    getFontManager() const;
        SoraSoundSystem*    getSoundSystem() const;
        
		void registerPlugin         (SoraPlugin* pPlugin);
		void unistallPlugin         (SoraPlugin* pPlugin);
		void unistallPluginS		(const SoraString& sPluginName);
		SoraPlugin* getPlugin		(const SoraString& sPluginName);
        
        bool  isRenderSystemExtensionEnabled(int32 extension);
        void  enableRenderSystemExtension(int32 extesion);
        void  disableRenderSystemExtension(int32 extension);
        void  setRenderSystemExtensionParam(int32 extension, int32 param);
        int32 getRenderSystemExtensionParam(int32 extension);

		void	setFPS(int32 fps);
		float32 getFPS();
		float32 getDelta();
        float32 getAbsoluteDelta();
		float32 getTime();
		int32	getFrameCount();
		void	setTimeScale(float32 scale);
		float32 getTimeScale();
		uint64  getCurrentSystemTime();
        void    setVerticalSync(bool flag);
        
        SimpleTimerPtr createTimer(const ITimerManager::TimerFunc& func);

		// render system APIs
		void beginScene(uint32 c=0xFF000000, ulong32 h=0, bool clear=true);
		void endScene();

		HSORATARGET     createTarget(int width, int height, bool zbuffer=true);
		void            freeTarget(ulong32 t);
		HSORATEXTURE    getTargetTexture(ulong32 t);

		HSORATEXTURE createTexture(const SoraWString& sTexturePath, bool bCache=true, bool bMipmap=false);
		HSORATEXTURE createTextureWH(int32 w, int32 h);
		HSORATEXTURE createTextureFromRawData(uint32* data, int32 w, int32 h);
		HSORATEXTURE createTextureFromMem(void* data, ulong32 size);
		uint32*		 textureLock(HSORATEXTURE);
        void		 textureUnlock(HSORATEXTURE);
		int32		 getTextureWidth(HSORATEXTURE, bool origin=false);
		int32		 getTextureHeight(HSORATEXTURE, bool origin=false);
		// depends on render system, may have different meaning
		ulong32		 getTextureId(HSORATEXTURE);
		void		 releaseTexture(HSORATEXTURE pTexture);
        void         clearTextureMap();

        SoraShaderContext*  createShaderContext();
		void                attachShaderContext(SoraShaderContext* context);
		void                detachShaderContext();

		SoraSprite* createSprite (const SoraWString& sPath);

		void renderQuad(SoraQuad& quad);
		void renderTriple(SoraTriple& trip);
		void renderWithVertices(HSORATEXTURE tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode=SORA_TRIANGLES);

		void renderRect     (float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth=1.f, uint32 color=0xFFFFFFFF, float32 z=0.0f);
		void renderBox		(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float32 z=0.f);
		void setClipping	(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform	(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f);
		
		void pushTransformMatrix();
		void popTransformMatrix();
        void pushClippingMatrix();
        void popClippingMatrix();
		/*
		 z sort without zbuffer, because there are some problems with transparency when zbuffer is on
		 we don't want our transparency pixels in textures to be omitted when rendering with z
		 implemention for fast sort, accuracy 0.001
		 */
		void beginZBufferSort();
		void endZBufferSort();

		/*
		 under windows, this would return HWND, 
		 under other plaforms, just return (ulong32)mainWindow
		 */
		ulong32 getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();

		void enableMessageBoxErrorPost(bool bFlag);

		/*generates a int32 random number using SFMT*/
		void	setRandomSeed(int32 seed);
		int32	getRandomSeed();
		int32	randomInt(int32 min, int32 max);
		float32 randomFloat(float32 min, float32 max);
		int32	randomIntNoRange();
		/*generates a float32 random number range [0, 1] using SFMT*/
		float32 randomFloatNoRange();

		int32 getScreenWidth();
		int32 getScreenHeight();

		HSORARESOURCE loadResourcePack	(const SoraWString& file);
		void	attachResourcePack		(HSORARESOURCE pfile);
		void	detachResourcePack		(HSORARESOURCE handle);
		void*	getResourceFile         (const SoraWString& file, ulong32& size);
		void*	readResourceFile		(const SoraWString& file, ulong32 size);
		ulong32 getResourceFileSize     (const SoraWString& file);
		void	freeResourceFile		(void* p);
		void	enumFilesInFolder		(std::vector<SoraWString>& cont, const SoraWString& folder);
	
		bool    isMainWindowSet() { return bMainWindowSet; }
		ulong32 createWindow(SoraWindowInfoBase* info);
		void    setWindowSize(int32 w, int32 h);
		void    setWindowTitle(const SoraWString& title);
		void    setWindowPos(int32 px, int32 py);
		void    setFullscreen(bool flag);
		bool    isFullscreen();

		// inputs
		void	getMousePos(float32 *x, float32 *y);
		float32 getMousePosX();
		float32 getMousePosY();
		void	setMousePos(float32 x, float32 y);
		int		getMouseWheel();
		bool	isMouseOver();
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		char*	getKeyName(int32 key);
		bool	getKeyEvent(SoraKeyEvent& ev);
		bool	joyKeyPressed(int32 key);
		bool	joyKeyDown(int32 key);
		bool	joyKeyUp(int32 key);
		bool	joyKeyState(int32 key, unsigned char state);
		bool	setJoyKey(int32 key);
		bool	hasJoy();
        
        /**
         *  register a global hotkey and it's conresponding event handler
         *  @returnval the id of the registered hotkey
         */
        int32   registerGlobalHotkey(const SoraHotkey& key, SoraEventHandler* handler);
        void    unregisterGlobalHotkey(int32 hid);
        void    setGlobalHotkey(int32 hid, const SoraHotkey& key);
        void    clearGlobalHotkeys();
		
		void	simulateKey(int32 key, int32 state);

		int32	messageBox	(const SoraString& sMssg, const SoraString& sTitle, int32 iCode);
		int32	messageBoxW (const SoraWString& sMssg, const SoraWString& sTitle, int32 iCode);
		void	log         (const SoraString& sMssg, int32 level=LOG_LEVEL_NORMAL);
		void	logw		(const SoraWString& sMssg, int32 level=LOG_LEVEL_NORMAL);
		void	logf		(const char* str, ...);
		
		SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);

		SoraFont* createFont(const SoraWString& fontName, int size);
		void releaseFont(SoraFont* font);
		
		SoraMusicFile* 			createMusicFile(const SoraWString& musicName, bool bStream=true);
		SoraSoundEffectFile* 	createSoundEffectFile(const SoraWString& se);
        SoraMusicFile* 			createMusicFile(bool bStream=false);
        SoraSoundEffectFile* 	createSoundEffectFile();
        
        void addMouseListener(SoraMouseListener* listener, int priority=0);
        void addKeyListener(SoraKeyListener* listener, int priority=0);
        void addJoystickListener(SoraJoystickListener* listener, int priority=0);
        
        void delMouseListener(SoraMouseListener* listener);
        void delKeyListener(SoraKeyListener* listener);
        void delJoystickListener(SoraJoystickListener* listener);
        
        
        void setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f);
        void execute(const SoraString& appPath, const SoraString& args);
        void snapshot(const SoraString& path);
		/*
		 for directX, return (ulong32)(pD3DDevice)
		 for ogl, just return (ulong32)(this)
		 */
		ulong32		getVideoDeviceHandle();
		SoraWString videoInfo();
		void		flush();

		void postError(const SoraString& sMssg);

		/*
		 if enabled
		 getDelta would always return 1.f
		 */
		void setFrameSync(bool flag);
 
		/* frame listener functions */
		void addFrameListener(SoraFrameListener* listener);
		void delFrameListener(SoraFrameListener* listener);
		
        /**
         *  Memory usage calculated by Operating System
         *  libproc for Linux
         **/
		uint64 getEngineMemoryUsage() const;
        /**
         *  Memory usage calculated by SoraResourceFileFinder
         *  Only calculates resource allocation and free
         *  (Library managed memory not included, such as gl texture)
         **/
        uint64 getResourceMemoryUsage() const;
		
		void setSystemFont(const wchar_t* font, int32 fontSize);
		
		void setIcon(const SoraString& icon);
		void setCursor(const SoraString& cursor);
		
		void enablePluginDetection(bool flag);
        
        /**
         * Enable Fullscreen buffer would cause everything be renderered to a screen texture buffer then render to screen
         * This is required for fullscreen post shader effects
         **/
        void enableFullscreenBuffer(bool flag);
        void registerFullscreenBufferDelegate(const SoraAbstractDelegate<HSORATEXTURE>& delegate);
		
	private:
        static SoraCore* mInstance;
        
		inline void _registerCoreCmds();
		
		inline void _checkCoreComponents();
		inline void _postError(const SoraString& str);
		inline void _initializeTimer();
		inline void _initializeMiscTool();
        inline void _registerEventTypes();

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
		bool bDisablePluginDetection;
		
		bool bPaused;
		bool bPauseRender;
		bool bPauseSound;

		int32 iRandomSeed;
		int32 iScreenWidth, iScreenHeight;

        bool bMainScene;
		bool bFrameSync;
		float32 mTime;
        float32 mTimeScale;

		SoraWindowInfoBase* mMainWindow;
        SoraShaderContext* mPrevShaderContext;
        
        bool bEnableScreenBuffer;
        bool bScreenBufferAttached;
        ulong32 mScreenBuffer;
        SoraSprite* mScreenBufferSprite;
				
		typedef std::list<SoraFrameListener*> FRAME_LISTENER_CONT;
		FRAME_LISTENER_CONT frameListeners;

		inline void _frameListenerStart();
		inline void _frameListenerEnd();
        inline void _modifierAdapterUpdate();
		
		bool bZBufferArea;
    };


#define SORA SoraCore::Instance()

} // namespace sora

#endif
