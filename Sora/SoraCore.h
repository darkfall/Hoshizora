#ifndef SORA_INTERNAL_CORE
#define SORA_INTERNAL_CORE

#include "SoraPlatform.h"
#include "SoraString.h"
#include "SoraMath.h"
#include "SoraKeyInfo.h"
#include "SoraResourceFileFinder.h"
#include "SoraVertex.h"
#include "function/SoraFunction.h"

#include <map>
#include <list>

namespace sora {
    
    class SoraSprite;
    class SoraInput;
    class SoraRenderSystem;
    class SoraPluginManager;
    class SoraResourceManager;
    class SoraShader;
    class SoraShaderContext;
    class SoraFrameListener;
    class SoraFontManager;
    class SoraTimer;
    class SoraMiscTool;
    class SoraSoundSystem;
    class SoraEventHandler;
    class SoraMusicFile;
    class SoraSoundEffectFile;
    class SoraPlugin;
    class SoraHotkey;
    class SoraFont;
    class SoraWindowInfoBase;
    class SoraMutexLock;
    class SoraInputListener;
    
	class SORA_API SoraCore {
	protected:
		SoraCore();
		~SoraCore() { }

	public:
        static SoraCore* Instance();
        
        struct Feature {
        public:
            Feature(bool loadPlugins=true, bool renderToBuffer=false, bool postErrorByMessageBox=false, bool seperateSSthread=false, bool debugRender=false):
            LoadPlugins(loadPlugins),
            RenderToBuffer(renderToBuffer),
            MessageBoxErrorPost(postErrorByMessageBox),
            SeperateSoundSystemThread(seperateSSthread), 
            DebugRender(debugRender) {
                
            }
            
            bool LoadPlugins;
            bool RenderToBuffer;
            bool MessageBoxErrorPost;
            bool SeperateSoundSystemThread;
            bool DebugRender;
        };
        
        void init(const Feature& param = Feature());
        
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
        
		void        registerPlugin  (SoraPlugin* pPlugin);
		SoraPlugin* unistallPlugin  (SoraPlugin* pPlugin);
		SoraPlugin* unistallPlugin  (const SoraString& sPluginName);
		SoraPlugin* getPlugin		(const SoraString& sPluginName);
        
        bool  isRenderSystemExtensionEnabled(int32 extension);
        void  enableRenderSystemExtension(int32 extesion);
        void  disableRenderSystemExtension(int32 extension);
        void  setRenderSystemExtensionParam(int32 extension, int32 param);
        int32 getRenderSystemExtensionParam(int32 extension);

		void	setFPS(int32 fps);
		float   getFPS();
		float   getDelta();
        float   getAbsoluteDelta();
		float   getTime();
		int32	getFrameCount();
		void	setTimeScale(float scale);
		float   getTimeScale();
		uint64  getCurrentSystemTime();
        void    setVerticalSync(bool flag);

		// render system APIs
		void beginScene(uint32 c=0xFF000000, SoraTargetHandle h=0, bool clear=true);
		void endScene();

		SoraTargetHandle     createTarget(int width, int height, bool zbuffer=true);
		void                 freeTarget(ulong32 t);
		SoraTextureHandle    getTargetTexture(ulong32 t);

		SoraTextureHandle   createTexture(const StringType& sTexturePath, bool bCache=true, bool bMipmap=false);
		SoraTextureHandle   createTextureWH(int32 w, int32 h);
		SoraTextureHandle   createTextureFromRawData(uint32* data, int32 w, int32 h);
		SoraTextureHandle   createTextureFromMem(void* data, ulong32 size);
		uint32*             textureLock(SoraTextureHandle);
        void                textureUnlock(SoraTextureHandle);
		// depends on render system, may have different meaning
		ulong32             getTextureId(SoraTextureHandle);
		void                releaseTexture(SoraTextureHandle);
        void                clearTextureMap();

