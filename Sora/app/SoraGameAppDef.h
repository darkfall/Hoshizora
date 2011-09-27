//
//  SoraGameAppDef.h
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGameAppDef_h
#define Sora_SoraGameAppDef_h

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
    
    // a simple container class that defines the main window of the game
    
    class SoraWindowInfoBase;
    
    struct SORA_API SoraGameAppDef {
        SoraGameAppDef();
        SoraGameAppDef(SoraWindowInfoBase* window);
        /**
         * Config must contain the following node
         *  /App/Window
         * With following attributes, some are optimal
         *  Width
         *  Height
         *  X
         *  Y
         *  Windowed
         *  HideMouse
         *  Icon
         *  Cursor
         *  Title
         *  Id
         **/
        explicit SoraGameAppDef(const StringType& loadFromConfig /* json or xml or ini */);
        
        // window size
        int32 WindowWidth, WindowHeight;
        // window pos
        int32 WindowPosX, WindowPosY;
        
        // features
        // default: YES
        bool WindowMode;
        // default: NO
        bool HideMouse;
        // default: NONE
        std::string Icon, Cursor;
        
        // default: "Sora App"
        std::string WindowTitle;
        // default: "MainWindow"
        std::string WindowId;
        
        // external window
        SoraWindowInfoBase* Window;
    };
    
} // namespace sora

#endif
