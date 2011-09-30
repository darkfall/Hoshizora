//
//  ConfigController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "ConfigController.h"

#include "config/SoraConfigParser.h"

namespace flower {
    
    float ConfigController::SEVolume = 1.f;
    float ConfigController::BGMVolume = 1.f;
    float ConfigController::MaxSpeed = 100.f;
    float ConfigController::AccelerometerSensetivity = 0.3f;
    ControlMode ConfigController::CurrentControlMode = CM_ACCELEROMETER;
    
    bool ConfigController::LoadConfig(const std::wstring& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            if(parser.toNode("/config"));
            
            SEVolume = parser.getFloat("SEVolume", 1.f);
            BGMVolume = parser.getFloat("BGMVolume", 1.f);
            MaxSpeed = parser.getFloat("MaxSpeed", 100.f);
            
            AccelerometerSensetivity = parser.getFloat("AccelerometerSensetivity", 0.3);
            
            std::string cm = parser.getString("ControlMode");
            if(cm == "Accelerometer")
                CurrentControlMode = CM_ACCELEROMETER;
            else
                CurrentControlMode = CM_TOUCH;
            
            return true;
        }
        return false;
    }
    
    bool ConfigController::WriteConfig(const std::wstring& path) {
        sora::SoraConfigParser parser;
        
        if(parser.open(path)) {
            if(parser.toNode("/config")) {
            }
        }
        return false;
    }
     
} // namespace flower