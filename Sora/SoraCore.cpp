#include "SoraCore.h"

#include "SoraTextureMap.h"
#include "SoraFileUtility.h"
#include "SoraEventManager.h"
#include "SoraConstantStrings.h"
#include "SoraMemoryFile.h"
#include "SoraFolderResourceManager.h"
#include "SoraInputQueue.h"

#include "Defaults/SoraDefaultMiscTool.h"
#include "Defaults/SoraDefaultTimer.h"

#include "Defaults/SoraTimer_OSX.h"
#include "Defaults/SoraTimer_Win32.h"
#include "Defaults/SoraTimer_PSP.h"
#include "Defaults/SoraMiscTool_Win32.h"
#include "Defaults/SoraMiscTool_OSX.h"
#include "Defaults/SoraMiscTool_iOS.h"

#include "Debug/SoraInternalLogger.h"
#include "Debug/SoraAutoProfile.h"
#include "Debug/SoraDebugRenderer.h"

#include "helpers/SoraInputSimulator.h"
#include "helpers/SoraZSorter.h"
#include "helpers/SoraBooter.h"

#include "cmd/CoreCmds.h"

#include "MemoryUsage.h"
#include "Rect4V.h"
#include "SoraCamera.h"

extern "C" {
#include "Random/SFMT.h"
}

namespace sora {
	
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

		_initializeTimer();
		_initializeMiscTool();
		
		SoraEventManager::Instance();
		SoraInternalLogger::Instance();

		pInput = NULL;
		pFontManager = NULL;
		pRenderSystem = NULL;
		pSoundSystem = NULL;
        
		mainWindow = NULL;
		
		__prevShader = NULL;
        shaderContext = NULL;
		mainCamera = NULL;

		pPluginManager = new SoraPluginManager;
		pResourceFileFinder = new SoraResourceFileFinder;
		pResourceFileFinder->attachResourceManager(new SoraFolderResourceManager);
		
		setRandomSeed(rand());
        initConstantStrings();
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
		if(!bDisablePluginDetection) 
			SoraBooter::loadExPlugins(L"./sora/plugins");
		
		// no main window created
		if(!bMainWindowSet) {
			THROW_SORA_EXCEPTION("No main window created");
			shutDown();
		}
		if(!bInitialized) {
			THROW_SORA_EXCEPTION("Sora not initialized");
			shutDown();
		}

		_checkCoreComponents();
      
