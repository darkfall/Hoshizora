//
//  SoraScene.h
//  Sora
//
//  Created by Ruiwei Bu on 7/17/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraScene_h
#define Sora_SoraScene_h

/**
 *  A simple scene implemention
 *  All objects in the scene would be updated and rendered when the scene
 *  is updated or rendered
 *  All subclasses of SoraObject in the scene would calculate their position depend on the position
 *  of the scene
 **/

#include "SoraPlatform.h"
#include "SoraObject.h"

namespace sora {
    
    class SoraScene: public SoraObject {
    public:
        
        /**
         *  If enabled
         *  Then the scene would try to render itself to a RenderTarget using SoraBaseCanvas
         *  Howerver, when enabled, you should not put the render task between beginScene and endScene
         *  Otherwise would cause the whole render stuff mess up
         **/
        void enableRenderAsCanvas(bool flag);
        
        void render();
        uint32 update(float32 dt);
        
    private:
        bool mRenderAsCanvas;
    };
    
    
} // namespace sora


#endif
