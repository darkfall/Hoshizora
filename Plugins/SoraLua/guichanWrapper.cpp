//
//  guichanWrapper.cpp
//  Sora
//
//  Created by Griffin Bu on 3/23/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//


#include "guichanWrapper.h"

#include "SoraGUIChan/guichansetup.h"
#include "SoraGUIChan/xmlgui.h"
#include "SoraGUIChan/jsongui.h"
#include "LuaPlusHelper.h"

namespace sora {
    
    gcn::Widget* SoraGuiCreator::createGuiWidget(const SoraString &ident, const SoraString& id) {
        stringId sid = str2id(ident);
        if(sid == SID_BUTTON) {
            gcn::Button* pbutton = new gcn::Button;
            pbutton->setId(id);
            return pbutton;
        }
        if(sid == SID_SLIDER) {
            gcn::Slider* pslider = new gcn::Slider;
            pslider->setId(id);
            return pslider;
        }
        if(sid == SID_CONTAINER) {
            gcn::Container* pcont = new gcn::Container;
            pcont->setId(id);
            return pcont;
        }
        if(sid == SID_WINDOW) {
            gcn::Window* pwind = new gcn::Window;
            pwind->setId(id);
            return pwind;
        }
        if(sid == SID_LABEL) {
            gcn::Label* plabel = new gcn::Label;
            plabel->setId(id);
            return plabel;
        }
        if(sid == SID_CHECKBOX) {
            gcn::CheckBox* pcheckbox = new gcn::CheckBox;
            pcheckbox->setId(id);
            return pcheckbox;
        }
        if(sid == SID_TEXTBOX) {
            gcn::TextBox* ptextbox = new gcn::TextBox;
            ptextbox->setId(id);
            return ptextbox;
        }
        if(sid == SID_TEXTFIELD) {
            gcn::TextField* pfield = new gcn::TextField;
            pfield->setId(id);
            return pfield;
        }
        if(sid == SID_ICON) {
            gcn::Icon* picon = new gcn::Icon;
            picon->setId(id);
            return picon;
        }
        if(sid == SID_IMAGEBUTTON) {
            gcn::ImageButton* pbutton = new gcn::ImageButton;
            pbutton->setId(id);
            return pbutton;
        }
        
        return NULL;
    }
    
} // namespace sora

namespace guiwrapper {
    
    using namespace sora;
    
    void export_guilibobj(SoraLuaObject* obj) {
        export_guilib(obj->getState());
    }
    
    void export_guilib(LuaPlus::LuaState* state) {
        assert(state != NULL);
        
        LuaModule(state, "guilib")
        .def("loadXmlGui", &guiwrapper::loadXmlGui)
        .def("loadJsonGui", &guiwrapper::loadJsonGui)
        .def("createWidget", &guiwrapper::createWidget)
        .def("getWidgetById", &guiwrapper::getWidgetById)
        .def("addWidget", &guiwrapper::addWidgetToParent)
        .def("removeWidget", &guiwrapper::removeWidget)
        .def("setWidgetResponser", &guiwrapper::setWidgetResponser)
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
        .def("isWigetOpaque", &guiwrapper::isWidgetOpaque)
        .def("setWidgetCaption", &guiwrapper::setWidgetCaption)
        .def("getWidgetCaption", &guiwrapper::getWidgetCaption)
        .def("setWidgetValue", &guiwrapper::setWidgetValue)
        .def("setWidgetEnabled", &guiwrapper::setWidgetEnabled)
        .def("isWidgetEnabled", &guiwrapper::isWidgetEnabled);
    }
    
    inline gcn::Widget* getWidgetByHandle(ulong32 h) {
        return (gcn::Widget*)h;
    }
    inline ulong32 getWidgetHandleByWidget(gcn::Widget* w) {
        return (ulong32)w;
    }
   
