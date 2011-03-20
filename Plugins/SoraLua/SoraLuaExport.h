#ifndef SORA_LUA_EXPORT
#define SORA_LUA_EXPORT

#include "luaclass.h"
#include "LuaPlusHelper.h"

//#include "SoraPhysicalObject.h"
//#include "SoraRenderObject.h"
//#include "SoraLuaObject.h"

//#include "SoraZipFile.h"
//#include "Files/SoraSoundFile.h"
//#include "Files/SoraHttpFile.h"
//#include "SoraINIFile.h"
//#include "SoraParticleSystem.h"

#include "SoraCore.h"

#include <vector>

//#include "Wrapper.h"

#include "SoraEventManager.h"

namespace sora { 

	class SoraLuaExport {
	public:
		static void export_soracore(SoraLuaObject* state) {
			export_soracore(state->getState());
		}
		static void export_soracore(LuaState* state) {
			SoraCore* pCore = SoraCore::Instance();
			gge::LuaClass<SoraCore>(state, "SoraCore", pCore)
				.def("shutDown", &SoraCore::shutDown)
				.def("start", &SoraCore::start)

				.def("beginScene", &SoraCore::beginScene)
				.def("endScene", &SoraCore::endScene)
				
				.def("createTarget", &SoraCore::createTarget)
				.def("freeTarget", &SoraCore::freeTarget)
				.def("getTargetTexture", &SoraCore::getTargetTexture)

				.def("messageBox", &SoraCore::messageBox)
				.def("log", &SoraCore::log)
				.def("unistallPlugin", &SoraCore::unistallPluginS)
				.def("setFPS", &SoraCore::setFPS)
				.def("getFPS", &SoraCore::getFPS)
				.def("getDelta", &SoraCore::getDelta)
				.def("getMousePosX", &SoraCore::getMousePosX)
				.def("getMousePosY", &SoraCore::getMousePosY)
				.def("setMousePos", &SoraCore::setMousePos)
				.def("getMouseWheel", &SoraCore::getMouseWheel)
				.def("isMouseOver", &SoraCore::isMouseOver)
				.def("keyDown", &SoraCore::keyDown)
				.def("keyUp", &SoraCore::keyUp)
				.def("getKeyState", &SoraCore::getKeyState)
				.def("getKeyName", &SoraCore::getKeyName)
				.def("joyKeyPressed", &SoraCore::joyKeyPressed)
				.def("joyKeyDown", &SoraCore::joyKeyDown)
				.def("joyKeyUp", &SoraCore::joyKeyUp)
				.def("joyKeyState", &SoraCore::joyKeyState)
				.def("setJoyKey", &SoraCore::setJoyKey)
				.def("hasJoy", &SoraCore::hasJoy)
			
				.def("enableMessageBoxErrorPost", &SoraCore::enableMessageBoxErrorPost)
				
		//		.def("createFont", &SoraCore::createFont)
				
				.def("createTexture", &SoraCore::createTexture)
				.def("createSprite", &SoraCore::createSprite)
				.def("createSpriteWithTex", &SoraCore::createSpriteTex)
			//	.def("renderSprite4V", &SoraCore::renderSprite4V)

				.def("renderRect", &SoraCore::renderRect)
			
				.def("loadResourcePack", &SoraCore::loadResourcePack)
				.def("attachResourcePack", &SoraCore::attachResourcePack)
				.def("detachResourcePack", &SoraCore::detachResourcePack)
				.def("readResourceFile", &SoraCore::readResourceFile)
				.def("getResourceFileSize", &SoraCore::getResourceFileSize)

				.def("setClipping", &SoraCore::setClipping)
				.def("setTransform", &SoraCore::setTransform);
			
			gge::LuaClass<SoraEventManager>(state, "SoraEventManager", SoraEventManager::Instance())
				.def("sendMessage", &SoraEventManager::sendMessage);
		}

