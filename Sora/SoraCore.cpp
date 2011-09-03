#include "SoraCore.h"

#include "SoraTextureMap.h"
#include "SoraFileUtility.h"
#include "SoraEventManager.h"
#include "SoraConstantStrings.h"
#include "SoraFolderResourceManager.h"
#include "SoraInputQueue.h"
#include "SoraModifierAdapter.h"
#include "SoraFullscreenBufferHandler.h"
#include "SoraSystemEvents.h"
#include "SoraPlaybackEvent.h"
#include "SoraTimerEvent.h"
#include "SoraKeyInfo.h"
#include "SoraFileChangeEvent.h"
#include "SoraFastRenderer.h"
#include "SoraAutoUpdate.h"

#include "Defaults/SoraDefaultMiscTool.h"
#include "Defaults/SoraDefaultTimer.h"

#include "event/SoraEventFactory.h"
#include "event/SoraEventWorld.h"

#include "Defaults/SoraTimer_OSX.h"
#include "Defaults/SoraTimer_Win32.h"
#include "Defaults/SoraTimer_PSP.h"
#include "Defaults/SoraMiscTool_Win32.h"
#include "Defaults/SoraMiscTool_OSX.h"
#include "Defaults/SoraMiscTool_iOS.h"

#include "SoraLogger.h"
#include "Debug/SoraAutoProfile.h"
#include "Debug/SoraDebugRenderer.h"

#include "helpers/SoraInputSimulator.h"
#include "helpers/SoraZSorter.h"
#include "helpers/SoraBooter.h"
#include "helpers/SoraMenuBar.h"

#include "message/SoraMessageEvent.h"

#include "factory/SoraSpriteFactory.h"

#include "timer/SoraSimpleTimerManager.h"

#include "cmd/SoraConsole.h"

#include "SoraMemoryUsage.h"
#include "SoraMath.h"
#include "SoraCamera.h"
#include "SoraRenderSystemExtension.h"

#include <stack>

extern "C" {
#include "Random/SFMT.h"
}

namespace sora {
    
    SoraCore* SoraCore::mInstance = NULL;
    
    int SoraCore::iRandomSeed = rand();
    
    struct TransformData {
        float x;
        float y;
        float dx;
        float dy;
        float rot;
        float hscale;
        float vscale;
        
        void set(float _x, float _y, float _dx, float _dy, float _rot, float _hscale, float _vscale) {
            x = _x;
            y = _y;
            dx = _dx;
            dy = _dy;
            rot = _rot;
            hscale = _hscale;
            vscale = _vscale;
        }
    };
    
    struct ClippingData {
        int32 x;
        int32 y;
        int32 w;
        int32 h;
        
        void set(int32 _x, int32 _y, int32 _w, int32 _h) {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
        }
    };
    
    namespace {
        static std::stack<TransformData> g_TransformStack;
        static std::stack<ClippingData> g_ClippingStack;
        
        static TransformData g_CurrentTransform;
        static ClippingData g_CurrentClipping;
        
        SoraKeyPool* g_keypool;
    }
    
    SoraCore* SoraCore::Instance() {
        static SoraCore instance;
        return &instance;
    }
	
	SoraCore::SoraCore() {
		bMainWindowSet = false;
		bMessageBoxErrorPost = false;
		bInitialized = false;
		bFrameSync = false;
		bHasInput = false;
		bZBufferArea = false;
        bMainScene = false;
		bPaused = false;
		bPauseRender = false;
		bPauseSound = false;
		bDisablePluginDetection = false;
        bEnableScreenBuffer = false;
        bScreenBufferAttached = false;

		_initializeTimer();
		_initializeMiscTool();
		
		SoraEventManager::Instance();
		SoraInternalLogger::Instance();

		pInput = NULL;
		pFontManager = NULL;
		pRenderSystem = NULL;
		pSoundSystem = NULL;
        
		mMainWindow = NULL;
		
		mPrevShaderContext = NULL;
        mScreenBuffer = NULL;
        
        mTime = 0.f;
        mTimeScale = 1.0f;

		pPluginManager = new SoraPluginManager;
		pResourceFileFinder = new SoraResourceFileFinder;
		pResourceFileFinder->attachResourceManager(new SoraFolderResourceManager);
        
		setRandomSeed(rand());
        initConstantStrings();
        
        _registerEventTypes();
        _regGlobalProducts();
        
        new SoraSimpleTimerManager;
	}
    
    void SoraCore::_regGlobalProducts() {
    }
    
    void SoraCore::init(const Parameter& param) {
        enablePluginDetection(param.isLoadPlugins());
        
        enableMessageBoxErrorPost(param.isPostErrorByMessageBox());
        
        enableFullscreenBuffer(param.isRenderToBuffer());
        
        try {
            if(!bDisablePluginDetection) 
                SoraBooter::loadExPlugins(L"./sora/plugins");
        } catch(const SoraException& exp) {
            _postError(exp.what());
        }
    }

	void SoraCore::_initializeTimer() {
#if defined(OS_OSX) || defined(OS_IOS)
		pTimer = new SoraOSXTimer; return;
#elif defined(OS_WIN32)
		pTimer = new SoraWin32Timer; return;
#elif defined(OS_PSP)
		pTimer = new SoraPSPTimer; return;
#endif
		pTimer = new SoraDefaultTimer;
	}

	void SoraCore::_initializeMiscTool() {
#ifdef OS_WIN32
		pMiscTool = new SoraMiscToolWin32;  return;
#elif defined(OS_OSX)
		pMiscTool = new SoraMiscToolOSX; return;
#elif defined(OS_IOS)
		pMiscTool = new SoraMiscTooliOS; return;
#endif
		pMiscTool = new SoraDefaultMiscTool;
	}