        SoraShaderContext*  createShaderContext();
		void                attachShaderContext(SoraShaderContext* context);
		void                detachShaderContext();

		SoraSprite* createSprite (const StringType& sPath);

		void renderQuad(SoraQuad& quad);
		void renderTriple(SoraTriple& trip);
		void renderWithVertices(SoraTextureHandle tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode=SORA_TRIANGLES);

        void renderLine     (float x1, float y1, float x2, float y2, uint32 color, float width=1.f, float z=0.f);
		void renderBox		(float x1, float y1, float x2, float y2, uint32 color, float lineWidth=1.f, float z=0.f);
        void fillBox        (float x1, float y1, float x2, float y2, uint32 color, float z=0.f);
        
		void setClipping	(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform	(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=1.f, float vscale=1.f);
		
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
        
        /**
         * Core Feature enums
         **/
        enum CoreFeatures {
            FeatureLoadPlugin = 1,
            FeatureRenderToBuffer,
            FeatureMessageBoxErrorPost,
            FeatureSeperateSoundSystem,
            FeatureDebugRendering,
        };
        
        void enableCoreFeature(CoreFeatures feature);
        void disableCoreFeature(CoreFeatures feature);

		/*generates a int32 random number using SFMT*/
		static void     setRandomSeed(int32 seed);
		static int32    getRandomSeed();
		static int32    randomInt(int32 min, int32 max);
		static float    randomFloat(float min, float max);
		static int32    randomIntNoRange();
		/*generates a float random number range [0, 1] using SFMT*/
		static float    randomFloatNoRange();
                
        /**
         *  register a global hotkey and it's conresponding event handler
         *  @returnval the id of the registered hotkey
         */
        static int32   registerGlobalHotkey(const SoraHotkey& key, SoraEventHandler* handler);
        static void    unregisterGlobalHotkey(int32 hid);
        static void    setGlobalHotkey(int32 hid, const SoraHotkey& key);
        static void    clearGlobalHotkeys();
		
		static void    simulateKey(int32 key, int32 state);


		int32 getScreenWidth();
		int32 getScreenHeight();

		SoraResourceHandle  loadResourcePack        (const StringType& file);
		void                detachResourcePack		(SoraResourceHandle handle);
		void*               getResourceFile         (const StringType& file, ulong32& size);
		void*               readResourceFile		(const StringType& file, ulong32 pos, ulong32 size);
		ulong32             getResourceFileSize     (const StringType& file);
		void                freeResourceFile		(void* p);
		void                enumFilesInFolder		(std::vector<SoraWString>& cont, const StringType& folder);

#ifdef SORA_ENABLE_MULTI_THREAD
        void  loadResourceFileAsync   (const StringType& file, const SoraResourceFileFinder::AsyncNotification& handler, void* puserdata);
        
        /*
         These calls must exist in the main thread
         After multithreadedRendering begins
         It's safe to call rendering functions in another thread
         */
        void beginMultithreadedRendering();
        void endMultithreadedRendering();
#endif
        
		bool    isMainWindowSet();
		bool    createWindow(SoraWindowInfoBase* info);
		void    setWindowSize(int32 w, int32 h);
		void    setWindowTitle(const StringType& title);
		void    setWindowPos(int32 px, int32 py);
		void    setFullscreen(bool flag);
		bool    isFullscreen();

		// inputs
		void	getMousePos(float *x, float *y);
		float   getMousePosX();
		float   getMousePosY();
		void	setMousePos(float x, float y);
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
     
		int32	messageBox  (const StringType& sMssg, const StringType& sTitle, int32 iCode);
		
		StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);

		SoraFont*   createFont(const StringType& fontName, int size);
        SoraFont*   createFont(void* data, ulong32 size, uint32 fontSize, const StringType& fileName);
		void        releaseFont(SoraFont* font);
		