		static void export_sorafiles(SoraLuaObject* state) {
			export_sorafiles(state->getState());
		}
		static void export_sorafiles(LuaState* state) {
		/*	::LuaClass<SoraMusicFile>(state)
				.create("SoraMusicFile")
				.create<int>("SoraMusicFileStreamFlag")
				.destroy("Free")
				.def("readFile", &SoraMusicFile::readFile)
				.def("readFileMem", &SoraMusicFile::readFileMem)
				.def("closeFile", &SoraMusicFile::closeFile)
				.def("play", &SoraMusicFile::play)
				.def("stop", &SoraMusicFile::stop)
				.def("pause", &SoraMusicFile::pause)
				.def("resume", &SoraMusicFile::resume)
				.def("setVolume", &SoraMusicFile::setVolume)
				.def("setPan", &SoraMusicFile::setPan)
				.def("setPitch", &SoraMusicFile::setPitch)
				.def("getPan", &SoraMusicFile::getPan)
				.def("getPitch", &SoraMusicFile::getPitch)
				.def("getVolume", &SoraMusicFile::getVolume)
				.def("setPosition", &SoraMusicFile::setPosition)
				.def("getPosition", &SoraMusicFile::getPosition)
				.def("getLength", &SoraMusicFile::getLength)
				.def("setRepeat", &SoraMusicFile::setRepeat)
				.def("isRepeat", &SoraMusicFile::isRepeat)
				.def("isPlaying", &SoraMusicFile::isPlaying)
				.def("getName", &SoraMusicFile::getName);

			::LuaClass<SoraHttpFile>(state)
				.create("SoraHttpFile")
				.destroy("Free")
				.def("setFile", &SoraHttpFile::setFile)
				.def("readFile", &SoraHttpFile::readFile)
				.def("readFileMem", &SoraHttpFile::readFileMem)
				.def("getDownloadedSize", &SoraHttpFile::getDownloadedSize)
				.def("getDownloadTime", &SoraHttpFile::getDownloadTime)
				.def("getState", &SoraHttpFile::getState)
				.def("suspend", &SoraHttpFile::suspend)
				.def("resume", &SoraHttpFile::resume);*/

		/*	::LuaClass<SoraZipFile>(state)
				.create("SoraZipFile")
			//	.create<const SoraWString&>("SoraZipFileOpen")
				.destroy("Free")
				.def("readFile", &SoraZipFile::readFile)
				.def("readFileMem", &SoraZipFile::readFileMem)
				.def("closeFile", &SoraZipFile::closeFile)
				.def("setPackCode", &SoraZipFile::setPackCode)
				.def("getFile", &SoraZipFile::getFile)
				.def("getFileSize", &SoraZipFile::getFileSize)
				.def("getFileCRC", &SoraZipFile::getFileCRC);

			::LuaClass<SoraINIFile>(state)
				.create("SoraINIFile")
				.destroy("Free")
				.def("readFile", &SoraINIFile::readFile)
				.def("readFileMem", &SoraINIFile::readFileMem)
				.def("getInt", &SoraINIFile::getInt)
				.def("getFloat", &SoraINIFile::getFloat)
				.def("getBool", &SoraINIFile::getBool)
				.def("getString", &SoraINIFile::getString)
				.def("writeInt", &SoraINIFile::writeInt)
				.def("writeFloat", &SoraINIFile::writeFloat)
				.def("writeString", &SoraINIFile::writeString);
		}

		static void export_soraobjects(SoraLuaObject* obj) {
			export_soraobjects(obj->getState());
		}
		static void export_soraobjects(LuaState* state) {
			// some of the functions dont work,
			// to do
			::LuaClass<SoraRenderObject>(state)
				.create("SoraRenderObject")
				.destroy("Free")
				.def("setImage", &SoraRenderObject::getImage)
				.def("getImage", &SoraRenderObject::getImage)
				.def("loadImage", &SoraRenderObject::loadImage)
				.def("setPosition", &SoraRenderObject::setPosition)
				.def("getPositionX", &SoraRenderObject::getPositionX)
				.def("getPositionY", &SoraRenderObject::getPositionY)
				.def("update", &SoraRenderObject::update)
				.def("render", &SoraRenderObject::render)
		//		.def("add", &SoraRenderObject::addlua)
		//		.def("setDestRect", &SoraRenderObject::setDestRect4V)
				.def("setScale", &SoraRenderObject::setScale)
				.def("getVScale", &SoraRenderObject::getVScale)
				.def("getHScale", &SoraRenderObject::getHScale)
				.def("setRotation", &SoraRenderObject::setRotation)
				.def("getRotation", &SoraRenderObject::getRotation)
				.def("setCenter", &SoraRenderObject::setCenter)
				.def("getCenterX", &SoraRenderObject::getCenterX)
				.def("getCenterY", &SoraRenderObject::getCenterY)
				.def("setFlip", &SoraRenderObject::setFlip)
				.def("getVFlip", &SoraRenderObject::getVFlip)
				.def("getHFlip", &SoraRenderObject::getHFlip)
				.def("setColor", &SoraRenderObject::setColor)
				.def("getColor", &SoraRenderObject::getColor)
				.def("setTextureRect", &SoraRenderObject::setTextureRect)
				.def("setZ", &SoraRenderObject::setZ)
				.def("getZ", &SoraRenderObject::getZ)
				.def("setBlendMode", &SoraRenderObject::setBlendMode)
				.def("getBlendMode", &SoraRenderObject::getBlendMode)
				.def("addEffect", &SoraRenderObject::addEffectLua)
				.def("stopEffect", &SoraRenderObject::stopEffectLua);

			::LuaClass<SoraPhysicalObject>(state)
			//	.create("SoraPhysicalObject")
			//	.create<float32, float32, int32>("SoraPhysicalObject")
				.destroy("Free")
				.def("setAsBox", &SoraPhysicalObject::setAsBox)
				.def("setAsCircle", &SoraPhysicalObject::setAsCircle)
				.def("applyForce", &SoraPhysicalObject::applyForce)
				.def("applyForceF", &SoraPhysicalObject::applyForceF)
				.def("getAngle", &SoraPhysicalObject::getAngle)
				.def("getPositionX", &SoraPhysicalObject::getPositionX)
				.def("getPositionY", &SoraPhysicalObject::getPositionY)
				.def("getSpeedX", &SoraPhysicalObject::getSpeedX)
				.def("getSpeedY", &SoraPhysicalObject::getSpeedY)
				.def("getMass", &SoraPhysicalObject::getMass)
				.def("setMass", &SoraPhysicalObject::setMass)
				.def("setAngularSpeed", &SoraPhysicalObject::setAngularSpeed)
				.def("getAngularSpeed", &SoraPhysicalObject::getAngularSpeed)
				.def("setBullet", &SoraPhysicalObject::setBullet)
				.def("isBullet", &SoraPhysicalObject::isBullet)
				.def("setSleep", &SoraPhysicalObject::setSleep)
				.def("isSleep", &SoraPhysicalObject::isSleep)
				.def("setActive", &SoraPhysicalObject::setActive)
				.def("isActive", &SoraPhysicalObject::isActive)
			//	.def("add", &SoraPhysicalObject::addlua)
				.def("setLocalAnchor", &SoraPhysicalObject::setLocalAnchor)
				.def("getLocalAnchorX", &SoraPhysicalObject::getLocalAnchorX)
				.def("getLocalAnchorY", &SoraPhysicalObject::getLocalAnchorY);

			/**::LuaClass<SoraLuaObject>(state)
				.create("SoraLuaObject")
				.create<const SoraWString&>("SoraLuaObjectFile")
				.create<LuaState*>("SoraLuaObjectState")
				.destroy("Free")
				.def("doScript", &SoraLuaObject::doScript)
				.def("doString", &SoraLuaObject::doString)
				.def("doBuffer", &SoraLuaObject::doBuffer)
				.def("loadString", &SoraLuaObject::loadString)
				.def("loadBuffer", &SoraLuaObject::loadBuffer)
				.def("loadScript", &SoraLuaObject::loadString)
				.def("createThread", &SoraLuaObject::createThread)
				.def("createTable", &SoraLuaObject::createTable)
				.def("createUserData", &SoraLuaObject::createUserData)
				.def("getGlobals", &SoraLuaObject::getGlobals)
				.def("get", &SoraLuaObject::get)
				.def("getInteger", &SoraLuaObject::getInteger)
				.def("getFlaot", &SoraLuaObject::getFloat)
				.def("getDouble", &SoraLuaObject::getDouble)
				.def("getString", &SoraLuaObject::getString)
				.def("getCFunction", &SoraLuaObject::getCFunction)
				.def("getUserData", &SoraLuaObject::getUserData)
				.def("getBoolean", &SoraLuaObject::getBoolean)
				.def("getState", &SoraLuaObject::getState);*/
			
		/*	::LuaClass<SoraParticleManager>(state)
				.create("SoraParticleManager")
				.destroy("Free")
				.def("emit", &SoraParticleManager::emitS)
				.def("setGlobalSprite", &SoraParticleManager::setGlobalSprite)
				.def("isActive", &SoraParticleManager::isActive)
				.def("kill", &SoraParticleManager::kill)
				.def("stop", &SoraParticleManager::stop)
				.def("restart", &SoraParticleManager::restart)
				.def("fire", &SoraParticleManager::fire)
				.def("fireAt", &SoraParticleManager::fireAt)
				.def("killAll", &SoraParticleManager::killAll)
				.def("getParticleAlive", &SoraParticleManager::getParticleAlive)
				.def("getTotalParticleAlive", &SoraParticleManager::getTotalParticleAlive)
				.def("render", &SoraParticleManager::render)
				.def("update", &SoraParticleManager::update)
				.def("save", &SoraParticleManager::save)
				.def("size", &SoraParticleManager::size);
			/*
			LuaModule(state, "spritelib")
				.def("setTextureRect", &spritewrapper::setTextureRect)
				.def("setColor", &spritewrapper::setColor)
				.def("setZ", &spritewrapper::setZ)
				.def("getColor", &spritewrapper::getColor)
				.def("getZ", &spritewrapper::getZ)
				.def("setCenter", &spritewrapper::setCenter)
				.def("getCenterX", &spritewrapper::getCenterX)
				.def("getCenterY", &spritewrapper::getCenterY)
				.def("setFlip", &spritewrapper::setFlip)
				.def("getVFlip", &spritewrapper::getVFlip)
				.def("getHFlip", &spritewrapper::getHFlip)
				.def("getTextureWidth", &spritewrapper::getTextureWidth)
				.def("getTextureHeight", &spritewrapper::getTextureHeight)
				.def("getSpriteWidth", &spritewrapper::getSpriteWidth)
				.def("getSpriteHeight", &spritewrapper::getSpriteHeight)
				.def("setScale", &spritewrapper::setScale)
				.def("setRotation", &spritewrapper::setRotation)
				.def("getVScale", &spritewrapper::getVScale)
				.def("getHScale", &spritewrapper::getHScale)
				.def("getRotation", &spritewrapper::getRotation)
				.def("getPixel", &spritewrapper::getPixel)
				.def("setPixel", &spritewrapper::setPixel)
				.def("setBlendMode", &spritewrapper::setBlendMode)
				.def("getBlendMode", &spritewrapper::getBlendMode)

				
				.def("gauseBlurNew", &spritewrapper::gaussBlurNew)
				.def("gaussBlur", &spritewrapper::gaussBlur)
				.def("grayNew", &spritewrapper::grayNew)
				.def("gray", &spritewrapper::gray)
				.def("reverse", &spritewrapper::reverse)
				.def("reverseNew", &spritewrapper::reverseNew)
				.def("alphaMixNew", &spritewrapper::alphaMixNew)
				.def("alphaMix", &spritewrapper::alphaMix)

				.def("release", &spritewrapper::release);

			LuaModule(state, "guilib")
				.def("setWidgetFrameSize", &guiwrapper::setWidgetFrameSize)
				.def("getWidgetFrameSize", &guiwrapper::getWidgetFrameSize)
				.def("getWidgetParent", &guiwrapper::getWidgetParent)
				.def("setWidgetWidth", &guiwrapper::setWidgetWidth)
				.def("setWidgetHeight", &guiwrapper::setWidgetHeight)
				.def("setWidgetSize", &guiwrapper::setWidgetSize)
				.def("getWidgetWidth", &guiwrapper::getWidgetWidth)
				.def("getWidgetHeight", &guiwrapper::getWidgetHeight)
				.def("setWidgetX", &guiwrapper::setWidgetX)
				.def("setWidgetY", &guiwrapper::setWidgetY)
				.def("setWidgetPos", &guiwrapper::setWidgetPos)
				.def("getWidgetX", &guiwrapper::getWidgetX)
				.def("getWidgetY", &guiwrapper::getWidgetY)
				.def("setWidgetFocusable", &guiwrapper::setWidgetFocusable)
				.def("isWidgetFocusable", &guiwrapper::isWidgetFocusable)
				.def("isWidgetFocused", &guiwrapper::isWidgetFocused)
				.def("setWidgetVisible", &guiwrapper::setWidgetVisible)
				.def("isWidgetVisible", &guiwrapper::isWidgetVisible)
				.def("setWidgetBaseColor", &guiwrapper::setWidgetBaseColor)
				.def("setWidgetForegroundColor", &guiwrapper::setWidgetForegroundColor)
				.def("setWidgetBackgroundColor", &guiwrapper::setWidgetBackgroundColor)
				.def("setWidgetSelectionColor", &guiwrapper::setWidgetSelectionColor)
				.def("requestFocus", &guiwrapper::requestWidgetFocus)
				.def("requestWidgetModalFocus", &guiwrapper::requestWidgetModalFocus)
				.def("requestWidgetModalMouseInputFocus", &guiwrapper::requestWidgetModalMouseInputFocus)
				.def("releaseWidgetModalFocus", &guiwrapper::releaseWidgetModalFocus)
				.def("releaseWidgetModalMouseInputFocus", &guiwrapper::releaseWidgetModalMouseInputFocus)
				.def("isWidgetModalFocused", &guiwrapper::isWidgetModalFocused)
				.def("isWidgetModalMouseInputFocused", &guiwrapper::isWidgetModalMouseInputFocused)
				.def("getWidgetAt", &guiwrapper::getWidgetAt)
				.def("setWidgetTabInEnabled", &guiwrapper::setWidgetTabInEnabled)
				.def("setWidgetTabOutEnabled", &guiwrapper::setWidgetTabOutEnabled)
				.def("isWidgetTabInEnabled", &guiwrapper::isWidgetTabInEnabled)
				.def("isWidgetTabOutEnabled", &guiwrapper::isWidgetTabOutEnabled)
				.def("setWidgetId", &guiwrapper::setWidgetId)
				.def("getWidgetId", &guiwrapper::getWidgetId)
				.def("showWidgetPart", &guiwrapper::showWidgetPart)
				.def("setWidgetOpaque", &guiwrapper::setWidgetOpaque)
				.def("setWidgetCaption", &guiwrapper::setWidgetCaption)
				.def("setWidgetValue", &guiwrapper::setWidgetValue);

			LuaModule(state, "imageeffect")
				.def("makeEffectFade", &imageeffectwrapper::makeEffect_fade)
				.def("makeEffectTransition", &imageeffectwrapper::makeEffect_transitions)
				.def("makeEffectScale", &imageeffectwrapper::makeEffect_scale)
				.def("makeEffectTensile", &imageeffectwrapper::makeEffect_scale_tensile);
*/
		}