        // create render target for debug renderer
#ifdef DEBUG
        DEBUG_RENDERER->createTarget();
#endif

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
		assert(bInitialized == true);
#ifdef PROFILE_CORE_UPDATE
		PROFILE("CORE_UPDATE");
#endif

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
				mainWindow->updateFunc();
			}
			
			{
#ifdef PROFILE_CORE_UPDATE
				PROFILE("UPDATE_EVENT_MANAGER");
#endif
				SORA_EVENT_MANAGER->update(bFrameSync?1.f:pTimer->getDelta());
			}
            
            {
#ifdef PROFILE_CORE_UPDATE
				PROFILE("FRAMELISTENER_START");
#endif
				_frameListenerStart();
			}
			
			{
#ifdef PROFILE_CORE_UPDATE
				PROFILE("UPDATE_PLUGINS");
#endif
				pPluginManager->update();
			}
			
			{
#ifdef PROFILE_CORE_UPDATE
				PROFILE("UPDATE_RENDERSYSTEM");
#endif
				pRenderSystem->update();
			}
		} else 
			mainWindow->pauseFunc();
        
		if(!bPaused && !bPauseRender) {
			{
#ifdef PROFILE_CORE_UPDATE
				PROFILE("RENDER_MAINWINDOW");
#endif
				mainWindow->renderFunc();
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
				DEBUG_RENDERER->render();
				
				SoraMenuBar::Instance()->update();
				SoraMenuBar::Instance()->render();
				
				SoraConsole::Instance()->render();

				
				if(bMainScene) {
					bMainScene = false;
					pRenderSystem->endScene();
				}
			}
			
			time += pTimer->getDelta();
			
			pRenderSystem->endFrame();
            
            keypoll::clearInputQueue();
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
			DebugPtr->log("No Input available, input related API disabled", 
						  LOG_LEVEL_WARNING);
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
			DebugPtr->log("No FontManager available, font related API disabled", 
						  LOG_LEVEL_WARNING);
		}
		
		if(!pSoundSystem) {
			_postError("SoraCore::CheckCoreComponents: no SoundSystem available");
			DebugPtr->log("No SoundSystem available, sound related API disabled", 
						  LOG_LEVEL_WARNING);
		}

		// no render system available, fatal error
		// cause currently there's no cross-platform render system HoshiNoSora implemented
		if(!pRenderSystem) {
			THROW_SORA_EXCEPTION("no render system available");
		}

		bInitialized = true;
	}

	void SoraCore::postError(const SoraString& string) {
		_postError(string);
	}

	void SoraCore::_postError(const SoraString& string) {
		if(!bMessageBoxErrorPost)
			DebugPtr->log(string, LOG_LEVEL_ERROR);
		else
			pMiscTool->messageBox(string, "SoraCoreError", MB_ICONERROR | MB_OK);
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
        if(mainWindow)
            delete mainWindow;
        if(shaderContext)
            delete shaderContext;
        
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
		
		if(pMiscTool) delete pMiscTool;
		
		// free all memory files
		SoraMemoryFile::Members memoryFiles = SoraMemoryFile::members;
		SoraMemoryFile::Members::iterator itMember = memoryFiles.begin();
		while(itMember != memoryFiles.end()) {
			delete *itMember;
			*itMember = NULL;
			
			++itMember;
		}
		
		SoraEventManager::Destroy();
		// force exiting
		exit(0);
	}

	ulong32 SoraCore::createWindow(SoraWindowInfoBase* info) {
		if(mainWindow == NULL) {
			iScreenWidth = info->getWindowWidth();
			iScreenHeight = info->getWindowHeight();
            
            SET_ENV_INT("CORE_SCREEN_WIDTH", iScreenWidth);
            SET_ENV_INT("CORE_SCREEN_HEIGHT", iScreenHeight);
			
			sora::SoraConsole::Instance();
			sora::SoraCoreCmdHandler::Instance();
		
			ulong32 result = pRenderSystem->createWindow(info);
			if(result) {
				if(pInput != NULL)
					pInput->setWindowHandle(result);
				pMiscTool->setMainWindowHandle(result);
				bMainWindowSet = true;
				mainWindow = info;
                mainWindow->setActive(true);
			
				DebugPtr->log(vamssg("Created MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mainWindow->getWindowName().c_str()),
							  LOG_LEVEL_NOTICE);
				return result;
			} else {
				_postError("SoraCore::createWindow, createWindow failed");
				shutDown();
				return 0;
			}
		} else {
            mainWindow->setActive(false);
			mainWindow = info;
            if(mainWindow->isActive())
                mainWindow->init();
            else 
                mainWindow->reinit();
            mainWindow->setActive(true);
            
			DebugPtr->log(vamssg("Set MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mainWindow->getWindowName().c_str()),
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
		
		
		return 0;
	}

	void SoraCore::setWindowSize(int32 w, int32 h) {
		assert(bInitialized == true);
		if(SoraConsole::Instance()->getWidth() == iScreenWidth-2) {
			SoraConsole::Instance()->setSize(w-2, SoraConsole::Instance()->getHeight()>h?h:SoraConsole::Instance()->getHeight());
		}
		
		iScreenWidth = w;
		iScreenHeight = h;
		pRenderSystem->setWindowSize(w, h);
	}

	void SoraCore::setWindowTitle(const SoraWString& title) {
		assert(bInitialized == true);

		pRenderSystem->setWindowTitle(title);
	}

	void SoraCore::setWindowPos(int32 px, int32 py) {
		assert(bInitialized == true);

		pRenderSystem->setWindowPos(px, py);
	}

	void SoraCore::setFullscreen(bool flag) {
		assert(bInitialized == true);

		pRenderSystem->setFullscreen(flag);
	}

	bool SoraCore::isFullscreen() {
		assert(bInitialized == true);

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
        
        keypoll::setQueueInput(input);
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

	float32 SoraCore::getFPS() {
		return pTimer->getFPS();
	}

	float32 SoraCore::getDelta() {
		if(!bFrameSync)
			return pTimer->getDelta();
		return 1.f;
	}

	float32 SoraCore::getTime() {
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

	void SoraCore::setTimeScale(float32 scale) {
		SET_ENV_FLOAT("CORE_TIMER_TIME_SCALE", scale);

		pTimer->setTimeScale(scale);
	}

	float32 SoraCore::getTimeScale() {
		return pTimer->getTimeScale();
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
    
    SoraShader* SoraCore::createShader(const SoraWString& file, const SoraString& entry, SORA_SHADER_TYPE type) {
        if(shaderContext == NULL) {
            shaderContext = createShaderContext();
            if(shaderContext == NULL)
                return NULL;
        }
        return shaderContext->createShader(file, entry, type);
    }

    SoraShaderContext* SoraCore::createShaderContext() {
        assert(bInitialized == true);
        return pRenderSystem->createShaderContext();
    }
    
	void SoraCore::attachShaderContext(SoraShaderContext* context) {
        assert(bInitialized == true);
        pRenderSystem->attachShaderContext(context);
		
		__prevShader = context;
	}

	void SoraCore::detachShaderContext() {
        assert(bInitialized == true);
        pRenderSystem->detachShaderContext();
		
		__prevShader = NULL;
	}

	HSORATEXTURE SoraCore::createTexture(const SoraWString& sTexturePath, bool bCache, bool bMipmap)	{
		assert(bInitialized==true);
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
		assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureWH(w, h);
	}

	HSORATEXTURE SoraCore::createTextureFromRawData(uint32* data, int32 w, int32 h) {
		assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureFromRawData(data, w, h);
	}

	HSORATEXTURE SoraCore::createTextureFromMem(void* data, ulong32 size) {
		assert(bInitialized==true);
		return (HSORATEXTURE)pRenderSystem->createTextureFromMem(data, size);
	}

	uint32* SoraCore::textureLock(HSORATEXTURE ht) {
		assert(bInitialized==true);
		return pRenderSystem->textureLock((SoraTexture*)ht);
	}

	void SoraCore::textureUnlock(HSORATEXTURE h) {
		if(!bInitialized) throw SoraException("Sora not initialized");;
		pRenderSystem->textureUnlock((SoraTexture*)h);
	}
	
	void SoraCore::releaseTexture(HSORATEXTURE pTexture) {
		assert(bInitialized==true);
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
		assert(bInitialized==true);
		return pRenderSystem->getVideoDeviceHandle();
	}

	SoraSprite* SoraCore::createSprite(const SoraWString& sPath) {
		assert(bInitialized==true);
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
	
	void SoraCore::renderQuad(SoraQuad& quad) {
		assert(bInitialized==true);
		if(bZBufferArea) {
			SoraZSorter::renderQuad(quad, __prevShader);
		} else 
			pRenderSystem->renderQuad(quad);
	}

	void SoraCore::renderTriple(SoraTriple& trip) {
		assert(bInitialized==true);
		
		if(bZBufferArea) {
			SoraZSorter::renderTriple(trip, __prevShader);
		} else 
			pRenderSystem->renderTriple(trip);
	}
	
	void SoraCore::renderWithVertices(HSORATEXTURE tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode) {
		assert(bInitialized == true);
		if(bZBufferArea) {
			SoraZSorter::renderWithVertices(tex, blendMode, vertices, vsize, mode, __prevShader);
		} else 	
			pRenderSystem->renderWithVertices((SoraTexture*)tex, blendMode, vertices, vsize, mode);
	}

	void SoraCore::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, ulong32 color, float32 z) {
		assert(bInitialized==true);
		pRenderSystem->renderRect(x1, y1, x2, y2, fWidth, color, z);
	}
	
	void SoraCore::renderBox(float32 x1, float32 y1, float32 x2, float32 y2, ulong32 color, float32 z) {
		assert(bInitialized==true);
		pRenderSystem->renderBox(x1, y1, x2, y2, color, z);
	}

	void SoraCore::setClipping(int32 x, int32 y, int32 w, int32 h) {
		assert(bInitialized==true);
		pRenderSystem->setClipping(x, y, w, h);
	}

	void SoraCore::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		assert(bInitialized==true);
		pRenderSystem->setTransform(x, y, dx, dy, rot, hscale, vscale);
	}
	
	void SoraCore::pushTransformMatrix() {
		assert(bInitialized==true);
	}
	
	void SoraCore::popTransformMatrix() {
		assert(bInitialized==true);
	}
    
    void SoraCore::setViewPoint(float32 x, float32 y, float32 z) {
        assert(bInitialized==true);
        pRenderSystem->setViewPoint(x, y, z);
    }

	void SoraCore::beginScene(ulong32 c, ulong32 t, bool clear) {
		assert(bInitialized==true);
		pRenderSystem->beginScene(c, t, clear);
        if(t == 0)
            bMainScene = true;
	}
	void SoraCore::endScene() {
		assert(bInitialized==true);
		if(!bMainScene) 
            pRenderSystem->endScene();
	}

	ulong32 SoraCore::createTarget(int width, int height, bool zbuffer) {
		assert(bInitialized==true);
		return pRenderSystem->createTarget(width==0?iScreenWidth:width, height==0?iScreenHeight:height, zbuffer);
	}

	void SoraCore::freeTarget(ulong32 t) {
		assert(bInitialized==true);
		pRenderSystem->freeTarget(t);
	}

	ulong32 SoraCore::getTargetTexture(ulong32 t) {
		assert(bInitialized==true);
		return pRenderSystem->getTargetTexture(t);
	}

	int32 SoraCore::getScreenHeight() {
		return iScreenHeight;
	}

	int32 SoraCore::getScreenWidth() {
		return iScreenWidth;
	}

	void SoraCore::getMousePos(float32 *x, float32 *y) {
		if(bHasInput) pInput->getMousePos(x, y);
	}

	float32 SoraCore::getMousePosX() {
		if(bHasInput) return pInput->getMousePosX();
		return 0.f;
	}

	float32 SoraCore::getMousePosY() {
		if(bHasInput) return pInput->getMousePosY();
		return 0.f;
	}

	void SoraCore::setMousePos(float32 x, float32 y) {
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
		if(bHasInput) return keypoll::getQueueEvent(ev);
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
        return keypoll::addGlobalHotKey(key, handler);
    }
    
    void SoraCore::setGlobalHotkey(int32 hid, const SoraHotkey& key) {
        keypoll::setGlobalHotkey(hid, key);
    }
    
    void SoraCore::unregisterGlobalHotkey(int32 hid) {
        keypoll::delGlobalHotkey(hid);
    }
    
    void SoraCore::clearGlobalHotkeys() {
        keypoll::clearGlobalHotkeys();
    }
	
	SoraWString SoraCore::fileOpenDialog(const char* filter, const char* defaultPath) {
		if(!pMiscTool)
			return L"\0";
		
		return pMiscTool->fileOpenDialog(filter, defaultPath);
	}
	
	SoraWString SoraCore::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		if(!pMiscTool)
			return L"\0";
		
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
		DebugPtr->log(sMssg, level);
	}

	void SoraCore::logw(const SoraWString& sMssg, int32 level) {
		DebugPtr->log(ws2s(sMssg), level);
	}
	
	void SoraCore::logf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		DebugPtr->log(Message);
	}

	void SoraCore::registerPlugin(SoraPlugin* pPlugin) {
		pPluginManager->registerPlugin(pPlugin);
	}

	void SoraCore::unistallPlugin(SoraPlugin* pPlugin) {
		pPluginManager->unistallPlugin(pPlugin);
	}

	void SoraCore::unistallPluginS(const SoraString& sPluginName) {
		pPluginManager->unistallPlugin(sPluginName);
	}

	SoraPlugin* SoraCore::getPlugin(const SoraString& sPluginName) {
		return pPluginManager->getPlugin(sPluginName);
	}

	SoraWString SoraCore::videoInfo() {
		assert(bInitialized==true);
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

	float32 SoraCore::randomFloat(float32 min, float32 max) {
		return (float32)(min+genrand_real1()*(max-min));
	}

	int32 SoraCore::randomIntNoRange() {
		return gen_rand32();
	}

	float32 SoraCore::randomFloatNoRange() {
		return (float32)genrand_real1();
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
    
    SoraMusicFile* SORACALL SoraCore::createMusicFile(bool bStream) {
        if(!pSoundSystem) {
			_postError("SoraCore::createMusicFile: no soundsystem available");
			return NULL;
		}
        
        return pSoundSystem->createMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SORACALL SoraCore::createSoundEffectFile() {
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
        assert(bInitialized == true);
        return pRenderSystem->snapshot(path);
    }

	uint64 SoraCore::getEngineMemoryUsage() {
		return getMemoryUsage();
	}
	
	void SORACALL SoraCore::enableMessageBoxErrorPost(bool bFlag) { 
		bMessageBoxErrorPost = bFlag;
	}
	
	SoraConsole* SoraCore::getConsole() const {
		return SoraConsole::Instance();
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
	
	SoraMenuBar* SoraCore::getMenuBar() const {
		return SoraMenuBar::Instance();
	}
	
	void SoraCore::enableMenuBar(bool flag) {
		SoraMenuBar::Instance()->setEnabled(flag);
	}

	void SoraCore::setIcon(const SoraString& icon) {
		assert(bInitialized);
		pRenderSystem->setIcon(icon);
	}

	void SoraCore::setCursor(const SoraString& cursor) {
		assert(bInitialized);
		pRenderSystem->setCursor(cursor);
	}
	
	
	void SoraCore::setMainCamera(SoraCamera* camera) {
		assert(camera != NULL);
		mainCamera = camera;
	}
	
	void SoraCore::enablePluginDetection(bool flag) {
		bDisablePluginDetection = !flag;
	}
	
	SoraCamera* SoraCore::getMainCamera() const {
		return mainCamera;
	}

} // namespace sora
