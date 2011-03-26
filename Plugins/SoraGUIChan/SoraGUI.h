#ifndef SORA_GUI_H
#define SORA_GUI_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraGUIListener.h"
#include "SoraGUIResponser.h"

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
        friend class XmlGui;
		friend class SoraSingleton<SoraGUI>;
        
    public:
		SoraGUI() {
			pActionListener = new SoraGUIActionListener;
			pKeyListener = new SoraGUIKeyListener;
			pDeathListener = new SoraGUIDeathListener;
			pFocusListener = new SoraGUIFocusListener;
			pMouseListener = new SoraGUIMouseListener;
		}
        
		void registerGUIResponser(gcn::Widget* pWidget, SoraGUIResponser* pResponser, const SoraString& responserID, int iResponceType) {
			if(iResponceType & RESPONCE_ACTION) pWidget->addActionListener(pActionListener);
			if(iResponceType & RESPONCE_DEATH) pWidget->addDeathListener(pDeathListener);
			if(iResponceType & RESPONCE_KEY) pWidget->addKeyListener(pKeyListener);
			if(iResponceType & RESPONCE_MOUSE) pWidget->addMouseListener(pMouseListener);
			if(iResponceType & RESPONCE_FOCUS) pWidget->addFocusListener(pFocusListener);
            
			SoraGUIResponserMap::Instance()->registerResponser(responserID, pResponser);
		}
        
        void registerExternalGUIResponser(gcn::Widget* pWidget, SoraGUIResponserExtern* pResponser, const SoraString& responserID, const SoraString& handleSrc, int iResponceType) {
            if(iResponceType & RESPONCE_ACTION) pWidget->addActionListener(pActionListener);
			if(iResponceType & RESPONCE_DEATH) pWidget->addDeathListener(pDeathListener);
			if(iResponceType & RESPONCE_KEY) pWidget->addKeyListener(pKeyListener);
			if(iResponceType & RESPONCE_MOUSE) pWidget->addMouseListener(pMouseListener);
			if(iResponceType & RESPONCE_FOCUS) pWidget->addFocusListener(pFocusListener);
            
            SoraGUIResponserMap::Instance()->registerHandleSrc((ulong32)pWidget, handleSrc);
			SoraGUIResponserMap::Instance()->registerExternalResponser(responserID, pResponser);
        }
       
    private:
        
		SoraGUIActionListener* pActionListener;
		SoraGUIKeyListener* pKeyListener;
		SoraGUIDeathListener* pDeathListener;
		SoraGUIFocusListener* pFocusListener;
		SoraGUIMouseListener* pMouseListener;
	};
} // namespace sora

#endif