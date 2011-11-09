//
//  SoraModelLoader.h
//  Sora
//
//  Created by Ruiwei Bu on 11/6/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraModelLoader_h
#define Sora_SoraModelLoader_h

#include "SoraPlatform.h"
#include "SoraVertex.h"
#include "SoraDynamicFactory.h"

#include "util/SoraArray.h"

namespace sora {
    
    /**
     * Simple model loader class
     * Currently only support .obj from wavefront without meterials
     **/
    
    class SoraMesh;
    
    class SoraMeshFactory: public SoraDynamicFactory<SoraMesh> {
        
    };
    
    class SoraModelLoader {
    public:
        SoraModelLoader();
        SoraModelLoader(const StringType& file);
        ~SoraModelLoader();
        
        bool load(const StringType& file);
        
        size_t getFaceSize() const;
        size_t getVertexSize() const;
        
        const SoraArray<SoraVertex>& getVertexList() const;
        
    private:        
        void parseObjLine(const char* data);
        void parseObj(const char* data, uint32 length);
        
        struct Vertex {
            float x, y, z;
        };
        
        struct TexCoord {
            float x, y;
        };
        
        SoraArray<Vertex> vertices;
        SoraArray<TexCoord> texCoords;
        SoraArray<Vertex> normals;
        
        uint32 mFaceSize;
        SoraArray<SoraVertex> mVertexList;
    };
    
} // namespace sora



#endif
