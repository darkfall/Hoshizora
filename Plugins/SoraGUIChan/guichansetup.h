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
		
	class gcnInitializer: public SoraFrameListener {
	public:        
        gcnInitializer(): 
        mGUIChan(NULL),
        mGlobalFont(NULL),
        mInput(NULL),
        mImageLoader(NULL),
        mGraphics(NULL),
        mSoundLoader(NULL) {
		}
		
		~gcnInitializer() {
			if(mGlobalFont)
				delete mGlobalFont;
			if(mImageLoader)
				delete mImageLoader;
			if(mInput)
				delete mInput;
			if(mGraphics)
				delete mGraphics;
			if(mSoundLoader)
				delete mSoundLoader;
		}
        
        void onFrameStart()  {}
        void onFrameEnd() {
            gcnLogic();
        }
        
        static gcnInitializer* Instance() {
            static gcnInitializer instance;
            return &instance;
        }
        
		gcn::Gui* getGui() const { return mGUIChan; }
		
		bool initGUIChan(const wchar_t* font, int size) {
            if(mGUIChan) return false;
            
			mGUIChan = new gcn::Gui;
		
			mInput = new gcn::SoraGUIInput;
			mGUIChan->setInput(mInput);
            
			mImageLoader = new gcn::SoraGUIImageLoader;
			gcn::Image::setImageLoader(mImageLoader);
			
            mGraphics = new gcn::SoraGUIGraphics;
			mGUIChan->setGraphics(mGraphics);
			
            mSoundLoader = new gcn::SoraGUISoundLoader;
			mGUIChan->setSoundLoader(mSoundLoader);
			
			if(font) { 
				mGlobalFont = new gcn::SoraGUIFont(font, size);
				if(mGlobalFont)
					gcn::Widget::setGlobalFont(mGlobalFont);
				else
					return false;
			}
			
            SoraCore::Instance()->addFrameListener(this);
			
			createTop();
			return true;
		}
		
		void createTop() {
            if(!mGUIChan || mGUIChan->getTop() != NULL) return;
            
			gcn::Container* contTop = new gcn::Container;
			contTop->setDimension(gcn::Rectangle(0, 0, 
												 sora::SORA->getScreenWidth(), 
												 sora::SORA->getScreenHeight()));
			mGUIChan->setTop(contTop);
			contTop->setOpaque(false);
		}
	
		gcn::Container* getTop() {
			return (gcn::Container*)mGUIChan->getTop();
		}
		
		void setTop(gcn::Container* top) {
			mGUIChan->setTop(top);
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
                    log_error("guilib exception: "+exp.getMessage());
                    return NULL;
                }
                return pWidget;
            }
            return NULL;
        }
        
        void addWidget(gcn::Widget* widget, const char* parentId) {
			if(!parentId) {
				getTop()->add(widget);
				return;
			}
			
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
                log_error("guilib exception: "+exp.getMessage());
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
                log_error("guilib exception: "+exp.getMessage());
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
            if(mGUIChan)
                mGUIChan->logic();
		}
	
		void gcnDraw() {
			if(mGUIChan)
                mGUIChan->draw();
		}
		
		
		void setGlobalBackgroundColor(ulong32 col) {
			gcn::Style::setGlobalBackgroundColor(gcn::Color(CGETR(col), CGETG(col), CGETB(col), CGETA(col)));
		}
		
		void setGlobalForegroundColor(ulong32 col) {
			gcn::Style::setGlobalForegroundColor(gcn::Color(CGETR(col), CGETG(col), CGETB(col), CGETA(col)));
		}
		
		void setGlobalBaseColor(ulong32 col) {
			gcn::Style::setGlobalBaseColor(gcn::Color(CGETR(col), CGETG(col), CGETB(col), CGETA(col)));
		}
		
		void setGlobalSelectionColor(ulong32 col) {
			gcn::Style::setGlobalSelectionColor(gcn::Color(CGETR(col), CGETG(col), CGETB(col), CGETA(col)));
		}
		
	private:
		gcn::SoraGUISoundLoader* mSoundLoader;
		gcn::SoraGUIGraphics* mGraphics;
		gcn::SoraGUIImageLoader* mImageLoader;
		gcn::SoraGUIInput* mInput;
		gcn::SoraGUIFont* mGlobalFont;
		
		gcn::Gui* mGUIChan;

	};
	
	#define GCN_GLOBAL gcnInitializer::Instance()
} // namespace srgc

#endif