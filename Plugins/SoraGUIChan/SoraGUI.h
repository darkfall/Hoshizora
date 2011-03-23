#ifndef SORA_GUI_H
#define SORA_GUI_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraGUIListener.h"

#include <map>

namespace sora {

	enum SoraGUIResponseType {
		RESPONCE_ACTION = 1,
		RESPONCE_KEY = 2,
		RESPONCE_DEATH = 4,
		RESPONCE_FOCUS = 8,
		RESPONCE_MOUSE = 16,
		RESPONCE_LUA = 32,
		RESPONCE_ALL = 31,
	};

//	const int RESPONCE_ALL = RESPONCE_ACTION | RESPONCE_KEY | RESPONCE_DEATH | RESPONCE_FOCUS | RESPONCE_MOUSE;

	class SoraGUI: public SoraSingleton<SoraGUI> {
		friend class SoraSingleton<SoraGUI>;

	public:
		SoraGUI() {
			pActionListener = new SoraGUIActionListener;
			pKeyListener = new SoraGUIKeyListener;
			pDeathListener = new SoraGUIDeathListener;
			pFocusListener = new SoraGUIFocusListener;
			pMouseListener = new SoraGUIMouseListener;
			pLuaActionListener = new SoraLuaGUIActionListener;
		}
        
		void registerGUIResponser(gcn::Widget* pWidget, SoraGUIResponser* pResponser, const SoraString& responserID, int iResponceType) {
			if(iResponceType & RESPONCE_ACTION) pWidget->addActionListener(pActionListener);
			if(iResponceType & RESPONCE_DEATH) pWidget->addDeathListener(pDeathListener);
			if(iResponceType & RESPONCE_KEY) pWidget->addKeyListener(pKeyListener);
			if(iResponceType & RESPONCE_MOUSE) pWidget->addMouseListener(pMouseListener);
			if(iResponceType & RESPONCE_FOCUS) pWidget->addFocusListener(pFocusListener);
			SoraGUIResponserMap::Instance()->registerResponser(responserID, pResponser);
		}
        
		void registerLuaGUIResponser(gcn::Widget* pWidget, const SoraString& sScript) {
			SoraGUIResponserMap::Instance()->registerLuaResponser((ulong32)pWidget, sScript);
			pWidget->addActionListener(pLuaActionListener);
		}

	private:
		std::map<SoraString, SoraGUIResponser*> responserMap;

		SoraGUIActionListener* pActionListener;
		SoraLuaGUIActionListener* pLuaActionListener;
		SoraGUIKeyListener* pKeyListener;
		SoraGUIDeathListener* pDeathListener;
		SoraGUIFocusListener* pFocusListener;
		SoraGUIMouseListener* pMouseListener;
	};
} // namespace sora

#endif