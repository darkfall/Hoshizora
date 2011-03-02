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

namespace srgc {
	
	using namespace gcn;
	
	class gcnInitializer: public sora::SoraSingleton<gcnInitializer> {
		friend class sora::SoraSingleton<gcnInitializer>;
		
	public:
		gcn::Gui* pGUIChan;
		
		void initGUIChan() {
			pGUIChan = new gcn::Gui;
		
			pGUIChan->setInput(new gcn::SoraGUIInput);
			gcn::Image::setImageLoader(new gcn::SoraGUIImageLoader);
			pGUIChan->setGraphics(new gcn::SoraGUIGraphics);
			gcn::Widget::setGlobalFont(new gcn::SoraGUIFont("ARIALN.TTF", 36));
		}
		
		void createTop() {
			gcn::Container* contTop = new gcn::Container;
			contTop->setDimension(gcn::Rectangle(0, 0, 
												 sora::SoraCore::Instance()->getScreenWidth(), 
												 sora::SoraCore::Instance()->getScreenHeight()));
			pGUIChan->setTop(contTop);
			contTop->setOpaque(false);
		}
	
		gcn::Container* getTop() {
			return (gcn::Container*)pGUIChan->getTop();
		}
	
		void gcnLogic() {
			pGUIChan->logic();
		}
	
		void gcnDraw() {
			pGUIChan->draw();
		}
	};
	
#define GCN_GLOBAL gcnInitializer::Instance()
} // namespace srgc

#endif