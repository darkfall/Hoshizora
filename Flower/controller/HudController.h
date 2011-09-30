//
//  HudController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_HudController_h
#define Sora_iPhone_HudController_h

#include "SoraPlatform.h"
#include "SoraFont.h"
#include "SoraSingleton.h"

namespace flower {
    
    class HudController: public sora::SoraSingleton<HudController> {
    public:
        friend class sora::SoraSingleton<HudController>;
        
        HudController();
        ~HudController();
        
        void setFont(const std::wstring& fontName, int32 fontSize);
        sora::SoraFont* getFont() const;
        
        void render();
        void update(float dt);
        
    private:
        sora::SoraFont* mFont;
    };
    
} // namespace flower




#endif