    static SoraGuiCreator* pCreator = new SoraGuiCreator;
    void setGuiCreator(SoraGuiCreator* creator) {
        if(pCreator) {
            delete pCreator;
        }
        pCreator = creator;
    }
    
    ulong32 createWidget(const SoraString& ident, const SoraString& id) {
        if(pCreator) {
            return (ulong32)pCreator->createGuiWidget(ident, id);
        }
        return 0;
    }
    
    ulong32 getWidgetById(const SoraString& id) {
        return (ulong32)GCN_GLOBAL->findWidget(id);
    }
    
    bool loadXmlGui(const SoraString& xmlPath) {
        XmlGui* pXmlGui = new XmlGui;
        bool result = pXmlGui->parse(xmlPath);
        delete pXmlGui;
        return result;
    }
    
    bool loadJsonGui(const SoraString& jsonPath) {
        JsonGui* pJsonGui = new JsonGui;
        bool result = pJsonGui->parse(jsonPath);
        delete pJsonGui;
        return result;
    }
    
    void setWidgetResponser(ulong32 h, const SoraString& responser, const SoraString& responseType) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            std::string type = responseType;
            XmlGui::parseResponser(w, responser, &type);
        }
    }
    
    void addWidgetToParent(ulong32 h, const SoraString& parent) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            GCN_GLOBAL->addWidget(w, parent);
        }
    }
    
    void removeWidget(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            GCN_GLOBAL->removeWidget(w);
        }
    }
    
    void setWidgetFrameSize(ulong32 h, uint32 framesize) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setFrameSize(framesize);
    }
    
    uint32 getWidgetFrameSize(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getFrameSize();
        return 0;
    }
    
    ulong32 getWidgetParent(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return getWidgetHandleByWidget(w->getParent());
        return 0;
    }
    
    void setWidgetWidth(ulong32 h, int32 wd) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setWidth(wd);
    }
    
    void setWidgetHeight(ulong32 h, int32 ht) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setHeight(ht);
    }
    
    void setWidgetSize(ulong32 h, int32 wd, int32 ht) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setSize(wd, ht);
    }
    
    int32 getWidgetWidth(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getWidth();
        return 0;
    }
    
    int32 getWidgetHeight(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getHeight();
        return 0;
    }
    
    void setWidgetX(ulong32 h, int32 x) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setX(x);
    }
    
    void setWidgetY(ulong32 h, int32 y) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setY(y);
    }
    
    void setWidgetPos(ulong32 h, int32 x, int32 y) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setPosition(x, y);
    }
    
    int32 getWidgetX(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getX();
        return 0;
    }
    
    int32 getWidgetY(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getY();
        return 0;
    }
    
    void setWidgetFocusable(ulong32 h, bool focusable) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setFocusable(focusable);
    }
    
    bool isWidgetFocusable(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isFocusable();
        return false;
    }
    
    bool isWidgetFocused(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isFocused();
        return false;
    }
    
    void setWidgetVisible(ulong32 h, bool visible) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setVisible(visible);
    }
    
    bool isWidgetVisible(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isVisible();
        return false;
    }
    
    void setWidgetBaseColor(ulong32 h, int32 c) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setBaseColor(gcn::Color(c));
    }
    
    void setWidgetForegroundColor(ulong32 h, int32 c) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setForegroundColor(gcn::Color(c));
    }
    
    void setWidgetBackgroundColor(ulong32 h, int32 c) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setBackgroundColor(gcn::Color(c));
    }
    
    void setWidgetSelectionColor(ulong32 h, int32 c) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setSelectionColor(gcn::Color(c));
    }
    
    void setWidgetFont(ulong32 h, const SoraString& fontName, int32 fontSize) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            SoraGUIFont* font = new SoraGUIFont(fontName.c_str(), fontSize);
            w->setFont(font);
        }
    }
    
    void setWidgetEnabled(ulong32 h, bool enabled) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            w->setEnabled(enabled);
        }
    }
    
    bool isWidgetEnabled(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) {
            w->isEnabled();
        }
    }
    
    void requestWidgetModalFocus(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->requestModalFocus();
    }
    
    void requestWidgetModalMouseInputFocus(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->requestModalMouseInputFocus();
    }
    
    void requestWidgetFocus(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->requestFocus();
    }
    
    void releaseWidgetModalFocus(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->releaseModalFocus();
    }
    
    void releaseWidgetModalMouseInputFocus(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->releaseModalMouseInputFocus();
    }
    
    bool isWidgetModalFocused(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isModalFocused();
        return false;
    }
    
    bool isWidgetModalMouseInputFocused(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isModalMouseInputFocused();
        return false;
    }
    
    ulong32 getWidgetAt(ulong32 h, int32 x, int32 y) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return getWidgetHandleByWidget(w->getWidgetAt(x, y));
        return 0;
    }
    
    void setWidgetTabInEnabled(ulong32 h, bool flag) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setTabInEnabled(flag);
    }
    
    void setWidgetTabOutEnabled(ulong32 h, bool flag) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setTabOutEnabled(flag);
    }
    
    bool isWidgetTabInEnabled(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isTabInEnabled();
        return false;
    }
    
    bool isWidgetTabOutEnabled(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->isTabOutEnabled();
        return false;
    }
    
    void setWidgetId(ulong32 h, const SoraString& str) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->setId(str);
    }
    
    SoraString getWidgetId(ulong32 h) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) return w->getId();
        return "\0";
    }
    
    void showWidgetPart(ulong32 h, int32 x, int32 y, int32 wd, int32 ht) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) w->showPart(gcn::Rectangle(x, y, wd, ht));
    }
    
    bool canWidgetSetCaption(gcn::Widget* widget, const SoraString& caption) {
        gcn::Button* b = dynamic_cast<gcn::Button*>(widget);
        if(b) {
            b->setCaption(caption); b->adjustSize();
            return true;
        }
        
        gcn::Window* w = dynamic_cast<gcn::Window*>(widget);
        if(w) {
            w->setCaption(caption);
            return true;
        }
        
        gcn::CheckBox* cb = dynamic_cast<gcn::CheckBox*>(widget);
        if(cb) {
            cb->setCaption(caption); cb->adjustSize();
            return true;
        }
        
        gcn::Label* l = dynamic_cast<gcn::Label*>(widget);
        if(l) {
            l->setCaption(caption); l->adjustSize();
            return true;
        }
        
        return false;
    }
    
    bool canWidgetSetValue(gcn::Widget* widget, float32 val) {
        gcn::Slider* s = dynamic_cast<gcn::Slider*>(widget);
        if(s) {
            s->setValue(val);
            return true;
        }
        
        return false;
    }
    
    bool canWidgetSetOpaque(gcn::Widget* widget, bool flag) {
        gcn::Container* c = dynamic_cast<gcn::Container*>(widget);
        if(c) {
            c->setOpaque(flag);
            return true;
        }
        
        gcn::Window* w = dynamic_cast<gcn::Window*>(widget);
        if(w) {
            w->setOpaque(flag);
            return true;
        }
        
        gcn::TextBox* t = dynamic_cast<gcn::TextBox*>(widget);
        if(t) {
            t->setOpaque(flag);
            return true;
        }
        
        return false;
    }
    
    void setWidgetOpaque(ulong32 h, bool flag) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) canWidgetSetOpaque(w, flag);
    }
    
    bool isWidgetOpaque(ulong32 h) {
        return false;
    }
    
    void setWidgetCaption(ulong32 h, const SoraString& caption) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) canWidgetSetCaption(w, caption);
    }
    
    SoraString getWidgetCaption(ulong32 h) {
        return "\0";
    }
    
    void setWidgetValue(ulong32 h, float32 val) {
        gcn::Widget* w = getWidgetByHandle(h);
        if(w) canWidgetSetValue(w, val);
    }
} // namespace guiwrapper