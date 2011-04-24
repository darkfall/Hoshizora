/*
 *  guichansetup.h
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_GUICHAN_SETUP_H_
#define SORA_GUICHAN_SETUP_H_

#include "SoraGUIChan/guichansora.hpp"

#include "SoraGUIChan/guichan.hpp"

#include "SoraCore.h"

namespace sora {
	
	using namespace gcn;
	
	class gcnInitializer: public sora::SoraSingleton<gcnInitializer> {
		friend class sora::SoraSingleton<gcnInitializer>;
		
	public:
        class gcnListener: public SoraFrameListener {
        public:
            gcnListener(gcnInitializer* initializer): pinitializer(initializer) {}
            
            void onFrameStart()  {}
            void onFrameEnd() {
                pinitializer->gcnLogic();
            }
        private:
            gcnInitializer* pinitializer;
        };
        
		gcn::Gui* pGUIChan;
        gcnInitializer(): pGUIChan(0) {}
		
		bool initGUIChan(const SoraWString& font, int size) {
            if(pGUIChan) return false;
            
			pGUIChan = new gcn::Gui;
		
			pGUIChan->setInput(new gcn::SoraGUIInput);
			gcn::Image::setImageLoader(new gcn::SoraGUIImageLoader);
			pGUIChan->setGraphics(new gcn::SoraGUIGraphics);
			gcn::SoraGUIFont* pfont = new gcn::SoraGUIFont(font, size);
			if(pfont)
				gcn::Widget::setGlobalFont(pfont);
			else
				return false;
            
            gcnListener* plistener = new gcnListener(this);
            SORA->addFrameListener(plistener);
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
                if(sid.compare("top") == 0)
                    return pTop;
                
                gcn::Widget* pWidget = 0;
                try {
                    pWidget = pTop->findWidgetById(sid.c_str());
                } catch(gcn::Exception& exp) {
                    SORA->log("guilib exception: "+exp.getMessage());
                }
                return pWidget;
            }
            return NULL;
        }
        
        void addWidget(gcn::Widget* widget, const SoraString& parentId) {
            gcn::Widget* pParent = findWidget(parentId);
            
            try {
                gcn::Container* cont = dynamic_cast<gcn::Container*>(pParent);
                if(cont != NULL) {
                    cont->add(widget);
                    return;
                }
                gcn::Window* wind = dynamic_cast<gcn::Window*>(pParent);
                if(wind != NULL) {
                    wind->add(widget);
                    return;
                }
                gcn::ScrollArea* scroll = dynamic_cast<gcn::ScrollArea*>(pParent);
                if(scroll) {
                    scroll->setContent(widget);
                    return;
                }
            } catch(gcn::Exception& exp) {
                SORA->log("guilib exception: "+exp.getMessage());
            }
        }
        
        void removeWidget(gcn::Widget* widget) {
            gcn::Widget* pParent = widget->getParent();            
            try {
                gcn::Container* cont = dynamic_cast<gcn::Container*>(pParent);
                if(cont != NULL) {
                    cont->remove(widget);
                    return;
                }
                gcn::Window* wind = dynamic_cast<gcn::Window*>(pParent);
                if(wind != NULL) {
                    wind->remove(widget);
                    return;
                }
                gcn::ScrollArea* scroll = dynamic_cast<gcn::ScrollArea*>(pParent);
                if(scroll) {
                    scroll->setContent(NULL);
                    return;
                }
            } catch(gcn::Exception& exp) {
                SORA->log("guilib exception: "+exp.getMessage());
            }
        }
		
		gcn::Widget* getWidgetAt(int32 x, int32 y, const char* wid=NULL) {
			if(wid == NULL) {
				gcn::Container* pTop = getTop();
				if(pTop) {
					return pTop->getWidgetAt(x, y);
				}
			} else {
				gcn::Widget* pWidget = findWidget(wid);
				if(pWidget)
					return pWidget->getWidgetAt(x, y);
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