	void SoraCore::start() {        
        try {
            // no main window created
            if(!bMainWindowSet) {
                THROW_SORA_EXCEPTION(SystemException, "No main window created");
                shutDown();
            }
            if(!bInitialized) {
                THROW_SORA_EXCEPTION(SystemException, "Sora not initialized");
                shutDown();
            }
        } catch(const SoraException& exp) {
            messageBox(exp.what(), "fatal error", MB_OK);
            shutDown();
        }
        
        _checkCoreComponents();
        
        // registering global keypool
        g_keypool = new SoraKeyPool;
        
        // startup consoles
        sora::SoraConsole::Instance();
        
        try {
        // create render target for debug renderer
#ifdef DEBUG
            DEBUG_RENDERER->createTarget();
#endif
            
            if(bEnableScreenBuffer) {
                mScreenBuffer = pRenderSystem->createTarget(iScreenWidth, iScreenHeight);
                if(!mScreenBuffer) {
                    bEnableScreenBuffer = false;
                    THROW_SORA_EXCEPTION(SystemException, "Error creating fullscreen buffer, fullscreen posteffect maybe disabled");
                }
                SoraFullscreenBufferHandler::Instance();
            }
            
        } catch(const SoraException& exp) {
            _postError(exp.what());
        }
        
        SoraFastRenderer::Instance();
        
        if(pRenderSystem)
            pRenderSystem->start(pTimer);
	}

	void SoraCore::_frameListenerStart() {
		FRAME_LISTENER_CONT::iterator p = frameListeners.begin();
		while( p != frameListeners.end() ) {
			(*p)->onFrameStart();
			++p;
		}
	}

	void SoraCore::_frameListenerEnd() {
		FRAME_LISTENER_CONT::iterator p = frameListeners.begin();
		while( p != frameListeners.end() ) {
			(*p)->onFrameEnd();
			++p;
		}
	}

	void SoraCore::addFrameListener(SoraFrameListener* listener) {
		frameListeners.push_back(listener);
	}

	void SoraCore::delFrameListener(SoraFrameListener* listener) {
		frameListeners.remove(listener);
	}

