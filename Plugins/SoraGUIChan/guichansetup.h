/*
 *  guichansetup.h
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef GUICHAN_SETUP_H_
#define GUICHAN_SETUP_H_

#include "SoraGUIChan/guichansora.hpp"

#include "SoraGUIChan/guichan.hpp"

#include "SoraCore.h"

namespace sora {
	
	using namespace gcn;
	
	class gcnInitializer: public sora::SoraSingleton<gcnInitializer> {
		friend class sora::SoraSingleton<gcnInitializer>;
		
	public:
		gcn::Gui* pGUIChan;
        gcnInitializer(): pGUIChan(0) {}
		
		bool initGUIChan(const SoraWString& font, int size) {
            if(pGUIChan) return false;
            
			pGUIChan = new gcn::Gui;
		
			pGUIChan->setInput(new gcn::SoraGUIInput);
			gcn::Image::setImageLoader(new gcn::SoraGUIImageLoader);
			pGUIChan->setGraphics(new gcn::SoraGUIGraphics);
			gcn::SoraGUIFont* pfont = new gcn::SoraGUIFont(ws2s(font).c_str(), size);
			if(pfont)
				gcn::Widget::setGlobalFont(pfont);
			else
				return false;
			return true;
		}
		
		void createTop() {
            if(!pGUIChan) return;
            
			gcn::Container* contTop = new gcn::Container;
			contTop->setDimension(gcn::Rectangle(0, 0, 
												 sora::SORA->getScreenWidth(), 
												 sora::SORA->getScreenHeight()));
			pGUIChan->setTop(contTop);
			contTop->setOpaque(false);
		}
	
		gcn::Container* getTop() {
			return (gcn::Container*)pGUIChan->getTop();
		}
        
        gcn::Widget* findWidget(const SoraString& sid) {
            gcn::Container* pTop = getTop();
            if(pTop) {
                return pTop->findWidgetById(sid.c_str());
            }
            return NULL;
        }
	
		void gcnLogic() {
            if(pGUIChan)
                pGUIChan->logic();
		}
	
		void gcnDraw() {
			if(pGUIChan)
                pGUIChan->draw();
		}
	};
	
	#define GCN_GLOBAL gcnInitializer::Instance()
} // namespace srgc

#endif