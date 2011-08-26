//
//  SoraLuaExporter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/25/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraLuaExporter.h"

namespace sora {
    
    SoraLuaExporter::ExporterSignal SoraLuaExporter::mExporter;
    
    void SoraLuaExporter::OnExport(LuaPlus::LuaState* state) {
        mExporter.sig(state);
    }
    
} // namespace sora