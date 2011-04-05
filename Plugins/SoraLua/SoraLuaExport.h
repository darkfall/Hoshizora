#ifndef SORA_LUA_EXPORT
#define SORA_LUA_EXPORT

#include "luaclass.h"
#include "LuaPlusHelper.h"
#include "SoraCore.h"

#include <vector>
#include <cassert>

#include "spriteWrapper.h"
#include "SoraEventManager.h"

namespace sora { 

	class SoraLuaExport {
	public:
		static void export_soracore(SoraLuaObject* state) {
			export_soracore(state->getState());
		}
		static void export_soracore(LuaState* state) {
            assert(state != NULL);
            
			SoraCore* pCore = SoraCore::Instance();
			gge::LuaClass<SoraCore>(state, "SoraCore", pCore)
            .def("setFPS", &SoraCore::setFPS)
            .def("setTimeScale", &SoraCore::setTimeScale)
            .def("beginScene", &SoraCore::beginScene)
            .def("endScene", &SoraCore::endScene)
            .def("createTarget", &SoraCore::createTarget)
            .def("freeTarget", &SoraCore::freeTarget)
            .def("getTargetTexture", &SoraCore::getTargetTexture)
            .def("createTexture", &SoraCore::createTexture)
            .def("createTextureWH", &SoraCore::createTextureWH)
            .def("setClipping", &SoraCore::setClipping)
            .def("setTransform", &SoraCore::setTransform)
            .def("setRandomSeed", &SoraCore::setRandomSeed)
            .def("getRandomSeed", &SoraCore::getRandomSeed)
            .def("randomInt", &SoraCore::randomInt)
            .def("randomFloat", &SoraCore::randomFloat)
            .def("getScreenWidth", &SoraCore::getScreenWidth)
            .def("getScreenHeight", &SoraCore::getScreenHeight)
            .def("loadResourcePack", &SoraCore::loadResourcePack)
            .def("attachResourcePack", &SoraCore::attachResourcePack)
            .def("setWindowTitle", &SoraCore::setWindowTitle)
            .def("setWindowPos", &SoraCore::setWindowPos)
            .def("setFullScreen", &SoraCore::setFullscreen)
            .def("isFullScreen", &SoraCore::isFullscreen)
            .def("getMousePosX", &SoraCore::getMousePosX)
            .def("getMousePosY", &SoraCore::getMousePosY)
            .def("setMousePos", &SoraCore::setMousePos)
            .def("getMouseWheel", &SoraCore::getMouseWheel)
            .def("keyDown", &SoraCore::keyDown)
            .def("keyUp", &SoraCore::keyUp)
            .def("getKeyState", &SoraCore::getKeyState)
            .def("messageBox", &SoraCore::messageBox)
            .def("log", &SoraCore::log)
            .def("execute", &SoraCore::execute)
            .def("setFrameSync", &SoraCore::setFrameSync);
		}
        

		static void export_sprites(SoraLuaObject* obj) {
            return export_sprites(obj->getState());
        }
        
        static void export_sprites(LuaState* state) {
            assert(state != NULL);
    
			LuaModule(state, "spritelib")
                .def("createSprite", &spritewrapper::createSprite)
                .def("createSpriteWithTex", &spritewrapper::createSpriteWithTex)
                .def("createSpriteWithTexEx", &spritewrapper::createSpriteWithTexEx)
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
				.def("setBlendMode", &spritewrapper::setBlendMode)
				.def("getBlendMode", &spritewrapper::getBlendMode)
                .def("addEffect", &spritewrapper::addEffect)
                .def("stopEffect", &spritewrapper::stopEffect)
                .def("clearEffects", &spritewrapper::clearEffects)
                .def("hasEffect", &spritewrapper::hasEffect)
                .def("update", &spritewrapper::update)
                .def("render", &spritewrapper::render)
                .def("renderWithPos", &spritewrapper::renderWithPos)
                .def("setPosition", &spritewrapper::setPosition)
                .def("getPosX", &spritewrapper::getPosX)
                .def("getPosY", &spritewrapper::getPosY)
				.def("release", &spritewrapper::release)
            
				.def("makeEffectFade", &imageeffectwrapper::makeEffect_fade)
				.def("makeEffectTransition", &imageeffectwrapper::makeEffect_transitions)
				.def("makeEffectScale", &imageeffectwrapper::makeEffect_scale)
				.def("makeEffectTensile", &imageeffectwrapper::makeEffect_scale_tensile)
                .def("makeEffectColorTransitions", &imageeffectwrapper::makeEffect_colorTransitions)
                .def("makeEffectRotation", &imageeffectwrapper::makeEffect_rotation);
		}
        
       

