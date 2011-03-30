//
//  jsongui.h
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef JSON_GUI_H_
#define JSON_GUI_H_

#include "SoraPlatform.h"
#include "stringId.h"
#include "json/json.h"

#include "SoraGUI.h"

namespace sora {
    /* 
     a json gui widget reader for soraguichan
     support extensions
     */
    
    class JsonGui;
    typedef void (*JsonGuiParseFunc)(const Json::Value& val, gcn::Widget* parent, JsonGui* pCaller);
    
    class JsonGui {
    public:
        typedef Json::Value::Members VALUE_NAMES;
        JsonGui();
        
        static void parseResponser(gcn::Widget* widget, const std::string& responser, const std::string* type);
        static int parseRespondType(const char* respondStr);

        bool parse(const SoraString& filePath);
        bool parse(void* strData, ulong32 size);
        
        void parseDefaults(const Json::Value& val, gcn::Widget* widget);

        void parseWidget(const Json::Value& val, gcn::Widget* parent);
        
        inline void addToParent(gcn::Widget* widget, gcn::Widget* parent);
        
        void parseContainer(const Json::Value& val, gcn::Widget* parent);
        void parseWindow(const Json::Value& val, gcn::Widget* parent);
        void parseButton(const Json::Value& val, gcn::Widget* parent);
        void parseSlider(const Json::Value& val, gcn::Widget* parent);
        void parseLabel(const Json::Value& val, gcn::Widget* parent);
        void parseIcon(const Json::Value& val, gcn::Widget* parent);
        void parseCheckbox(const Json::Value& val, gcn::Widget* parent);
        void parseTextBox(const Json::Value& val, gcn::Widget* parent);
        void parseTextField(const Json::Value& val, gcn::Widget* parent);
        void parseRadioButton(const Json::Value& val, gcn::Widget* parent);
        void parseImageButton(const Json::Value& val, gcn::Widget* parent);
        
        void registerParseFunc(const SoraString& name, JsonGuiParseFunc func);
        
        gcn::Widget* getWidget(const SoraString& name);
        
    private:
        Json::Reader reader;
        Json::Value rootValue;
        
        bool opened;
        
        typedef std::map<stringId, JsonGuiParseFunc> EXTENSION_FUNC_MAP;
        EXTENSION_FUNC_MAP extensionFuncs;
        
        typedef std::map<stringId, gcn::Widget*> WIDGET_MAP;
        WIDGET_MAP widgets;
    };
    
} // namespace sora


#endif