        SoraMusicFile*          createMusicFileFromMemory(void* pdata, ulong32 size);
        SoraSoundEffectFile*    createSoundEffectFileFromMemory(void* pdata, ulong32 size);
		SoraMusicFile* 			createMusicFile(const StringType& musicName, bool bStream=true);
		SoraSoundEffectFile* 	createSoundEffectFile(const StringType& se);
        SoraMusicFile* 			createMusicFile(bool bStream=false);
        SoraSoundEffectFile* 	createSoundEffectFile();
        
        void addInputListener(SoraInputListener* listener, int priority=0);
        void delInputListener(SoraInputListener* listener);
        
        static void execute(const SoraString& appPath, const SoraString& args);
        void snapshot(const SoraString& path);
		/*
		 for directX, return (ulong32)(pD3DDevice)
		 for ogl, just return (ulong32)(this)
		 */
		ulong32		 getVideoDeviceHandle();
		StringType   videoInfo();
		void		 flush();

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
         *  libproc required for Linux
         **/
		uint64 getEngineMemoryUsage() const;
        /**
         *  Memory usage calculated by SoraResourceFileFinder
         *  Only calculates resource allocation and free
         *  (Library managed memory not included, such as gl texture)
         **/
        uint64 getResourceMemoryUsage() const;
		
        void setSystemFont(SoraFont* font);
		void setSystemFont(const wchar_t* font, int32 fontSize);
		SoraFont* getSystemFont() const;
        
		void setIcon(const SoraString& icon);
		void setCursor(const SoraString& cursor);
                
        typedef SoraFunction<void(SoraTextureHandle)> FullScreenBufferDelegateType;
        void registerFullscreenBufferDelegate(const FullScreenBufferDelegateType& delegate);
        void unregisterFullscreenBufferDelegate();
        
        /**
         *  transform a 3d point to 2d using a far point as view
         **/
        void        setFarPoint(const SoraVector3& ptFar);
        SoraVector3 getFarPoint() const;
        float       transform3DPoint(SoraVector3* point);
		float       transform3DPoint(float* x, float* y, float* z);
        
	private:
        static SoraCore* mInstance;
        
		inline void _registerCoreCmds();
		
		inline void _checkCoreComponents();
		inline void _postError(const SoraString& str);
		inline void _initializeTimer();
		inline void _initializeMiscTool();
        inline void _registerEventTypes();
        inline void _updateEnd();
        inline void _regGlobalProducts();
		inline void _frameListenerStart();
		inline void _frameListenerEnd();
        inline void _modifierAdapterUpdate();

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
		bool bPluginDetection;
        bool bSeperateSoundSystemThread;
		bool bDebugRender;
        
		bool bPaused;
		bool bPauseRender;
		bool bPauseSound;

		static int32 iRandomSeed;
		int32 iScreenWidth, iScreenHeight;

        bool bMainScene;
		bool bFrameSync;
		float mTime;
        float mTimeScale;

		SoraWindowInfoBase* mMainWindow;
        SoraShaderContext* mPrevShaderContext;
        
        bool bEnableScreenBuffer;
        bool bScreenBufferAttached;
        ulong32 mScreenBuffer;
				
		typedef std::list<SoraFrameListener*> FRAME_LISTENER_CONT;
		FRAME_LISTENER_CONT frameListeners;
		
		bool bZBufferArea;
        SoraVector3 mFarPoint;
        SoraFont* mSystemFont;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        bool bMultithreadedRendering;
        SoraMutex mRenderingLock;
#endif
    };

    inline SORA_API SoraCore* InitAndCreateSoraCore(SoraWindowInfoBase* window, const SoraCore::Feature& param = SoraCore::Feature()) {
        SoraCore* instance = SoraCore::Instance();
        instance->init(param);
        instance->createWindow(window);
        return instance;
    }
    
    typedef SoraCore::Feature SoraCoreFeature;
        
#define SORA SoraCore::Instance()

} // namespace sora

#endif
