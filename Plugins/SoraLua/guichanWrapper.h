//
//  guichanWrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/23/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//
#ifndef SORA_GUICHAN_WRAPPER_H_
#define SORA_GUICHAN_WRAPPER_H_

#include "SoraPlatform.h"
#include "SoraGUIChan/SoraGUI.h"
#include "SoraGUIChan/guichansetup.h"

#include "SoraLuaStateManager.h"
#include "SoraLuaObject.h"
#include "stringId.h"

namespace sora {
	/*
	 gui creator delegate
	 inheritate from this to create your own widgets
	 @ident, widget ident from script, "button" etc
	 @id, the id of the widget
	 */
    class SoraGuiCreator {
    public:
        virtual gcn::Widget* createGuiWidget(const SoraString& ident, const SoraString& id);
    };
    
    const stringId SID_BUTTON       = str2id("button");
    const stringId SID_SLIDER       = str2id("slider");
    const stringId SID_CONTAINER    = str2id("container");
    const stringId SID_WINDOW       = str2id("window");
    const stringId SID_LABEL        = str2id("label");
    const stringId SID_ICON         = str2id("icon");
    const stringId SID_CHECKBOX     = str2id("checkbox");
    const stringId SID_TEXTBOX      = str2id("textbox");
    const stringId SID_TEXTFIELD    = str2id("textfield");
    const stringId SID_IMAGEBUTTON  = str2id("imagebutton");
    
    
} // namespace sora

namespace guiwrapper {
    
    void export_guilibobj(sora::SoraLuaObject* obj);
    void export_guilib(LuaPlus::LuaState* state);
    
    bool loadXmlGui(const SoraWString& xmlPath);
    bool loadJsonGui(const SoraWString& jsonPath);
    
    void setGuiCreator(sora::SoraGuiCreator* pCreator);
    ulong32 createWidget(const SoraString& ident, const SoraString& id);
    
    ulong32 getWidgetById(const SoraString& id);
    void addWidgetToParent(ulong32 h, const SoraString& parent);
    void removeWidget(ulong32 h);
    
    void setWidgetResponser(ulong32 h, const SoraString& responser, const SoraString& responsetype);
    
    void setWidgetFrameSize(ulong32 h, uint32 framesize);
    uint32 getWidgetFrameSize(ulong32 h);
    
    ulong32 getWidgetParent(ulong32 h);
    
    void setWidgetWidth(ulong32 h, int32 w);
    void setWidgetHeight(ulong32 h, int32 ht);
    void setWidgetSize(ulong32 h, int32 w, int32 ht);
    int32 getWidgetWidth(ulong32 h);
    int32 getWidgetHeight(ulong32 h);
    void adjustWidgetSize(ulong32 h);
    
    void setWidgetX(ulong32 h, int32 x);
    void setWidgetY(ulong32 h, int32 y);
    void setWidgetPos(ulong32 h, int32 x, int32 y);
    int32 getWidgetX(ulong32 h);
    int32 getWidgetY(ulong32 h);
    
    void setWidgetFocusable(ulong32 h, bool focusable);
    bool isWidgetFocusable(ulong32 h);
    bool isWidgetFocused(ulong32 h);
    
    void setWidgetVisible(ulong32 h, bool visible);
    bool isWidgetVisible(ulong32 h);
    
    void setWidgetEnabled(ulong32 h, bool enabled);
    bool isWidgetEnabled(ulong32 h);
    
    void setWidgetBaseColor(ulong32 h, int32 c);
    void setWidgetForegroundColor(ulong32 h, int32 c);
    void setWidgetBackgroundColor(ulong32 h, int32 c);
    void setWidgetSelectionColor(ulong32 h, int32 c);
    
    void setWidgetFont(ulong32 h, const SoraString& fontName, int32 fontSize);
    
    void requestWidgetFocus(ulong32 h);
    void requestWidgetModalFocus(ulong32 h);
    void requestWidgetModalMouseInputFocus(ulong32 h);
    void releaseWidgetModalFocus(ulong32 h);
    void releaseWidgetModalMouseInputFocus(ulong32 h);
    bool isWidgetModalFocused(ulong32 h);
    bool isWidgetModalMouseInputFocused(ulong32 h);
    
    ulong32 getWidgetAt(ulong32 h, int32 x, int32 y);
    
    void setWidgetTabInEnabled(ulong32 h, bool flag);
    void setWidgetTabOutEnabled(ulong32 h, bool flag);
    bool isWidgetTabInEnabled(ulong32 h);
    bool isWidgetTabOutEnabled(ulong32 h);
    
    void setWidgetId(ulong32 h, const SoraString& str);
    SoraString getWidgetId(ulong32 h);
    void showWidgetPart(ulong32 h, int32 x, int32 y, int32 wd, int32 ht);
    
    bool canWidgetSetCaption(gcn::Widget* Widget, const SoraString& caption);
    bool canWidgetSetValue(gcn::Widget* Widget, float32 val);
    bool canWidgetSetOpaque(gcn::Widget* widget, bool flag);
    
    void setWidgetOpaque(ulong32 h, bool flag);
    bool isWidgetOpaque(ulong32 h);
    
    void setWidgetCaption(ulong32 h, const SoraString& caption);
    SoraString getWidgetCaption(ulong32 h);
    
    void setWidgetValue(ulong32 h, float32 val);
} // namespace guiwrapper

#endif