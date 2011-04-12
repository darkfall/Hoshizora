//
//  jsongui.cpp
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "jsongui.h"

#include "SoraGUIResponser.h"
#include "SoraGUIResponserMap.h"
#include "guichansora.hpp"
#include "guichansetup.h"

#include "SoraCore.h"

namespace sora {
    
    JsonGui::JsonGui() {
        opened = false;
    }
    
    bool JsonGui::parse(const SoraString& filePath) {
        ulong32 size;
        void* pdata = SORA->getResourceFile(s2ws(filePath), size);
        if(pdata) {
            bool result = parse(pdata, size);
            SORA->freeResourceFile(pdata);
            return result;
        }
        return false;
    }
    
    bool JsonGui::parse(void* data, ulong32 size) {
        const char* pstrdata = (const char*)data;
        if(reader.parse(pstrdata, pstrdata+size, rootValue)) {
            parseWidget(rootValue, NULL);
            return true;
        }
        return false;
    }
    
    void JsonGui::addToParent(gcn::Widget* widget, gcn::Widget* parent) {
        gcn::Container* pcont = dynamic_cast<gcn::Container*>(parent);
        if(pcont) {
            pcont->add(widget);
        } else {
            gcn::Window* pwnd = dynamic_cast<gcn::Window*>(parent);
            if(pwnd) {
                pwnd->add(widget);
            } else {
                gcn::ScrollArea* pscroll = dynamic_cast<gcn::ScrollArea*>(parent);
                if(pscroll) {
                    pscroll->setContent(widget);
                }
            }
        }
    }
    
    gcn::Widget* JsonGui::getWidget(const SoraString& name) {
        WIDGET_MAP::iterator itWidget = widgets.find(str2id(name));
        if(itWidget != widgets.end()) {
            return itWidget->second;
        }
        return NULL;
    }
    
    void JsonGui::registerParseFunc(const SoraString& name, JsonGuiParseFunc func) {
        extensionFuncs[str2id(name)] = func;
    }
    
    void JsonGui::parseWidget(const Json::Value& val, gcn::Widget* parent) {
        VALUE_NAMES members = val.getMemberNames();
        VALUE_NAMES::iterator itMember = members.begin();
        while(itMember != members.end()) {
            const char* type = itMember->c_str();
            bool isObjectValue = val[type].isObject();
            if(isObjectValue) {
                if(strcmpnocase(type, "container") == 0) {
                    parseContainer(val[type], parent);
                } else if(strcmpnocase(type, "window") == 0) {
                    parseWindow(val[type], parent);
                } else if(strcmpnocase(type, "button") == 0) {
                    parseButton(val[type], parent);
                } else if(strcmpnocase(type, "slider") == 0) {
                    parseSlider(val[type], parent);
                } else if(strcmpnocase(type, "textbox") == 0) {
                    parseTextBox(val[type], parent);
                } else if(strcmpnocase(type, "textfield") == 0) {
                    parseTextBox(val[type], parent);
                } else if(strcmpnocase(type, "label") == 0) {
                    parseTextBox(val[type], parent);
                } else if(strcmpnocase(type, "checkbox") == 0) {
                    parseCheckbox(val[type], parent);
                } else if(strcmpnocase(type, "radiobutton") == 0) {
                    parseRadioButton(val[type], parent);
                } else if(strcmpnocase(type, "imagebutton") == 0) {
                    parseImageButton(val[type], parent);
                } else if(strcmpnocase(type, "icon") == 0) {
                    parseIcon(val[type], parent);
                }
                else {
                    EXTENSION_FUNC_MAP::iterator itFunc = extensionFuncs.find(str2id(type));
                    if(itFunc != extensionFuncs.end()) {
                        itFunc->second(val, parent, this);
                    }
                }    
            }
            ++itMember;
        }

    }
    
