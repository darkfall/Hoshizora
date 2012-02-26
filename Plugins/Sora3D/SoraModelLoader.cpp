//
//  SoraModelLoader.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/6/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraModelLoader.h"
#include "SoraResourceFile.h"
#include "SoraPath.h"

#include "SoraModel.h"
#include "SoraMesh.h"
#include "SoraMaterial.h"

#include "SoraMath.h"

#include "math/SoraPerlinNoise.h"

namespace sora {
    
    SoraModelLoaderFactory& SoraModelLoader::GetModelLoaderFactory() {
        static SoraModelLoaderFactory instance;
        return instance;
    }
    
    SoraModel::Ptr SoraModelLoader::LoadModel(const StringType& file) {
        SoraPath path(file.get());
        
        SoraModelLoader* loader = GetModelLoaderFactory().createInstance(path.getExtension());
        if(loader != 0) {
            SoraResourceFile data(file);
            if(data.isValid()) {
                SoraModel::Ptr model = loader->loadModelFromData(data, data.size());
                
                delete loader;
                return model;
            }
        }
        return SoraModel::Ptr(0);
    }
    
    SoraModel::Ptr SoraModelLoader::LoadModelFromRawData(void* data, uint32 size, const char* type) {
        SoraModelLoader* loader = GetModelLoaderFactory().createInstance(type);
        if(loader != 0) {
            SoraModel::Ptr model = loader->loadModelFromData((const char*)data, size);
            
            delete loader;
            return model;
        }
        return SoraModel::Ptr(0);
    }
    
    SoraModel::Ptr SoraModelLoader::BuildModelFromAABB(const SoraAABB3& aabb, SoraMaterial::Ptr mat) {
        SoraVertexN box[8];
        
        box[0].pos = aabb.min;
        box[0].normal = box[0].pos;
        
        box[1].pos.x = aabb.min.x;
        box[1].pos.y = aabb.min.y;
        box[1].pos.z = aabb.max.z;
        box[1].normal = box[1].pos;
        
        box[2].pos.x = aabb.max.x;
        box[2].pos.y = aabb.min.y;
        box[2].pos.z = aabb.max.z;
        box[2].normal = box[2].pos;
        
        box[3].pos.x = aabb.max.x;
        box[3].pos.y = aabb.min.y;
        box[3].pos.z = aabb.min.z;
        box[3].normal = box[3].pos;
        
        box[4].pos.x = aabb.min.x;
        box[4].pos.y = aabb.max.y;
        box[4].pos.z = aabb.min.z;
        box[4].normal = box[4].pos;
        
        box[5].pos.x = aabb.min.x;
        box[5].pos.y = aabb.max.y;
        box[5].pos.z = aabb.max.z;
        box[5].normal = box[5].pos;
        
        box[6].pos = aabb.max;
        box[6].normal = box[6].pos;
        
        box[7].pos.x = aabb.max.x;
        box[7].pos.y = aabb.max.y;
        box[7].pos.z = aabb.min.z;
        box[7].normal = box[7].pos;
        
        SoraMesh::Ptr mesh = new SoraMesh;
        
        mesh->beginVertex();
        if(mat->getMaterialType() == SoraMaterial::WireFrame) {
            mesh->pushVertex(box[0]);
            mesh->pushVertex(box[1]);
            
            mesh->pushVertex(box[0]);
            mesh->pushVertex(box[3]);
            
            mesh->pushVertex(box[0]);
            mesh->pushVertex(box[4]);
            
            mesh->pushVertex(box[5]);
            mesh->pushVertex(box[1]);
            
            mesh->pushVertex(box[5]);
            mesh->pushVertex(box[6]);
            
            mesh->pushVertex(box[5]);
            mesh->pushVertex(box[4]);
            
            mesh->pushVertex(box[2]);
            mesh->pushVertex(box[1]);
            
            mesh->pushVertex(box[2]);
            mesh->pushVertex(box[3]);
            
            mesh->pushVertex(box[2]);
            mesh->pushVertex(box[6]);
            
            mesh->pushVertex(box[7]);
            mesh->pushVertex(box[6]);
            
            mesh->pushVertex(box[7]);
            mesh->pushVertex(box[3]);
            
            mesh->pushVertex(box[7]);
            mesh->pushVertex(box[4]);
        } else {
            
            
        }
        mesh->endVertex();
        
        return new SoraModel(mesh, mat);
    }
        
    void getSphereVertex(float r, float a, float b, sora::SoraVertexN& vertex) {
        float sina = sinf(sora::DegreeToRadius(a));
        vertex.pos.x = r * sina * cosf(sora::DegreeToRadius(b));
        vertex.pos.y = r * sina * sinf(sora::DegreeToRadius(b));
        vertex.pos.z = r * cosf(sora::DegreeToRadius(a));
    
        vertex.normal = vertex.pos.normalize();
        
        double phi = acos(vertex.normal.z);
        vertex.v = phi / sora::D_PI;
        
        if(vertex.normal.z == 1.0f || vertex.normal.z == -1.0f) {
            vertex.u = 0.5f;
        } else {
            double u = acosf(SORA_MAX(SORA_MIN(vertex.normal.y / sin(phi), 1.0), -1.0)) / (2.0 * D_PI);
            
            vertex.u = vertex.normal.x > 0.f ? u : 1 - u;
        }
        
        vertex.col = 0xffffffff;
     //   vertex.col = sora::SoraColorRGBA(vertex.x / r, vertex.y / r, vertex.z / r, 1.0f);
    }
  
    SoraModel::Ptr SoraModelLoader::BuildModelFromSphere(const SoraSphere& sphere, uint32 slices, SoraMaterial::Ptr mat) {
        int w = 2 * slices, h = slices;
        float a = 0.0f, b = 0.0f;
        float hStep = 180.0f/(h-1);
        float wStep = 360.0f/w;
        int length = w * h;
        int i, j;
        
        SoraMesh::Ptr mesh = new SoraMesh();
        
        SoraArray<SoraVertexN> vertices(length, 0);
        
        for(a = 0.0, i = 0; i < h; i++, a += hStep)
            for(b = 0.0, j = 0; j < w; j++, b += wStep)  
                getSphereVertex(sphere.radius(), a, b, vertices[i*w+j]); 
        
        mesh->beginVertex();
        for(i=0; i < h-1; i++) {
            for(j=0; j < w-1; j++) {
                mesh->pushVertex(vertices[i*w+j]);
                mesh->pushVertex(vertices[i*w+j+1]);
                mesh->pushVertex(vertices[(i+1)*w+j+1]);
                
                mesh->pushVertex(vertices[(i+1)*w+j+1]);
                mesh->pushVertex(vertices[(i+1)*w+j]);
                mesh->pushVertex(vertices[i*w+j]);
            }
            mesh->pushVertex(vertices[i*w+j]);
            mesh->pushVertex(vertices[i*w]);
            mesh->pushVertex(vertices[(i+1)*w]);
            
            mesh->pushVertex(vertices[(i+1)*w]);
            mesh->pushVertex(vertices[(i+1)*w+j]);
            mesh->pushVertex(vertices[i*w+j]);
        }        
        mesh->endVertex();
        
        SoraModel::Ptr model = new SoraModel(mesh, mat);
        model->getTransform().setPosition(sphere.center());
        return model;
    }
    
    SoraModel::Ptr SoraModelLoader::BuildModelFromPlane(const SoraPlane& plane, SoraMaterial::Ptr mat) {
        return SoraModel::Ptr();
    }

    
} // namespace sora