		static void export_constants(SoraLuaObject* state) {
			export_constants(state->getState());
		}
		static void export_constants(LuaState* state) {
			LuaObject global = state->GetGlobals();
			global.SetInteger("SORA_KEY_LBUTTON",	0x01);
			global.SetInteger("SORA_KEY_RBUTTON",	0x02);
			global.SetInteger("SORA_KEY_MBUTTON",	0x04);

			global.SetInteger("SORA_KEY_ESCAPE",	0x1B);
			global.SetInteger("SORA_KEY_BACKSPACE",	0x08);
			global.SetInteger("SORA_KEY_TAB",		0x09);
			global.SetInteger("SORA_KEY_ENTER",		0x0D);
			global.SetInteger("SORA_KEY_SPACE",		0x20);

			global.SetInteger("SORA_KEY_SHIFT",		0x10);
			global.SetInteger("SORA_KEY_CTRL",		0x11);
			global.SetInteger("SORA_KEY_ALT",		0x12);

			global.SetInteger("SORA_KEY_LWIN",		0x5B);
			global.SetInteger("SORA_KEY_RWIN",		0x5C);
			global.SetInteger("SORA_KEY_APPS",		0x5D);

			global.SetInteger("SORA_KEY_PAUSE",		0x13);
			global.SetInteger("SORA_KEY_CAPSLOCK",	0x14);
			global.SetInteger("SORA_KEY_NUMLOCK",	0x90);
			global.SetInteger("SORA_KEY_SCROLLLOCK", 0x91);

			global.SetInteger("SORA_KEY_PGUP",		0x21);
			global.SetInteger("SORA_KEY_PGDN",		0x22);
			global.SetInteger("SORA_KEY_HOME",		0x24);
			global.SetInteger("SORA_KEY_END",		0x23);
			global.SetInteger("SORA_KEY_INSERT",	0x2D);
			global.SetInteger("SORA_KEY_DELETE",	0x2E);

			global.SetInteger("SORA_KEY_LEFT",		0x25);
			global.SetInteger("SORA_KEY_UP",		0x26);
			global.SetInteger("SORA_KEY_RIGHT",		0x27);
			global.SetInteger("SORA_KEY_DOWN",		0x28);

			global.SetInteger("SORA_KEY_0",			0x30);
			global.SetInteger("SORA_KEY_1",			0x31);
			global.SetInteger("SORA_KEY_2",			0x32);
			global.SetInteger("SORA_KEY_3",			0x33);
			global.SetInteger("SORA_KEY_4",			0x34);
			global.SetInteger("SORA_KEY_5",			0x35);
			global.SetInteger("SORA_KEY_6",			0x36);
			global.SetInteger("SORA_KEY_7",			0x37);
			global.SetInteger("SORA_KEY_8",			0x38);
			global.SetInteger("SORA_KEY_9",			0x39);

			global.SetInteger("SORA_KEY_A",			0x41);
			global.SetInteger("SORA_KEY_B",			0x42);
			global.SetInteger("SORA_KEY_C",			0x43);
			global.SetInteger("SORA_KEY_D",			0x44);
			global.SetInteger("SORA_KEY_E",			0x45);
			global.SetInteger("SORA_KEY_F",			0x46);
			global.SetInteger("SORA_KEY_G",			0x47);
			global.SetInteger("SORA_KEY_H",			0x48);
			global.SetInteger("SORA_KEY_I",			0x49);
			global.SetInteger("SORA_KEY_J",			0x4A);
			global.SetInteger("SORA_KEY_K",			0x4B);
			global.SetInteger("SORA_KEY_L",			0x4C);
			global.SetInteger("SORA_KEY_M",			0x4D);
			global.SetInteger("SORA_KEY_N",			0x4E);
			global.SetInteger("SORA_KEY_O",			0x4F);
			global.SetInteger("SORA_KEY_P",			0x50);
			global.SetInteger("SORA_KEY_Q",			0x51);
			global.SetInteger("SORA_KEY_R",			0x52);
			global.SetInteger("SORA_KEY_S",			0x53);
			global.SetInteger("SORA_KEY_T",			0x54);
			global.SetInteger("SORA_KEY_U",			0x55);
			global.SetInteger("SORA_KEY_V",			0x56);
			global.SetInteger("SORA_KEY_W",			0x57);
			global.SetInteger("SORA_KEY_X",			0x58);
			global.SetInteger("SORA_KEY_Y",			0x59);
			global.SetInteger("SORA_KEY_Z",			0x5A);

			global.SetInteger("SORA_KEY_GRAVE",		0xC0);
			global.SetInteger("SORA_KEY_MINUS",		0xBD);
			global.SetInteger("SORA_KEY_EQUALS",	0xBB);
			global.SetInteger("SORA_KEY_BACKSLASH",	0xDC);
			global.SetInteger("SORA_KEY_LBRACKET",	0xDB);
			global.SetInteger("SORA_KEY_RBRACKET",	0xDD);
			global.SetInteger("SORA_KEY_SEMICOLON",	0xBA);
			global.SetInteger("SORA_KEY_APOSTROPHE",0xDE);
			global.SetInteger("SORA_KEY_COMMA",		0xBC);
			global.SetInteger("SORA_KEY_PERIOD",	0xBE);
			global.SetInteger("SORA_KEY_SLASH",		0xBF);

			global.SetInteger("SORA_KEY_NUMPAD0",	0x60);
			global.SetInteger("SORA_KEY_NUMPAD1",	0x61);
			global.SetInteger("SORA_KEY_NUMPAD2",	0x62);
			global.SetInteger("SORA_KEY_NUMPAD3",	0x63);
			global.SetInteger("SORA_KEY_NUMPAD4",	0x64);
			global.SetInteger("SORA_KEY_NUMPAD5",	0x65);
			global.SetInteger("SORA_KEY_NUMPAD6",	0x66);
			global.SetInteger("SORA_KEY_NUMPAD7",	0x67);
			global.SetInteger("SORA_KEY_NUMPAD8",	0x68);
			global.SetInteger("SORA_KEY_NUMPAD9",	0x69);

			global.SetInteger("SORA_KEY_MULTIPLY",	0x6A);
			global.SetInteger("SORA_KEY_DIVIDE",	0x6F);
			global.SetInteger("SORA_KEY_ADD",		0x6B);
			global.SetInteger("SORA_KEY_SUBTRACT",	0x6D);
			global.SetInteger("SORA_KEY_DECIMAL",	0x6E);

			global.SetInteger("SORA_KEY_F1",		0x70);
			global.SetInteger("SORA_KEY_F2",		0x71);
			global.SetInteger("SORA_KEY_F3",		0x72);
			global.SetInteger("SORA_KEY_F4",		0x73);
			global.SetInteger("SORA_KEY_F5",		0x74);
			global.SetInteger("SORA_KEY_F6",		0x75);
			global.SetInteger("SORA_KEY_F7",		0x76);
			global.SetInteger("SORA_KEY_F8",		0x77);
			global.SetInteger("SORA_KEY_F9",		0x78);
			global.SetInteger("SORA_KEY_F10",		0x79);
			global.SetInteger("SORA_KEY_F11",		0x7A);
			global.SetInteger("SORA_KEY_F12",		0x7B);

			global.SetInteger("SORA_INPUT_KEYDOWN",		1);
			global.SetInteger("SORA_INPUT_KEYUP",		2);
			global.SetInteger("SORA_INPUT_MBUTTONDOWN",	3);
			global.SetInteger("SORA_INPUT_MBUTTONUP",	4);
			global.SetInteger("SORA_INPUT_MOUSEMOVE",	5);
			global.SetInteger("SORA_INPUT_MOUSEWHEEL",	6);

			global.SetInteger("SORA_INPUT_FLAG_SHIFT",	1);
			global.SetInteger("SORA_INPUT_FLAG_CTRL",	2);
			global.SetInteger("SORA_INPUT_FLAG_ALT",	4);
			global.SetInteger("SORA_INPUT_FLAG_CAPSLOCK",8);
			global.SetInteger("SORA_INPUT_FLAG_SCROLLLOCK",	16);
			global.SetInteger("SORA_INPUT_FLAG_NUMLOCK",	32);
			global.SetInteger("SORA_INPUT_FLAG_REPEAT",		64);

			global.SetInteger("SORA_DIKEY_PRESSED",	0x0);
			global.SetInteger("SORA_DIKEY_UP",		0x10);
			global.SetInteger("SORA_DIKEY_DOWN",	0x20);

			global.SetInteger("SORA_JOY_LEFT",		0x40);
			global.SetInteger("SORA_JOY_RIGHT",		0x50);
			global.SetInteger("SORA_JOY_UP",		0x60);
			global.SetInteger("SORA_JOY_DOWN",		0x70);

			global.SetInteger("MB_OK", MB_OK);

			global.SetInteger("IMAGE_EFFECT_ONCE", IMAGE_EFFECT_ONCE);
			global.SetInteger("IMAGE_EFFECT_PINGPONG", IMAGE_EFFECT_PINGPONG);
			global.SetInteger("IMAGE_EFFECT_REPEAT", IMAGE_EFFECT_REPEAT);

			global.SetInteger("BLEND_COLORADD", BLEND_COLORADD);
			global.SetInteger("BLEND_COLORMUL", BLEND_COLORMUL);
			global.SetInteger("BLEND_ALPHABLEND", BLEND_ALPHABLEND);
			global.SetInteger("BLEND_ALPHAADD", BLEND_ALPHAADD);
			global.SetInteger("BLEND_ZWRITE", BLEND_ZWRITE);
			global.SetInteger("BLEND_NOZWRITE", BLEND_NOZWRITE);
		}

	};

} // namespace sora

#endif