//
//  SoraExternalRenderObject.h
//  Sora
//
//  Created by Ruiwei Bu on 7/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraExternalRenderObject_h
#define Sora_SoraExternalRenderObject_h

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraDelegate.h"

namespace sora {
    
    /**
     *  A special object that adapts it's render function to a 
     *  external render delegate
     *  For use with subobjects such as SoraScene etc
     **/
    
    class SORA_API SoraExternalRenderObject: public SoraObject {
    public:
        typedef SoraAbstractDelegate<SoraObject> RenderDelegate;
        
        SoraExternalRenderObject();
        SoraExternalRenderObject(const RenderDelegate& delegate);
        ~SoraExternalRenderObject();
        
        void setDelegate(const RenderDelegate& delegate);
 
        virtual void render();
        
    private:
        RenderDelegate* mDelegate;
    };
    
}

#endif