    void JsonGui::parseDefaults(const Json::Value& val, gcn::Widget* widget) {
        if(val.isMember("x")) {
            widget->setX(val["x"].asInt());
        }
        if(val.isMember("y")) {
            widget->setX(val["y"].asInt());
        }
        if(val.isMember("width")) {
            widget->setWidth(val["width"].asInt());
        }
        if(val.isMember("height")) {
            widget->setHeight(val["height"].asInt());
        }
        if(val.isMember("basecolor")) {
            int color;
            sscanf(val["basecolor"].asCString(), "%x", &color);
            widget->setBaseColor(gcn::Color(color));
        }
        if(val.isMember("foregroundcolor")) {
            int color;
            sscanf(val["foregroundcolor"].asCString(), "%x", &color);
            widget->setBaseColor(gcn::Color(color));
        }
        if(val.isMember("backgroundcolor")) {
            int color;
            sscanf(val["backgroundcolor"].asCString(), "%x", &color);
            widget->setBaseColor(gcn::Color(color));
        }
        
        if(val.isMember("framesize")) {
            widget->setFrameSize(val["framesize"].asInt());
        }
        
        if(val.isMember("font")) {
            int fontsize;
            if(val.isMember("fontsize")) 
                fontsize = val["fontsize"].asInt();
            else fontsize = 16;	
            gcn::SoraGUIFont* pfont = new gcn::SoraGUIFont(val["font"].asCString(), fontsize);
            if(pfont) {
                widget->setFont(pfont);
            }
        }
        
        if(val.isMember("visible"))
            widget->setVisible(val["visible"].asBool());
        
        if(val.isMember("focusable"))
            widget->setFocusable(val["focusable"].asBool());
        
        if(val.isMember("enabled"))
            widget->setEnabled(val["enabled"].asBool());
        
        if(val.isMember("tabin"))
            widget->setTabInEnabled(val["tabin"].asBool());
        
        if(val.isMember("tabout"))
            widget->setTabOutEnabled(val["tabout"].asBool());
        
        if(val.isMember("eventId"))
            widget->setActionEventId(val["eventId"].asString());
        
        if(val.isMember("id")) {
            widget->setId(val["id"].asString());
            if(val.isMember("responser")) {
                SoraString arg = val["responser"].asString();
                SoraString type = val["responsetype"].asString();
                parseResponser(widget, arg, type.size()==0?NULL:&type);
            }
        }
        
        if(val.isMember("parent")) {
            if(widget->getParent() == NULL)
                GCN_GLOBAL->addWidget(widget, val["parent"].asString());
        }
    }
    
    void JsonGui::parseContainer(const Json::Value& val, gcn::Widget* parent) {
        printf("parsing container\n");
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Container Widget must have a unique name");
        }
        
        gcn::Container *c = new gcn::Container();
        
