//
//  ConfigController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_ConfigController_h
#define Sora_iPhone_ConfigController_h

#include "SoraPlatform.h"
#include "defines.h"

namespace flower {
    
    class ConfigController {
    public:
        static bool LoadConfig(const std::wstring& path);
        static bool WriteConfig(const std::wstring& path);
        
        static float SEVolume;
        static float BGMVolume;
        static float MaxSpeed;
        static float AccelerometerSensetivity;
        
        static ControlMode CurrentControlMode;
    };
    
    
} // namespace flower


#endif
