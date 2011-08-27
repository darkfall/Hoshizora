//
//  SoraLuaExporter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraLuaExporter.h"

namespace sora {
    
    SoraLuaExporter::ExporterSignal SoraLuaExporter::mExporter;
    
    void SoraLuaExporter::OnExport(LuaPlus::LuaState* state) {
        mExporter.sig(state);
    }
    
} // namespace sora