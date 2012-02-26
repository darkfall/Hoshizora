//
//  SoraModel.h
//  Sora
//
//  Created by Ruiwei Bu on 11/9/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraModel_h
#define Sora_SoraModel_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraMesh.h"
#include "SoraMaterial.h"

#include "SoraAABB3.h"

namespace sora {
    
    class SoraMesh;
    class SoraMaterial;
    
    class SORA_API SoraModel: public SoraObject {
    public:
        typedef SoraAutoPtr<SoraModel> Ptr;
        
        SoraModel();
        SoraModel(SoraMesh::Ptr mesh, SoraMaterial::Ptr mat);
        virtual ~SoraModel();
        
        SoraMesh::Ptr getMesh() const;
        SoraMaterial::Ptr getMaterial() const;
        
        void setMesh(SoraMesh::Ptr mesh);
        void setMaterial(SoraMaterial::Ptr mat);

        void render();
        int32 update(float dt);
        
    private:
        SoraMesh::Ptr mMesh;
        SoraMaterial::Ptr mMaterial;
    };
    
} // namespace sora



#endif
