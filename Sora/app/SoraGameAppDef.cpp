//
//  SoraGameAppDef.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGameAppDef.h"
#include "SoraCore.h"
#include "config/SoraConfigParser.h"

namespace sora {
    
    SoraGameAppDef::SoraGameAppDef():
    WindowWidth(800),
    WindowHeight(600),
    WindowMode(true),
    HideMouse(false),
    Icon(std::string()),
    Cursor(std::string()),
    WindowTitle("Sora App"),
    WindowId("MainWindow"),
    Window(0) {
        
    }
    
    SoraGameAppDef::SoraGameAppDef(SoraWindowInfoBase* window):
    Window(window) {
        
    }
    
    SoraGameAppDef::SoraGameAppDef(const StringType& loadFromConfig):
    WindowWidth(800),
    WindowHeight(600),
    WindowMode(true),
    HideMouse(false),
    Icon(std::string()),
    Cursor(std::string()),
    WindowTitle("Sora App"),
    WindowId("MainWindow"),
    Window(0) {
        SoraConfigParser parser;
        if(parser.open(loadFromConfig, CONFIG_AUTO_DETECT)) {
            if(parser.toNode("/app/window")) {
                WindowWidth = parser.getInt("width", 800);
                WindowHeight = parser.getInt("height", 600);
                WindowPosX = parser.getInt("x", 0);
                WindowPosY = parser.getInt("y", 0);
                WindowMode = parser.getBool("windowed", true);
                HideMouse = parser.getBool("hidemouse", false);
                Icon = parser.getString("icon", "");
                Cursor = parser.getString("cursor", "");
                WindowTitle = parser.getString("title", "Sora App");
                WindowId = parser.getString("id", "MainWindow");
            } else {
                log_error("no /app/window node exists in window config, default used");
            }
            if(parser.toNode("/app/feature")) {
                SoraCoreFeature feature(parser.getBool("load_plugin", false),
                                        parser.getBool("fsb", false),
                                        parser.getBool("msgbox_show_error", false),
                                        parser.getBool("seperate_sound_thread", false),
                                        parser.getBool("debug_render", false));
                SoraCore::Instance()->init(feature);
            }
        }
    }
    
} // namespace sora