        if(val.isMember("opaque"))
            c->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, c);
        
        //parsing child elements
        parseWidget(val, c);
        
        addToParent(c, parent);
        widgets[str2id(name)] = c;
    }
    
    void JsonGui::parseWindow(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Window Widget must have a unique name");
        }
        
        gcn::Window *window = new gcn::Window;
        if(val.isMember("caption")) {
            window->setCaption(val["caption"].asString());
        }
        if(val.isMember("tabbing")) {
            window->setPadding(val["tabbing"].asBool());
        }
        if(val.isMember("movable")) {
            window->setMovable(val["movable"].asBool());
        }
        if(val.isMember("titleBarHeight")) {
            window->setTitleBarHeight(val["titleBarHeight"].asInt());
        }
        
        if(val.isMember("opaque"))
            window->setOpaque(val["opaque"].asBool());
        
        
        parseDefaults(val, window);
        
        //parsing child elements
        parseWidget(val, window);
                
        addToParent(window, parent);
        widgets[str2id(name)] = window;
    }
    
    void JsonGui::parseButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Button Widget must have a unique name");
        }
        
        gcn::Button *button = new gcn::Button;
        if(val.isMember("caption")) {
            button->setCaption(val["caption"].asString());
        }
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmpnocase(align, "center")) {
                button->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmpnocase(align, "left")) {
                button->setAlignment(gcn::Graphics::LEFT);
            } else {
                button->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        button->adjustSize();
        parseDefaults(val, button);
        
        //	button->lostFocus(); //fix: this call must't exists
        //?????
        
        addToParent(button, parent);
        widgets[str2id(name)] = button;
    }
    
    void JsonGui::parseSlider(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::Slider *slider = new gcn::Slider;
        if(val.isMember("start")) {            
            slider->setScaleStart(val["start"].asDouble());
        }
        if(val.isMember("end")) {
            slider->setScaleEnd(val["end"].asDouble());
        }
        if(val.isMember("value")) {
            slider->setValue(val["value"].asDouble());
        }
        if(val.isMember("markerLength")) {
            slider->setMarkerLength(val["markerLength"].asInt());
        }
        if(val.isMember("stepLength")) {
            slider->setStepLength(val["stepLength"].asDouble());
        }
        if(val.isMember("orientation")) {
            if(strcmpnocase(val["orientation"].asCString(), "horizontal"))
                slider->setOrientation(gcn::Slider::HORIZONTAL);
            else slider->setOrientation(gcn::Slider::VERTICAL);
        }
        
        parseDefaults(val, slider);
        
        addToParent(slider, parent);
        widgets[str2id(name)] = slider;
    }
    
    void JsonGui::parseLabel(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::Label *label = new gcn::Label;
        if(val.isMember("caption")) {
            label->setCaption(val["caption"].asString());
        }
        
        label->adjustSize();
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmpnocase(align, "center")) {
                label->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmpnocase(align, "left")) {
                label->setAlignment(gcn::Graphics::LEFT);
            } else {
                label->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        parseDefaults(val, label);
        
        addToParent(label, parent);
        widgets[str2id(name)] = label;
    }
    
    void JsonGui::parseImageButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::ImageButton* pButton = 0;
        gcn::Image* image;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        if(image) {
            pButton = new gcn::ImageButton(image);
            parseDefaults(val, pButton);
            
            addToParent(pButton, parent);
            widgets[str2id(name)] = pButton;
        }
    }
    
    void JsonGui::parseIcon(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::Icon *icon;
        gcn::Image *image;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        
        if(image) {
            icon = new gcn::Icon(image);
            
            parseDefaults(val, icon);
            
            addToParent(icon, parent);
            widgets[str2id(name)] = icon;
        }
    }
    
    void JsonGui::parseCheckbox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::CheckBox *checkbox = new gcn::CheckBox;
        
        if(val.isMember("caption")) {
            checkbox->setCaption(val["caption"].asString());
        }
        
        checkbox->adjustSize();
    
        parseDefaults(val, checkbox);

        addToParent(checkbox, parent);
        widgets[str2id(name)] = checkbox;
    }
    
    void JsonGui::parseTextBox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::TextBox *textbox = new gcn::TextBox;
        
        if(val.isMember("editable"))
            textbox->setEditable(val["editable"].asBool());
        
        if(val.isMember("text")) {
            textbox->setText(sora::s2ws(val["text"].asString()).c_str());
        }
        
        if(val.isMember("opaque"))
            textbox->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, textbox);
        
        addToParent(textbox, parent);
        widgets[str2id(name)] = textbox;
    }
    
    void JsonGui::parseTextField(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::TextField *textField = new gcn::TextField;
    
        if(val.isMember("text")) {
            textField->setText(sora::s2ws(val["text"].asString()).c_str());
        }
    
        parseDefaults(val, textField);
        
        addToParent(textField, parent);
        widgets[str2id(name)] = textField;
    }
    
    void JsonGui::parseRadioButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::RadioButton *radio = new gcn::RadioButton;
        
        if(val.isMember("caption")) {
            radio->setCaption(val["caption"].asString());
        }
        radio->adjustSize();
        
        if(val.isMember("group"))
            radio->setGroup(val["group"].asString());		
        
        parseDefaults(val, radio);
        
        addToParent(radio, parent);
        widgets[str2id(name)] = radio;
    }
    
    
    int JsonGui::parseRespondType(const char* respondStr) {
        std::vector<std::string> rtypes;
        sora::deliStr(rtypes, respondStr, ',');
        int irtype = 0;
        for(size_t i=0; i<rtypes.size(); ++i) {
            if(rtypes[i].compare("action") == 0) irtype |= sora::RESPONCE_ACTION;
            else if(rtypes[i].compare("death") == 0) irtype |= sora::RESPONCE_DEATH;
            else if(rtypes[i].compare("focus") == 0) irtype |= sora::RESPONCE_FOCUS;
            else if(rtypes[i].compare("mouse") == 0) irtype |= sora::RESPONCE_MOUSE;
            else if(rtypes[i].compare("key") == 0) irtype |= sora::RESPONCE_KEY;
        }
        return irtype;
    }
    
    void JsonGui::parseResponser(gcn::Widget* widget, const std::string& arg, const std::string* type) {
        if(arg[0] == '@') {
            size_t ap = arg.find("=");
            if(ap != std::string::npos) {
                SoraString srcType = arg.substr(1, ap);
                
                sora::SoraGUIResponserExtern* pResponser = sora::SoraGUIResponserMap::Instance()->getExternalResponser( srcType );
                if(pResponser) {
                    const std::string* respondtypestr = type;
                    if(respondtypestr) {
                        int irtype = parseRespondType(respondtypestr->c_str());
                        sora::SoraGUI::Instance()->registerExternalGUIResponser(widget, 
                                                                                pResponser, 
                                                                                widget->getId(),
                                                                                arg.substr(ap+1, arg.size()),
                                                                                irtype);
                    } else {
                        sora::SoraGUI::Instance()->registerExternalGUIResponser(widget, 
                                                                                pResponser, 
                                                                                widget->getId(),
                                                                                arg.substr(ap+1, arg.size()),
                                                                                sora::RESPONCE_ACTION);
                    }
                }
            }
        } else {
            sora::SoraGUIResponser* pResponser = sora::SoraGUIResponserMap::Instance()->getResponser( arg );
            if(pResponser) {
                const std::string* respondtypestr = type;
                if(respondtypestr) {
                    int irtype = parseRespondType(respondtypestr->c_str());
                    sora::SoraGUI::Instance()->registerGUIResponser(widget, 
                                                                    pResponser, 
                                                                    widget->getId(),
                                                                    irtype);
                } else {
                    sora::SoraGUI::Instance()->registerGUIResponser(widget, 
                                                                    pResponser, 
                                                                    widget->getId(),
                                                                    sora::RESPONCE_ACTION);
                }
            } 
        }
    }
    
} // namespace sora