		static void export_constants(SoraLuaObject* state) {
			export_constants(state->getState());
		}
		static void export_constants(LuaState* state) {
            assert(state != NULL);
            
			LuaObject global = state->GetGlobals();
			global.SetInteger("SORA_KEY_LBUTTON",	SORA_KEY_LBUTTON);
			global.SetInteger("SORA_KEY_RBUTTON",	SORA_KEY_RBUTTON);
			global.SetInteger("SORA_KEY_MBUTTON",	SORA_KEY_MBUTTON);

			global.SetInteger("SORA_KEY_ESCAPE",	SORA_KEY_ESCAPE);
			global.SetInteger("SORA_KEY_BACKSPACE",	SORA_KEY_BACKSPACE);
			global.SetInteger("SORA_KEY_TAB",		SORA_KEY_TAB);
			global.SetInteger("SORA_KEY_ENTER",		SORA_KEY_ENTER);
			global.SetInteger("SORA_KEY_SPACE",		SORA_KEY_SPACE);

			global.SetInteger("SORA_KEY_SHIFT",		SORA_KEY_SHIFT);
			global.SetInteger("SORA_KEY_CTRL",		SORA_KEY_CTRL);
			global.SetInteger("SORA_KEY_CTRL",		SORA_KEY_CTRL);

			global.SetInteger("SORA_KEY_CTRL",		SORA_KEY_CTRL);
			global.SetInteger("SORA_KEY_RWIN",		SORA_KEY_RWIN);
			global.SetInteger("SORA_KEY_APPS",		SORA_KEY_APPS);

			global.SetInteger("SORA_KEY_PAUSE",		SORA_KEY_PAUSE);
			global.SetInteger("SORA_KEY_CAPSLOCK",	SORA_KEY_CAPSLOCK);
			global.SetInteger("SORA_KEY_NUMLOCK",	SORA_KEY_CAPSLOCK);
			global.SetInteger("SORA_KEY_SCROLLLOCK", SORA_KEY_SCROLLLOCK);

			global.SetInteger("SORA_KEY_PGUP",		SORA_KEY_PGUP);
			global.SetInteger("SORA_KEY_PGDN",		SORA_KEY_PGDN);
			global.SetInteger("SORA_KEY_HOME",		SORA_KEY_HOME);
			global.SetInteger("SORA_KEY_END",		SORA_KEY_END);
			global.SetInteger("SORA_KEY_INSERT",	SORA_KEY_INSERT);
			global.SetInteger("SORA_KEY_DELETE",	SORA_KEY_DELETE);

			global.SetInteger("SORA_KEY_LEFT",		SORA_KEY_LEFT);
			global.SetInteger("SORA_KEY_UP",		SORA_KEY_UP);
			global.SetInteger("SORA_KEY_RIGHT",		SORA_KEY_RIGHT);
			global.SetInteger("SORA_KEY_DOWN",		SORA_KEY_DOWN);

			global.SetInteger("SORA_KEY_0",			SORA_KEY_0);
			global.SetInteger("SORA_KEY_1",			SORA_KEY_1);
			global.SetInteger("SORA_KEY_2",			SORA_KEY_2);
			global.SetInteger("SORA_KEY_3",			SORA_KEY_3);
			global.SetInteger("SORA_KEY_4",			SORA_KEY_4);
			global.SetInteger("SORA_KEY_5",			SORA_KEY_5);
			global.SetInteger("SORA_KEY_6",			SORA_KEY_6);
			global.SetInteger("SORA_KEY_7",			SORA_KEY_7);
			global.SetInteger("SORA_KEY_8",			SORA_KEY_8);
			global.SetInteger("SORA_KEY_9",			SORA_KEY_9);

			global.SetInteger("SORA_KEY_A",			SORA_KEY_A);
			global.SetInteger("SORA_KEY_B",			SORA_KEY_B);
			global.SetInteger("SORA_KEY_C",			SORA_KEY_C);
			global.SetInteger("SORA_KEY_D",			SORA_KEY_D);
			global.SetInteger("SORA_KEY_E",			SORA_KEY_E);
			global.SetInteger("SORA_KEY_F",			SORA_KEY_F);
			global.SetInteger("SORA_KEY_G",			SORA_KEY_G);
			global.SetInteger("SORA_KEY_H",			SORA_KEY_H);
			global.SetInteger("SORA_KEY_I",			SORA_KEY_I);
			global.SetInteger("SORA_KEY_J",			SORA_KEY_J);
			global.SetInteger("SORA_KEY_K",			SORA_KEY_K);
			global.SetInteger("SORA_KEY_L",			SORA_KEY_L);
			global.SetInteger("SORA_KEY_M",			SORA_KEY_M);
			global.SetInteger("SORA_KEY_N",			SORA_KEY_N);
			global.SetInteger("SORA_KEY_O",			SORA_KEY_O);
			global.SetInteger("SORA_KEY_P",			SORA_KEY_P);
			global.SetInteger("SORA_KEY_Q",			SORA_KEY_Q);
			global.SetInteger("SORA_KEY_R",			SORA_KEY_R);
			global.SetInteger("SORA_KEY_S",			SORA_KEY_S);
			global.SetInteger("SORA_KEY_T",			SORA_KEY_T);
			global.SetInteger("SORA_KEY_U",			SORA_KEY_U);
			global.SetInteger("SORA_KEY_V",			SORA_KEY_V);
			global.SetInteger("SORA_KEY_W",			SORA_KEY_W);
			global.SetInteger("SORA_KEY_X",			SORA_KEY_X);
			global.SetInteger("SORA_KEY_Y",			SORA_KEY_Y);
			global.SetInteger("SORA_KEY_Z",			SORA_KEY_Z);

			global.SetInteger("SORA_KEY_GRAVE",		SORA_KEY_GRAVE);
			global.SetInteger("SORA_KEY_MINUS",		SORA_KEY_MINUS);
			global.SetInteger("SORA_KEY_EQUALS",	SORA_KEY_EQUALS);
			global.SetInteger("SORA_KEY_BACKSLASH",	SORA_KEY_BACKSLASH);
			global.SetInteger("SORA_KEY_LBRACKET",	SORA_KEY_LBRACKET);
			global.SetInteger("SORA_KEY_RBRACKET",	SORA_KEY_RBRACKET);
			global.SetInteger("SORA_KEY_SEMICOLON",	SORA_KEY_SEMICOLON);
			global.SetInteger("SORA_KEY_APOSTROPHE",SORA_KEY_APOSTROPHE);
			global.SetInteger("SORA_KEY_COMMA",		SORA_KEY_COMMA);
			global.SetInteger("SORA_KEY_PERIOD",	SORA_KEY_PERIOD);
			global.SetInteger("SORA_KEY_SLASH",		SORA_KEY_SLASH);

			global.SetInteger("SORA_KEY_NUMPAD0",	SORA_KEY_NUMPAD0);
			global.SetInteger("SORA_KEY_NUMPAD1",	SORA_KEY_NUMPAD1);
			global.SetInteger("SORA_KEY_NUMPAD2",	SORA_KEY_NUMPAD2);
			global.SetInteger("SORA_KEY_NUMPAD3",	SORA_KEY_NUMPAD3);
			global.SetInteger("SORA_KEY_NUMPAD4",	SORA_KEY_NUMPAD4);
			global.SetInteger("SORA_KEY_NUMPAD5",	SORA_KEY_NUMPAD5);
			global.SetInteger("SORA_KEY_NUMPAD6",	SORA_KEY_NUMPAD6);
			global.SetInteger("SORA_KEY_NUMPAD7",	SORA_KEY_NUMPAD7);
			global.SetInteger("SORA_KEY_NUMPAD8",	SORA_KEY_NUMPAD8);
			global.SetInteger("SORA_KEY_NUMPAD9",	SORA_KEY_NUMPAD9);

			global.SetInteger("SORA_KEY_MULTIPLY",	SORA_KEY_MULTIPLY);
			global.SetInteger("SORA_KEY_DIVIDE",	SORA_KEY_DIVIDE);
			global.SetInteger("SORA_KEY_ADD",		SORA_KEY_ADD);
			global.SetInteger("SORA_KEY_SUBTRACT",	SORA_KEY_SUBTRACT);
			global.SetInteger("SORA_KEY_DECIMAL",	SORA_KEY_DECIMAL);

			global.SetInteger("SORA_KEY_F1",		SORA_KEY_F1);
			global.SetInteger("SORA_KEY_F2",		SORA_KEY_F2);
			global.SetInteger("SORA_KEY_F3",		SORA_KEY_F3);
			global.SetInteger("SORA_KEY_F4",		SORA_KEY_F4);
			global.SetInteger("SORA_KEY_F5",		SORA_KEY_F5);
			global.SetInteger("SORA_KEY_F6",		SORA_KEY_F6);
			global.SetInteger("SORA_KEY_F7",		SORA_KEY_F7);
			global.SetInteger("SORA_KEY_F8",		SORA_KEY_F8);
			global.SetInteger("SORA_KEY_F9",		SORA_KEY_F9);
			global.SetInteger("SORA_KEY_F10",		SORA_KEY_F10);
			global.SetInteger("SORA_KEY_F11",		SORA_KEY_F11);
			global.SetInteger("SORA_KEY_F12",		SORA_KEY_F12);

			global.SetInteger("SORA_INPUT_KEYDOWN",		SORA_INPUT_KEYDOWN);
			global.SetInteger("SORA_INPUT_KEYUP",		SORA_INPUT_KEYUP);
			global.SetInteger("SORA_INPUT_MBUTTONDOWN",	SORA_INPUT_MBUTTONDOWN);
			global.SetInteger("SORA_INPUT_MBUTTONUP",	SORA_INPUT_MBUTTONUP);
			global.SetInteger("SORA_INPUT_MOUSEMOVE",	SORA_INPUT_MOUSEMOVE);
			global.SetInteger("SORA_INPUT_MOUSEWHEEL",	SORA_INPUT_MOUSEWHEEL);

			global.SetInteger("SORA_INPUT_FLAG_SHIFT",	SORA_INPUT_FLAG_SHIFT);
			global.SetInteger("SORA_INPUT_FLAG_CTRL",	SORA_INPUT_FLAG_CTRL);
			global.SetInteger("SORA_INPUT_FLAG_ALT",	SORA_INPUT_FLAG_ALT);
			global.SetInteger("SORA_INPUT_FLAG_CAPSLOCK",SORA_INPUT_FLAG_CAPSLOCK);
			global.SetInteger("SORA_INPUT_FLAG_SCROLLLOCK",	SORA_INPUT_FLAG_SCROLLLOCK);
			global.SetInteger("SORA_INPUT_FLAG_NUMLOCK",	SORA_INPUT_FLAG_NUMLOCK);
			global.SetInteger("SORA_INPUT_FLAG_REPEAT",		SORA_INPUT_FLAG_REPEAT);

			global.SetInteger("SORA_DIKEY_PRESSED",	SORA_DIKEY_PRESSED);
			global.SetInteger("SORA_DIKEY_UP",		SORA_DIKEY_UP);
			global.SetInteger("SORA_DIKEY_DOWN",	SORA_DIKEY_DOWN);

			global.SetInteger("SORA_JOY_LEFT",		SORA_JOY_LEFT);
			global.SetInteger("SORA_JOY_RIGHT",		SORA_JOY_RIGHT);
			global.SetInteger("SORA_JOY_UP",		SORA_JOY_UP);
			global.SetInteger("SORA_JOY_DOWN",		SORA_JOY_DOWN);

			global.SetInteger("MB_OK", MB_OK);
            global.SetInteger("MB_ICONERROR", MB_ICONERROR);
            global.SetInteger("MB_OKCANCEL", MB_OKCANCEL);
            global.SetInteger("MB_ICONWARNING", MB_ICONWARNING);
            global.SetInteger("MB_ICONSTOP", MB_ICONSTOP);
            global.SetInteger("MB_ICONERROR", MB_ICONERROR);

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