	void SoraCore::update() {
		sora_assert(bInitialized == true);
        float dt = bFrameSync?1.f:pTimer->getDelta();
#ifdef PROFILE_CORE_UPDATE
		PROFILE("CORE_UPDATE");
#endif

     //   try {
            if(!bPaused && !bPauseSound) {
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("UPDATE_SOUNDSYSTEM");
#endif
                    if(pSoundSystem) pSoundSystem->update();
                }
            }
            
            
            if(!bPaused) {
                SoraInputSimulator::clear();
                
                pRenderSystem->beginFrame();
                
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("UPDATE_MAINWINDOW");
#endif
                    _modifierAdapterUpdate();
                    SoraSimpleTimerManager::Instance()->update(dt);
                    
                    mMainWindow->updateFunc();
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("UPDATE_EVENT_MANAGER");
#endif
                    SoraEventManager::Instance()->update(dt);
                    SoraEventWorld::defaultWorld().update(dt);
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("FRAMELISTENER_START");
#endif
                    _frameListenerStart();
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("UPDATE_AUTO_UPDATE_OBJS");
#endif
                    SoraAutoUpdate::updateList(dt);
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("UPDATE_RENDERSYSTEM");
#endif
                    pRenderSystem->update();
                }
            } else 
                mMainWindow->pauseFunc();
            
            if(!bPaused && !bPauseRender) {
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("RENDER_MAINWINDOW");
#endif
                    mMainWindow->renderFunc();
                }
            }
            
            if(!bPaused) {
                {
#ifdef PROFILE_CORE_UPDATE
                    PROFILE("FRAMELISTENER_END");
#endif
                    _frameListenerEnd();
                }
                
                if(!bPauseRender) {                    
                    _updateEnd();
                }
                
                
                mTime += pTimer->getDelta();
                
                pRenderSystem->endFrame();
            }
     /*   } catch (const SoraException& exp) {
            messageBox(exp.what(), "A Error Happened :(", MB_OK | MB_ICONERROR);
        }*/
        
	}
    
    void SoraCore::_updateEnd() {
        if(bMainScene) {
            bMainScene = false;
            pRenderSystem->endScene();
            
            if(mScreenBuffer && bEnableScreenBuffer) {
                pRenderSystem->beginScene(0, 0, true);
                SoraFullscreenBufferHandler::Instance()->onBufferRender(getTargetTexture(mScreenBuffer));
                pRenderSystem->endScene();
                bScreenBufferAttached = false;
            }
        }
    }

	void SoraCore::flush() {
		if(pRenderSystem)
			pRenderSystem->flush();
	}

	void SoraCore::_checkCoreComponents() {
		if(!pInput) {
			bHasInput = false;
			_postError("SoraCore::CheckCoreComponents: No Input provided");
			DebugPtr->warning("No Input available, input related API disabled");
		}

		if(!pPluginManager) {
			pPluginManager = new SoraPluginManager;
			_postError("SoraCore::CheckCoreComponents: No PluginManager registered, using default plugin manager");
		}

		if(!pMiscTool) {
			pMiscTool = new SoraDefaultMiscTool;
			_postError("SoraCore::CheckCoreComponents: No MiscTool registered, using default tool");
		}

		if(!pFontManager) {
			_postError("SoraCore::CheckCoreComponents: no FontManager available");
			DebugPtr->warning("No FontManager available, font related API disabled");
		}
		
		if(!pSoundSystem) {
			_postError("SoraCore::CheckCoreComponents: no SoundSystem available");
			DebugPtr->warning("No SoundSystem available, sound related API disabled");
		}

		// no render system available, fatal error
		// cause currently there's no cross-platform render system HoshiNoSora implemented
		if(!pRenderSystem) {
			THROW_SORA_EXCEPTION(SystemException, "no render system available");
		}

		bInitialized = true;
	}

	void SoraCore::postError(const SoraString& string) {
		_postError(string);
	}

	void SoraCore::_postError(const SoraString& string) {
		if(!bMessageBoxErrorPost)
			log_mssg(string, LOG_LEVEL_ERROR);
		else
			pMiscTool->messageBox(string, "Some Error Happened :(", MB_ICONERROR | MB_OK);
	}
    
    void SoraCore::_registerEventTypes() {
        SoraEventFactory* factory = SoraEventFactory::Instance();
        factory->registerEvent<SoraPlaybackEvent>("PlaybackEvent");
        factory->registerEvent<SoraFileChangeEvent>("FileChangeEvent");
        factory->registerEvent<SoraKeyEvent>("KeyEvent");
      //  factory->registerEvent<SoraMouseEvent>("MouseEvent");
       // factory->registerEvent<SoraJoystickEvent>("JoystickEvent");
        factory->registerEvent<SoraTimerEvent>("TimerEvent");
        factory->registerEvent<SoraConsoleEvent>("ConsoleEvent");
        factory->registerEvent<SoraMenuBarClickEvent>("MenubarEvent");
        factory->registerEvent<SoraHotkeyEvent>("HotkeyEvent");
        factory->registerEvent<SoraMessageEvent>("MessageEvent");
    }

	ulong32 SoraCore::getMainWindowHandle() {
		return pRenderSystem->getMainWindowHandle();
	}

	SoraWindowInfoBase* SoraCore::getMainWindow() {
		return pRenderSystem->getMainWindow();
	}

	bool SoraCore::isActive() {
		return pRenderSystem->isActive();
	}

	void SoraCore::shutDown() {
		//SoraTextureMap::Instance()->Destroy();
        if(mMainWindow)
            delete mMainWindow;
        
		if(bHasInput) delete pInput;
		if(pPluginManager) delete pPluginManager;
		if(pTimer) delete pTimer;
		if(pFontManager) delete pFontManager;

		if(bInitialized) {
			pRenderSystem->shutdown();
			delete pRenderSystem;
		}
		delete pResourceFileFinder;
		
		if(pSoundSystem) {
			pSoundSystem->shutdown();
			delete pSoundSystem;
		}
		
        if(g_keypool) delete g_keypool;
		if(pMiscTool) delete pMiscTool;
				// force exiting
		exit(0);
	}

	bool SoraCore::createWindow(SoraWindowInfoBase* info) {
		if(mMainWindow == NULL) {
			iScreenWidth = info->getWindowWidth();
			iScreenHeight = info->getWindowHeight();
            
            SET_ENV_INT("CORE_SCREEN_WIDTH", iScreenWidth);
            SET_ENV_INT("CORE_SCREEN_HEIGHT", iScreenHeight);
			
			ulong32 result = pRenderSystem->createWindow(info);
			if(result) {
                if(pInput != NULL)
					pInput->setWindowHandle(result);
				pMiscTool->setMainWindowHandle(result);
				bMainWindowSet = true;
				mMainWindow = info;
                mMainWindow->setActive(true);
                mMainWindow->init();

				DebugPtr->notice(vamssg("Created MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mMainWindow->getWindowName().c_str()));
                
			} else {
				_postError("SoraCore::createWindow, createWindow failed");
				return false;
			}
		} else {
            mMainWindow->setActive(false);
			mMainWindow = info;
            if(mMainWindow->isActive())
                mMainWindow->init();
            else 
                mMainWindow->reinit();
            mMainWindow->setActive(true);
            
			log_mssg(vamssg("Set MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mMainWindow->getWindowName().c_str()),
						  LOG_LEVEL_NOTICE);
			
			if(info->getWindowWidth() != iScreenWidth || info->getWindowHeight() != iScreenHeight) {
				iScreenWidth = info->getWindowWidth();
				iScreenHeight = info->getWindowHeight();
				
				pRenderSystem->setWindowSize(iScreenWidth, iScreenHeight);
				pRenderSystem->setWindowTitle(s2ws(info->getWindowName()));
				pRenderSystem->setWindowPos(info->getWindowPosX(), info->getWindowPosY());
			}
		}
        
        SET_ENV_INT("CORE_SCREEN_WIDTH", iScreenWidth);
		SET_ENV_INT("CORE_SCREEN_HEIGHT", iScreenHeight);
		
        g_CurrentTransform.set(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
        g_CurrentClipping.set(0, 0, iScreenWidth, iScreenHeight);
        
		return true;
	}

	void SoraCore::setWindowSize(int32 w, int32 h) {
		sora_assert(bInitialized == true);
		if(SoraConsole::Instance()->getWidth() == iScreenWidth-2) {
			SoraConsole::Instance()->setSize(w-2.f, SoraConsole::Instance()->getHeight()>h?h:SoraConsole::Instance()->getHeight());
		}
		
		iScreenWidth = w;
		iScreenHeight = h;
		pRenderSystem->setWindowSize(w, h);
	}

	void SoraCore::setWindowTitle(const SoraWString& title) {
		sora_assert(bInitialized == true);

		pRenderSystem->setWindowTitle(title);
	}

	void SoraCore::setWindowPos(int32 px, int32 py) {
		sora_assert(bInitialized == true);

		pRenderSystem->setWindowPos(px, py);
	}

	void SoraCore::setFullscreen(bool flag) {
		sora_assert(bInitialized == true);

		pRenderSystem->setFullscreen(flag);
	}

	bool SoraCore::isFullscreen() {
		sora_assert(bInitialized == true);

		return pRenderSystem->isFullscreen();
	}

	void SoraCore::registerRenderSystem(SoraRenderSystem* renderSystem) {
		if(pRenderSystem) {
			pRenderSystem->shutdown();
			delete pRenderSystem;
		}
		pRenderSystem = renderSystem;
		bInitialized = true;
	}

	void SoraCore::registerInput(SoraInput* input) {
		if(bHasInput) delete pInput;
		pInput = input;
		bHasInput = true;
        
        SoraKeyPool::setQueueInput(input);
	}

	void SoraCore::registerResourceManager(SoraResourceManager* pResourceManager) {
		pResourceFileFinder->attachResourceManager(pResourceManager);
	}

	void SoraCore::registerMiscTool(SoraMiscTool* miscTool) {
		if(pMiscTool) delete pMiscTool;
		pMiscTool = miscTool;
	}

	void SoraCore::registerPluginManager(SoraPluginManager* pluginManager) {
		if(pPluginManager) delete pPluginManager;
		pPluginManager = pluginManager;
	}

	void SoraCore::registerTimer(SoraTimer* timer) {
		if(pTimer) delete pTimer;
		pTimer = timer;
	}
	
	void SoraCore::registerSoundSystem(SoraSoundSystem* ss) {
		if(pSoundSystem) {
			pSoundSystem->shutdown();
			delete pSoundSystem;
		}
		pSoundSystem = ss;
		pSoundSystem->init();
	}

	void SoraCore::registerFontManager(SoraFontManager* fontManager) {
		if(pFontManager) delete pFontManager;
		pFontManager = fontManager;
	}
    
    SoraRenderSystem* SoraCore::getRenderSystem() const {
        return pRenderSystem;
    }
    
    SoraInput* SoraCore::getInput() const {
        return pInput;
    }
    
    SoraMiscTool* SoraCore::getMiscTool() const {
        return pMiscTool;
    }
    
    SoraPluginManager* SoraCore::getPluginManager() const {
        return pPluginManager;
    }
    
    SoraTimer* SoraCore::getTimer() const {
        return pTimer;
    }
    
    SoraFontManager* SoraCore::getFontManager() const {
        return pFontManager;
    }
    
    SoraSoundSystem* SoraCore::getSoundSystem() const {
        return pSoundSystem;
    }

	float SoraCore::getFPS() {
		return pTimer->getFPS();
	}

	float SoraCore::getDelta() {
		if(!bFrameSync)
			return pTimer->getDelta() * mTimeScale;
		return 1.f;
	}
    
    float SoraCore::getAbsoluteDelta() {
        return pTimer->getDelta();
    }

	float SoraCore::getTime() {
		return pTimer->getTime();
	}

	uint64 SoraCore::getCurrentSystemTime() {
		return SoraTimestamp::currentTime();
	}

	void SoraCore::setFPS(int32 fps) {
		SET_ENV_INT("CORE_TIMER_SET_FPS", fps);
		
		pTimer->setFPS(fps);
	}

	int32 SoraCore::getFrameCount() {
		return pTimer->getFrameCount();
	}

	void SoraCore::setTimeScale(float scale) {
		SET_ENV_FLOAT("CORE_TIMER_TIME_SCALE", scale);
		mTimeScale = scale;
	}

	float SoraCore::getTimeScale() {
		return mTimeScale;
	}
    
    void SoraCore::setVerticalSync(bool flag) {
        if(!bInitialized) return;
        pRenderSystem->setVerticalSync(flag);
    }

	HSORARESOURCE SoraCore::loadResourcePack(const SoraWString& file) {
		return pResourceFileFinder->loadResourcePack(file);
	}

	void SoraCore::attachResourcePack(HSORARESOURCE pfile) {
		pResourceFileFinder->attachResourcePack(pfile);
	}

	void SoraCore::detachResourcePack(HSORARESOURCE handle) {
		pResourceFileFinder->detachResourcePack(handle);
	}

	void* SoraCore::getResourceFile(const SoraWString& sfile, ulong32& size) {
		return pResourceFileFinder->getResourceFile(sfile, size);
	}

	void SoraCore::freeResourceFile(void* p) {
		return pResourceFileFinder->freeResourceFile(p);
	}

	void* SoraCore::readResourceFile(const SoraWString& sfile, ulong32 size) {
		return pResourceFileFinder->readResourceFile(sfile, size);
	}

	ulong32 SoraCore::getResourceFileSize(const SoraWString& file) {
		return pResourceFileFinder->getResourceFileSize(file);
	}
    
    SoraShaderContext* SoraCore::createShaderContext() {
        sora_assert(bInitialized == true);
        return pRenderSystem->createShaderContext();
    }
    
	void SoraCore::attachShaderContext(SoraShaderContext* context) {
        sora_assert(bInitialized == true);
        pRenderSystem->attachShaderContext(context);
		
		mPrevShaderContext = context;
	}

	void SoraCore::detachShaderContext() {
        sora_assert(bInitialized == true);
        pRenderSystem->detachShaderContext();
        
		mPrevShaderContext = NULL;
	}
    
	HSORATEXTURE SoraCore::createTexture(const SoraWString& sTexturePath, bool bCache, bool bMipmap)	{
		sora_assert(bInitialized==true);
		HSORATEXTURE tex;
		if((tex = SoraTextureMap::Instance()->get(sTexturePath)) != 0) return tex;
		ulong32 size;
		void* data = getResourceFile(sTexturePath, size);
		if(data) {
			tex = (HSORATEXTURE)pRenderSystem->createTextureFromMem(data, size, bMipmap);
			if(bCache) {
				SoraTextureMap::Instance()->add(sTexturePath, tex);
			}

			freeResourceFile(data);
			return tex;
		}
		return 0;
	}

	HSORATEXTURE SoraCore::createTextureWH(int w, int h) {
		sora_assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureWH(w, h);
	}

	HSORATEXTURE SoraCore::createTextureFromRawData(uint32* data, int32 w, int32 h) {
		sora_assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureFromRawData(data, w, h);
	}

	HSORATEXTURE SoraCore::createTextureFromMem(void* data, ulong32 size) {
		sora_assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureFromMem(data, size);
	}

	uint32* SoraCore::textureLock(HSORATEXTURE ht) {
		sora_assert(bInitialized==true);
		return pRenderSystem->textureLock((SoraTexture*)ht);
	}

	void SoraCore::textureUnlock(HSORATEXTURE h) {
		if(!bInitialized) throw SoraException("Sora not initialized");;
		pRenderSystem->textureUnlock((SoraTexture*)h);
	}
	
	void SoraCore::releaseTexture(HSORATEXTURE pTexture) {
		sora_assert(bInitialized==true);
        if(!pTexture) return;
		SoraTextureMap::Instance()->del(pTexture);
        pRenderSystem->releaseTexture((SoraTexture*)pTexture);
	}
	
	int32 SoraCore::getTextureWidth(HSORATEXTURE tex, bool origin) {
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			return (origin?ptex->mOriginalWidth:ptex->mTextureWidth);
		return 0;
	}
	
	int32 SoraCore::getTextureHeight(HSORATEXTURE tex, bool origin) {
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			return (origin?ptex->mOriginalHeight:ptex->mTextureHeight);
		return 0;
	}
	
	ulong32	SoraCore::getTextureId(HSORATEXTURE tex) {
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			return ptex->mTextureID;
		return 0;
	}

	void SoraCore::clearTextureMap() {
		SoraTextureMap::Instance()->clear();
	}

	ulong32 SoraCore::getVideoDeviceHandle() {
		sora_assert(bInitialized==true);
		return pRenderSystem->getVideoDeviceHandle();
	}

	SoraSprite* SoraCore::createSprite(const SoraWString& sPath) {
		sora_assert(bInitialized==true);
		HSORATEXTURE tex;
		if((tex = SoraTextureMap::Instance()->get(sPath)) != 0) {
			SoraSprite* pspr = new SoraSprite(tex);
			pspr->setName(str2id(sPath));
			return pspr;
		} else {
			tex = createTexture(sPath);
			if(!tex)
				return new SoraSprite(tex);
			else {
				SoraSprite* pspr = new SoraSprite(tex);
				pspr->setName(str2id(sPath));
				return pspr;
			}
		}
	}
    
    void SoraCore::renderSprite(const SoraWString& path, float x, float y) {
        SoraFastRenderer::Instance()->renderSprite(path, x, y);
    }
	
	void SoraCore::renderQuad(SoraQuad& quad) {
		sora_assert(bInitialized==true);
		if(bZBufferArea) {
			SoraZSorter::renderQuad(quad, mPrevShaderContext);
		} else 
			pRenderSystem->renderQuad(quad);
	}

	void SoraCore::renderTriple(SoraTriple& trip) {
		sora_assert(bInitialized==true);
		
		if(bZBufferArea) {
			SoraZSorter::renderTriple(trip, mPrevShaderContext);
		} else 
			pRenderSystem->renderTriple(trip);
	}
	
	void SoraCore::renderWithVertices(HSORATEXTURE tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode) {
		sora_assert(bInitialized == true);
		if(bZBufferArea) {
			SoraZSorter::renderWithVertices(tex, blendMode, vertices, vsize, mode, mPrevShaderContext);
		} else 	
			pRenderSystem->renderWithVertices((SoraTexture*)tex, blendMode, vertices, vsize, mode);
	}

	void SoraCore::renderLine(float x1, float y1, float x2, float y2, uint32 color, float z) {
		sora_assert(bInitialized==true);
		pRenderSystem->renderLine(x1, y1, x2, y2, color, z);
	}
	
	void SoraCore::renderBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
		sora_assert(bInitialized==true);
		pRenderSystem->renderBox(x1, y1, x2, y2, color, z);
	}
    
    void SoraCore::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
		sora_assert(bInitialized==true);
		pRenderSystem->fillBox(x1, y1, x2, y2, color, z);
	}

	void SoraCore::setClipping(int32 x, int32 y, int32 w, int32 h) {
		sora_assert(bInitialized==true);
		pRenderSystem->setClipping(x, y, w, h);
        
        g_CurrentClipping.set(x, y, w, h);
	}

	void SoraCore::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		sora_assert(bInitialized==true);
		pRenderSystem->setTransform(x, y, dx, dy, rot, hscale, vscale);
        
        g_CurrentTransform.set(x, y, dx, dy, rot, hscale, vscale);
	}
    
    void SoraCore::setViewPoint(float x, float y, float z) {
        sora_assert(bInitialized==true);
        pRenderSystem->setViewPoint(x, y, z);
    }

	void SoraCore::beginScene(uint32 c, ulong32 t, bool clear) {
		sora_assert(bInitialized==true);
        if(mScreenBuffer && bEnableScreenBuffer) {
            if(t == 0) {
                bMainScene = true;
                bScreenBufferAttached = true;
                pRenderSystem->beginScene(0x00000000, mScreenBuffer, true);
            } else {
                bMainScene = false;
                bScreenBufferAttached = false;
                pRenderSystem->endScene();
                pRenderSystem->beginScene(c, t, clear);
            }
        } else {
            if(t == 0) {
                bMainScene = true;
            } else bMainScene = false;
            pRenderSystem->beginScene(c, t, clear);
        }
	}
	void SoraCore::endScene() {
		sora_assert(bInitialized==true);
		if(!bMainScene) {
            pRenderSystem->endScene();
            if(mScreenBuffer && bEnableScreenBuffer && bScreenBufferAttached) 
                pRenderSystem->beginScene(0, mScreenBuffer, false);
        }
	}

	ulong32 SoraCore::createTarget(int width, int height, bool zbuffer) {
		sora_assert(bInitialized==true);
		return pRenderSystem->createTarget(width==0?iScreenWidth:width, height==0?iScreenHeight:height, zbuffer);
	}

	void SoraCore::freeTarget(ulong32 t) {
		sora_assert(bInitialized==true);
		pRenderSystem->freeTarget(t);
	}

	ulong32 SoraCore::getTargetTexture(ulong32 t) {
		sora_assert(bInitialized==true);
		return pRenderSystem->getTargetTexture(t);
	}

	int32 SoraCore::getScreenHeight() {
		return iScreenHeight;
	}

	int32 SoraCore::getScreenWidth() {
		return iScreenWidth;
	}

	void SoraCore::getMousePos(float *x, float *y) {
		if(bHasInput) pInput->getMousePos(x, y);
	}

	float SoraCore::getMousePosX() {
		if(bHasInput) return pInput->getMousePosX();
		return 0.f;
	}

	float SoraCore::getMousePosY() {
		if(bHasInput) return pInput->getMousePosY();
		return 0.f;
	}

	void SoraCore::setMousePos(float x, float y) {
		if(bHasInput) pInput->setMousePos(x, y);
	}

	int	SoraCore::getMouseWheel() {
		if(bHasInput) return pInput->getMouseWheel();
		return 0;
	}

	bool SoraCore::isMouseOver() {
		if(bHasInput) return pInput->isMouseOver();
		return false;
	}

	bool SoraCore::keyDown(int32 key) {
		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState == SORA_INPUT_KEYDOWN;
		
		if(bHasInput) return pInput->keyDown(key);
		return false;
	}

	bool SoraCore::keyUp(int32 key) {
		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState == SORA_INPUT_KEYUP;
		
		if(bHasInput) return pInput->keyUp(key);
		return false;
	}
	
	void SoraCore::simulateKey(int32 key, int32 state) {
		SoraInputSimulator::simulateKey(key, state);
	}

	int32 SoraCore::getKeyState(int32 key) {
		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState;
		
		if(bHasInput) return pInput->getKeyState(key);
		return false;
	}

	char* SoraCore::getKeyName(int32 key) {
		if(bHasInput) return pInput->getKeyName(key);
		return 0;
	}

	bool SoraCore::getKeyEvent(SoraKeyEvent& ev) {
		if(bHasInput) return SoraKeyPool::getQueueEvent(ev);
		return false;
	}

	bool SoraCore::joyKeyPressed(int32 key) {
		if(bHasInput) return pInput->joyKeyPressed(key);
		return false;
	}

	bool SoraCore::joyKeyDown(int32 key) {
		if(bHasInput) return pInput->joyKeyDown(key);
		return false;
	}

	bool SoraCore::joyKeyUp(int32 key) {
		if(bHasInput) return pInput->joyKeyUp(key);
		return false;
	}

	bool SoraCore::joyKeyState(int32 key, unsigned char state) {
		if(bHasInput) return pInput->joyKeyState(key, state);
		return false;
	}

	bool SoraCore::setJoyKey(int32 key) {
		if(bHasInput) return pInput->setJoyKey(key);
		return false;
	}

	bool SoraCore::hasJoy() {
		if(bHasInput) return pInput->hasJoy();
		return false;
	}
    
    int32 SoraCore::registerGlobalHotkey(const SoraHotkey& key, SoraEventHandler* handler) {
        return SoraKeyPool::addGlobalHotKey(key, handler);
    }
    
    void SoraCore::setGlobalHotkey(int32 hid, const SoraHotkey& key) {
        SoraKeyPool::setGlobalHotkey(hid, key);
    }
    
    void SoraCore::unregisterGlobalHotkey(int32 hid) {
        SoraKeyPool::delGlobalHotkey(hid);
    }
    
    void SoraCore::clearGlobalHotkeys() {
        SoraKeyPool::clearGlobalHotkeys();
    }
	
	SoraWString SoraCore::fileOpenDialog(const char* filter, const char* defaultPath) {
		if(!pMiscTool)
			return SoraWString();
		
		return pMiscTool->fileOpenDialog(filter, defaultPath);
	}
	
	SoraWString SoraCore::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		if(!pMiscTool)
			return SoraWString();
		
		return pMiscTool->fileSaveDialog(filter, defaultPath, defaultExt);
	}

	int32 SoraCore::messageBox(const SoraString& sMssg, const SoraString& sTitle, int32 iCode) {
		if(!pMiscTool)
			return 0;
		
		pMiscTool->setMainWindowHandle(getMainWindowHandle());
		return pMiscTool->messageBox(sMssg, sTitle, iCode);
	}

	int32 SoraCore::messageBoxW(const SoraWString& sMssg, const SoraWString& sTitle, int32 iCode) {
		if(!pMiscTool)
			return 0;
		
		pMiscTool->setMainWindowHandle(getMainWindowHandle());
		return pMiscTool->messageBox(sMssg, sTitle, iCode);
	}

	void SoraCore::log(const SoraString& sMssg, int32 level) {
		log_mssg(sMssg, level);
	}

	void SoraCore::logw(const SoraWString& sMssg, int32 level) {
		log_mssg(ws2s(sMssg), level);
	}
	
	void SoraCore::logf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		log_mssg(Message);
	}

	void SoraCore::registerPlugin(SoraPlugin* pPlugin) {
		pPluginManager->registerPlugin(pPlugin);
	}

	SoraPlugin* SoraCore::unistallPlugin(SoraPlugin* pPlugin) {
		return pPluginManager->unistallPlugin(pPlugin);
	}

	SoraPlugin* SoraCore::unistallPlugin(const SoraString& sPluginName) {
		return pPluginManager->unistallPlugin(sPluginName);
	}

	SoraPlugin* SoraCore::getPlugin(const SoraString& sPluginName) {
		return pPluginManager->getPlugin(sPluginName);
	}

	SoraWString SoraCore::videoInfo() {
		sora_assert(bInitialized==true);
		return pRenderSystem->videoInfo();
	}

	void SoraCore::setRandomSeed(int32 seed) {
		init_gen_rand(seed);
		iRandomSeed = seed;
	}

	int32 SoraCore::getRandomSeed() {
		return iRandomSeed;
	}

	int32 SoraCore::randomInt(int32 min, int32 max) {
		return (int32)(min+genrand_real1()*(max-min));
	}

	float SoraCore::randomFloat(float min, float max) {
		return (float)(min+genrand_real1()*(max-min));
	}

	int32 SoraCore::randomIntNoRange() {
		return gen_rand32();
	}

	float SoraCore::randomFloatNoRange() {
		return (float)genrand_real1();
	}

	SoraFont* SoraCore::createFont(const SoraWString& font, int32 size) {
		if(!pFontManager) {
			_postError("FontManager not available");
			return 0;
		}

#ifndef OS_IOS
		SoraString fontName = ws2s(font);
		
		SoraFont* f;
		if(pFontManager->fontExists(fontName.c_str())) {
			f = pFontManager->getFont(fontName.c_str(), size, 0, 0);
			return f;
		}
		
      /*  if(SoraFileUtility::fileExists(font)) {
            f = pFontManager->getFont(fontName.c_str(), size);

			if(SoraConsole::Instance()->getFont() == NULL)
				SoraConsole::Instance()->setFont(f);
            return f;
        }*/
		ulong32 s;
		void* p = getResourceFile(font, s);
		if(p) {
			SoraFont* f = pFontManager->getFont((const char*)p, size, s-1, ws2s(font).c_str());
			//freeResourceFile(p);
			if(SoraConsole::Instance()->getFont() == NULL)
				SoraConsole::Instance()->setFont(f);
			if(SoraMenuBar::Instance()->getFont() == NULL)
				SoraMenuBar::Instance()->setFont(f);
			return f;
		}
#else
		SoraFont* ff = pFontManager->getFont(ws2s(font).c_str(), size);
		if(ff)
			SoraConsole::Instance()->setFont(ff);
		else
			_postError("Error loading Font: "+ws2s(font));
#endif
		return 0;
	}

	void SoraCore::releaseFont(SoraFont* font) {
		if(!pFontManager) {
			_postError("FontManager not available");
			return;
		}
		
		pFontManager->releaseFont(font);
	}
	
	void SoraCore::enumFilesInFolder(std::vector<SoraWString>& cont, const SoraWString& folder) {
		pResourceFileFinder->enumFiles(cont, folder);
	}

	void SoraCore::setFrameSync(bool flag) {
		bFrameSync = flag;
	}
	
	SoraMusicFile* SoraCore::createMusicFile(const SoraWString& musicName, bool bStream) {
		if(!pSoundSystem) {
			_postError("SoraCore::createMusicFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createMusicFile(musicName, bStream);
	}
	
	SoraSoundEffectFile* SoraCore::createSoundEffectFile(const SoraWString& se) {
		if(!pSoundSystem) {
			_postError("SoraCore::createSoundEffectFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createSoundEffectFile(se);
	}
    
    SoraMusicFile* SoraCore::createMusicFile(bool bStream) {
        if(!pSoundSystem) {
			_postError("SoraCore::createMusicFile: no soundsystem available");
			return NULL;
		}
        
        return pSoundSystem->createMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraCore::createSoundEffectFile() {
        if(!pSoundSystem) {
			_postError("SoraCore::createSoundEffectFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createSoundEffectFile();
    }
    
    void SoraCore::execute(const SoraString& appPath, const SoraString& args) {
        system((appPath+" "+args).c_str());
    }
    
    void SoraCore::snapshot(const SoraString& path) {
        sora_assert(bInitialized == true);
        return pRenderSystem->snapshot(path);
    }

	uint64 SoraCore::getEngineMemoryUsage() const {
		return getMemoryUsage();
	}
	
	void SORACALL SoraCore::enableMessageBoxErrorPost(bool bFlag) { 
		bMessageBoxErrorPost = bFlag;
	}
	
	void SoraCore::setSystemFont(const wchar_t* font, int32 fontSize) {
		SoraFont* ff = createFont(font, fontSize);
		SoraConsole::Instance()->setFont(ff);
		SoraMenuBar::Instance()->setFont(ff);
	}
	
	void SoraCore::beginZBufferSort() {
		bZBufferArea = true;
	}

	void SoraCore::endZBufferSort() {
		bZBufferArea = false;
		SoraZSorter::endSortAndRender();
	}

	void SoraCore::pause(bool render, bool sound) {
		bPauseRender = render;
		bPauseSound = sound;
		
		bPaused = true;
	}
	
	void SoraCore::resume() {
		bPaused = false;
	}
	
	bool SoraCore::isPaused() {
		return bPaused;
	}

	void SoraCore::setIcon(const SoraString& icon) {
		sora_assert(bInitialized);
		pRenderSystem->setIcon(icon);
	}

	void SoraCore::setCursor(const SoraString& cursor) {
		sora_assert(bInitialized);
		pRenderSystem->setCursor(cursor);
	}
	
	void SoraCore::enablePluginDetection(bool flag) {
		bDisablePluginDetection = !flag;
	}

    void SoraCore::_modifierAdapterUpdate() {
        SoraAbstractModiferAdapter::Members modifierAdapaterList = SoraAbstractModiferAdapter::members;
        
        if(modifierAdapaterList.size() == 0) {
            return;
        }
        
        SoraAbstractModiferAdapter::Members::iterator itModifier = modifierAdapaterList.begin();
        float dt = getDelta();
        while(itModifier != modifierAdapaterList.end()) {
            (*itModifier)->update(dt);
            ++itModifier;
        }
    }
    
    void  SoraCore::enableRenderSystemExtension(int32 extesion) {
        EnableRenderSystemExtension((ExtensionFeature)extesion);
    }
    
    void  SoraCore::disableRenderSystemExtension(int32 extension) {
        DisableRenderSystemExtension((ExtensionFeature)extension);
    }
    void  SoraCore::setRenderSystemExtensionParam(int32 extension, int32 param) {
        SetRenderSystemExtensionParam((ExtensionFeature)extension, param);
    }
    
    int32 SoraCore::getRenderSystemExtensionParam(int32 extension) {
        return GetRenderSystemExtensionParam((ExtensionFeature)extension);
    }
    
    bool SoraCore::isRenderSystemExtensionEnabled(int32 ext) {
        return IsRenderSystemExtensionEnabled((ExtensionFeature)ext);
    }
    
    void SoraCore::enableFullscreenBuffer(bool flag) {
        bEnableScreenBuffer = flag;
        if(flag && !mScreenBuffer) {
            mScreenBuffer = createTarget(getScreenWidth(), getScreenHeight());
            if(!mScreenBuffer)
                THROW_SORA_EXCEPTION(SystemException, "Error creating screen buffer, fullscreen buffer maybe disabled. :(");
        }
    }
    
    void SoraCore::registerFullscreenBufferDelegate(const SoraFunction<void(HSORATEXTURE)>& delegate) {
        SoraFullscreenBufferHandler::Instance()->registerDelegate(delegate);
    }
    
    void SoraCore::pushTransformMatrix() {
        g_TransformStack.push(g_CurrentTransform);
    }
    
    void SoraCore::popTransformMatrix() {
        if(!g_TransformStack.empty()) {
            g_CurrentTransform = g_TransformStack.top();
            g_TransformStack.pop();
            
            setTransform(g_CurrentTransform.x,
                         g_CurrentTransform.y,
                         g_CurrentTransform.dx,
                         g_CurrentTransform.dy,
                         g_CurrentTransform.rot,
                         g_CurrentTransform.hscale,
                         g_CurrentTransform.vscale);
        }
    }
   
    void SoraCore::pushClippingMatrix() {
        g_ClippingStack.push(g_CurrentClipping);
    }
    
    void SoraCore::popClippingMatrix() {
        if(!g_ClippingStack.empty()) {
            g_CurrentClipping = g_ClippingStack.top();
            g_ClippingStack.pop();
            
            setClipping(g_CurrentClipping.x,
                        g_CurrentClipping.y,
                        g_CurrentClipping.w,
                        g_CurrentClipping.h);
        }
    }
    
    uint64 SoraCore::getResourceMemoryUsage() const {
        return SoraResourceFileFinder::ResourceMemory / 1024;
    }
    
    void SoraCore::addMouseListener(SoraMouseListener* listener, int priority) {
        SoraKeyPool::addMouseListener(listener, priority);
    }
    
    void SoraCore::addKeyListener(SoraKeyListener* listener, int priority) {
        SoraKeyPool::addKeyListener(listener, priority);
    }
    
    void SoraCore::addJoystickListener(SoraJoystickListener* listener, int priority) {
        SoraKeyPool::addJoystickListener(listener, priority);
    }
    
    void SoraCore::delMouseListener(SoraMouseListener* listener) {
        SoraKeyPool::delMouseListener(listener);
    }
    
    void SoraCore::delKeyListener(SoraKeyListener* listener) {
        SoraKeyPool::delKeyListener(listener);
    }
    
    void SoraCore::delJoystickListener(SoraJoystickListener* listener) {
        SoraKeyPool::delJoystickListener(listener);
    }
    
    void SoraCore::setFarPoint(const SoraVector3& ptFar) {
        mFarPoint = ptFar;
    }
    
    SoraVector3 SoraCore::getFarPoint() const {
        return mFarPoint;
    }
    
    float SoraCore::transform3DPoint(SoraVector3* point) {
        if(mFarPoint.z == 0.f)
            return 1.f;
        
        float scale = (mFarPoint.z - point->z) / mFarPoint.z;
        point->x = (point->x - mFarPoint.x) * scale + mFarPoint.x;
        point->y = (point->y - mFarPoint.y) * scale + mFarPoint.y;
        return scale;
    }
    
    float SoraCore::transform3DPoint(float* x, float* y, float* z) {
        if(mFarPoint.z == 0.f)
            return 1.f;
        
        float scale = (mFarPoint.z - *z) / mFarPoint.z;
        *x = (*x - mFarPoint.x) * scale + mFarPoint.x;
        *y = (*y - mFarPoint.y) * scale + mFarPoint.y;
        return scale;
    }

} // namespace sora
