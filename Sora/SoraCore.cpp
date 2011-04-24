#include "SoraCore.h"

#include "SoraTextureMap.h"
#include "SoraFileUtility.h"
#include "SoraEventManager.h"
#include "SoraConstantStrings.h"

#include "Defaults/SoraDefaultMiscTool.h"
#include "Defaults/SoraDefaultTimer.h"

#include "osxfilewrapper.h"
#include "win32filewrapper.h"

#include "SoraFolderResourceManager.h"

#include "Defaults/SoraOSXTimer.h"
#include "Defaults/SoraTimer_Win32.h"
#include "Defaults/SoraMiscTool_Win32.h"
#include "Defaults/SoraMiscTool_OSX.h"
#include "Defaults/SoraMiscTool_iOS.h"

#include "Random/hgeRandom.h"

#include "Debug/SoraInternalLogger.h"
#include "Debug/SoraAutoProfile.h"
#include "Debug/SoraDebugRenderer.h"

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

		_initializeTimer();
		_initializeMiscTool();

		pInput = 0;
		pFontManager = 0;
		pRenderSystem = 0;
		pSoundSystem = 0;
        
        bMainScene = false;
		mainWindow = 0;
        
        shaderContext = 0;

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
		// no main window created
		if(!bMainWindowSet) {
			throw SORA_EXCEPTION("No main window created");
			shutDown();
		}
		if(!bInitialized) {
			throw SORA_EXCEPTION("Sora not initialized");
			shutDown();
		}

		_checkCoreComponents();
		_logInternalLog();
      
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
		
		{
#ifdef PROFILE_CORE_UPDATE
            PROFILE("UPDATE_SOUNDSYSTEM");
#endif
            if(pSoundSystem) pSoundSystem->update();
        }

		pRenderSystem->beginFrame();
        
		_frameListenerStart();
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("UPDATE_MAINWINDOW");
#endif
            mainWindow->updateFunc();
        }
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("UPDATE_PLUGINS");
#endif
            pPluginManager->update();
        }
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("UPDATE_EVENT_MANAGER");
#endif
            SORA_EVENT_MANAGER->update(bFrameSync?1.f:pTimer->getDelta());
        }
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("UPDATE_RENDERSYSTEM");
#endif
            pRenderSystem->update();
        }
        
		
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("RENDER_MAINWINDOW");
#endif
            mainWindow->renderFunc();
        }
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("FRAMELISTENER_END");
#endif
            _frameListenerEnd();
        }
        
        {
#ifdef PROFILE_CORE_UPDATE
            PROFILE("DEBUG_RENDER");
#endif
            
#ifdef _DEBUG
            DEBUG_RENDERER->render();
#endif
        }
        
        if(bMainScene) {
            bMainScene = false;
            pRenderSystem->endScene();
        }
        
		_logInternalLog();
		time += pTimer->getDelta();
        
		pRenderSystem->endFrame();
	}

	void SoraCore::flush() {
		if(pRenderSystem)
			pRenderSystem->flush();
	}

	void SoraCore::_checkCoreComponents() {
		if(!pInput) {
			bHasInput = false;
			_postError("No input provided");
		}

		if(!pPluginManager) {
			pPluginManager = new SoraPluginManager;
			_postError("SoraCore::CheckCoreComponents: No plugin manager registered, using default plugin manager");
		}

		if(!pMiscTool) {
			pMiscTool = new SoraDefaultMiscTool;
			_postError("SoraCore::CheckCoreComponents: No misc tool registered, using default tool");
		}

		if(!pFontManager) {
			_postError("SoraCore::CheckCoreComponents: no fontmanager available");
		}
		
		if(!pSoundSystem) {
			_postError("SoraCore::CheckCoreComponents: no soundsystem available");
		}

		// no render system available, fatal error
		// cause currently there's no cross-platform render system HoshiNoSora implemented
		if(!pRenderSystem) {
			throw SORA_EXCEPTION("no render system available");
		}

		bInitialized = true;
	}

	void SoraCore::_logInternalLog() {
		std::vector<SoraString> vLog = SoraInternalLogger::Instance()->get();
		for(size_t i=0; i<vLog.size(); ++i)
			pMiscTool->log(vLog[i]);
		SoraInternalLogger::Instance()->clear();
	}

	void SoraCore::postError(const SoraString& string) {
		_postError(string);
	}

	void SoraCore::_postError(const SoraString& string) {
		if(!bMessageBoxErrorPost)
			pMiscTool->log(string);
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

		SoraEventManager::Destroy();
		// force exiting
		exit(0);
	}

	ulong32 SoraCore::createWindow(SoraWindowInfoBase* info) {
		iScreenWidth = info->getWindowWidth();
		iScreenHeight = info->getWindowHeight();

		ulong32 result = pRenderSystem->createWindow(info);
		if(result) {
			pMiscTool->setMainWindowHandle(result);
			bMainWindowSet = true;
			mainWindow = info;
			return result;
		} else {
			_postError("SoraCore::createWindow, createWindow failed");
			shutDown();
			return 0;
		}
		return 0;
	}

	void SoraCore::setWindowSize(int32 w, int32 h) {
		assert(bInitialized == true);
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

	float32 SoraCore::getFPS() const {
		return pTimer->getFPS();
	}

	float32 SoraCore::getDelta() const {
		if(!bFrameSync)
			return pTimer->getDelta();
		return 1.f;
	}

	float32 SoraCore::getTime() const {
		return pTimer->getTime();
	}

	s_int64 SoraCore::getCurrentSystemTime() const {
		return pTimer->getCurrentSystemTime();
	}

	void SoraCore::setFPS(int32 fps) {
		pTimer->setFPS(fps);
	}

	int32 SoraCore::getFrameCount() const {
		return pTimer->getFrameCount();
	}

	void SoraCore::setTimeScale(float32 scale) {
		pTimer->setTimeScale(scale);
	}

	float32 SoraCore::getTimeScale() const {
		return pTimer->getTimeScale();
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
	}

	void SoraCore::detachShaderContext() {
        assert(bInitialized == true);
        pRenderSystem->detachShaderContext();
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

	ulong32* SoraCore::textureLock(HSORATEXTURE ht) {
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
        pRenderSystem->releaseTexture((SoraTexture*)pTexture);
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
			return new SoraSprite(tex);
		} else {
			tex = createTexture(sPath);
			return new SoraSprite(tex);
		}
	}

	SoraSprite* SoraCore::createSpriteTex(HSORATEXTURE tex) {
		assert(bInitialized==true);
		return new SoraSprite(tex);
	}

	void SoraCore::renderQuad(SoraQuad& quad) {
		assert(bInitialized==true);
		pRenderSystem->renderQuad(quad);
	}

	void SoraCore::renderTriple(SoraTriple& trip) {
		assert(bInitialized==true);
		pRenderSystem->renderTriple(trip);
	}

	void SoraCore::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, DWORD color, float32 z) {
		assert(bInitialized==true);
		pRenderSystem->renderRect(x1, y1, x2, y2, fWidth, color, z);
	}

	void SoraCore::setClipping(int32 x, int32 y, int32 w, int32 h) {
		assert(bInitialized==true);
		pRenderSystem->setClipping(x, y, w, h);
	}

	void SoraCore::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		assert(bInitialized==true);
		pRenderSystem->setTransform(x, y, dx, dy, rot, hscale, vscale);
	}

	void SoraCore::setTransformWindowSize(float32 w, float32 h) {
		assert(bInitialized==true);
		pRenderSystem->setTransformWindowSize(w, h);
	}
    
    void SoraCore::setViewPoint(float32 x, float32 y, float32 z) {
        assert(bInitialized==true);
        pRenderSystem->setViewPoint(x, y, z);
    }

	void SoraCore::beginScene(ulong32 c, ulong32 t) {
		assert(bInitialized==true);
		pRenderSystem->beginScene(c, t);
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
		return pRenderSystem->createTarget(width, height, zbuffer);
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
		if(bHasInput) return pInput->keyDown(key);
		return false;
	}

	bool SoraCore::keyUp(int32 key) {
		if(bHasInput) return pInput->keyUp(key);
		return false;
	}

	bool SoraCore::getKeyState(int32 key) {
		if(bHasInput) return pInput->getKeyState(key);
		return false;
	}

	char* SoraCore::getKeyName(int32 key) {
		if(bHasInput) return pInput->getKeyName(key);
		return 0;
	}

	bool SoraCore::getKeyEvent(SoraKeyEvent& ev) {
		if(bHasInput) return pInput->getKeyEvent(ev);
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

	int32 SoraCore::messageBox(const SoraString& sMssg, const SoraString& sTitle, int32 iCode) {
		pMiscTool->setMainWindowHandle(getMainWindowHandle());
		return pMiscTool->messageBox(sMssg, sTitle, iCode);
	}

	int32 SoraCore::messageBoxW(const SoraWString& sMssg, const SoraWString& sTitle, int32 iCode) {
		pMiscTool->setMainWindowHandle(getMainWindowHandle());
		return pMiscTool->messageBox(sMssg, sTitle, iCode);
	}

	void SoraCore::log(const SoraString& sMssg) {
		pMiscTool->log(sMssg);
	}

	void SoraCore::logw(const SoraWString& sMssg) {
		pMiscTool->log(sMssg);
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

	void SoraCore::logf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);

		pMiscTool->log(Message);
	}

	void SoraCore::setRandomSeed(int32 seed) {
		Random_Seed(seed);
		init_gen_rand(seed);
	}

	int32 SoraCore::getRandomSeed() {
		return g_seed2;
	}

	int32 SoraCore::randomInt(int32 min, int32 max) {
		return Random_int32(min, max);
	}

	float32 SoraCore::randomFloat(float32 min, float32 max) {
		return Random_float32(min, max);
	}

	int32 SoraCore::_SFMTRandomInt(int32 min, int32 max) {
		return (int32)(min+genrand_real1()*(max-min));
	}

	float32 SoraCore::_SFMTRandomFloat(float32 min, float32 max) {
		return (float32)(min+genrand_real1()*(max-min));
	}

	int32 SoraCore::_SFMTRandomInt32() {
		return gen_rand32();
	}

	float32 SoraCore::_SFMTRandomFloat32() {
		return (float32)genrand_real1();
	}

	SoraFont* SoraCore::createFont(const SoraWString& font, int32 size) {
		if(!pFontManager) {
			_postError("FontManager not available");
			return 0;
		}

#ifndef OS_IOS
        if(SoraFileUtility::fileExists(font)) {
            SoraFont* f = pFontManager->getFont(ws2s(font).c_str(), size);
            return f;
        }
		ulong32 s;
		void* p = getResourceFile(font, s);
		if(p) {
			SoraFont* f = pFontManager->getFont((const char*)p, size, s-1, ws2s(font).c_str());
			//freeResourceFile(p);
			return f;
		}
#else
		return pFontManager->getFont(ws2s(font).c_str(), size);
#endif
		_postError("Error loading font");
		return 0;
	}

	void SoraCore::enumFilesInFolder(std::vector<SoraWString>& cont, const SoraWString& folder) {
#ifdef WIN32
		sorawin32::enumFilesInFolder(cont, folder);
#endif
#ifdef OS_OSX
		soraosx::enumFilesInFolder(cont, folder);
#endif
#ifndef USE_BOOST
#endif
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
        system((appPath+args).c_str());
    }
    
    void SoraCore::snapshot(const SoraString& path) {
        assert(bInitialized == true);
        return pRenderSystem->snapshot(path);
    }


} // namespace sora
