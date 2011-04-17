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

        bool parse(const SoraWString& filePath);
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
        void parseListBox(const Json::Value& val, gcn::Widget* parent);
		void parseDropDown(const Json::Value& val, gcn::Widget* parent);
		
        void registerParseFunc(const SoraString& name, JsonGuiParseFunc func);
        
        gcn::Widget* getWidget(const SoraString& name);
        
    private:
		inline void _parseWidget(const char* name, const Json::Value& val, gcn::Widget* parent);
		
        Json::Reader reader;
        Json::Value rootValue;
        
        bool opened;
        
        typedef std::map<stringId, JsonGuiParseFunc> EXTENSION_FUNC_MAP;
        EXTENSION_FUNC_MAP extensionFuncs;
        
        typedef std::map<stringId, gcn::Widget*> WIDGET_MAP;
        WIDGET_MAP widgets;
    };
	
	class JsonListModel : public gcn::ListModel {
	public:
		virtual int getNumberOfElements() { return items.size(); }
		virtual std::string getElementAt(int i) { 
			if(i < items.size())
				return items[i];
			return "null";
		}
		virtual void pushElement(const std::string& str) { items.push_back(str); }
		
		std::vector<std::string> items;
	};
    
} // namespace sora


#endif