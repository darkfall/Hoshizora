//
//  SoraInputListeners.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraInputListeners_h
#define Sora_SoraInputListeners_h

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

namespace sora {
    
    class SORA_API SoraMouseListener {
    public:
        virtual ~SoraMouseListener() {}
        
        virtual void mouseMoved(SoraMouseEvent& event) { }
        virtual void mouseDragged(SoraMouseEvent& event) { }
        virtual void mouseWheelUp(SoraMouseEvent& event) { }
        virtual void mouseWheelDown(SoraMouseEvent& event) { }
        virtual void mouseClicked(SoraMouseEvent& event) { }
        virtual void mouseReleased(SoraMouseEvent& event) { }
    };
    
    class SORA_API SoraKeyListener {
    public:
        virtual ~SoraKeyListener() {}
        
        virtual void keyPressed(SoraKeyEvent& event) { }
        virtual void keyReleased(SoraKeyEvent& event) { }
    };
    
    class SORA_API SoraJoystickListener {
    public:
        virtual ~SoraJoystickListener() {}
        
        virtual void joystickPressed(SoraJoystickEvent& event) { }
        virtual void joystickRelesed(SoraJoystickEvent& event) { }
    };
    
} // namespace sora

#endif
