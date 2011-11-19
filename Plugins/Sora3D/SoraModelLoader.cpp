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
        SoraVertex box[8];
        
        box[0].x = aabb.min.x;
        box[0].y = aabb.min.y;
        box[0].z = aabb.min.z;
        
        box[1].x = aabb.min.x;
        box[1].y = aabb.min.y;
        box[1].z = aabb.max.z;
        
        box[2].x = aabb.max.x;
        box[2].y = aabb.min.y;
        box[2].z = aabb.max.z;
        
        box[3].x = aabb.max.x;
        box[3].y = aabb.min.y;
        box[3].z = aabb.min.z;
        
        box[4].x = aabb.min.x;
        box[4].y = aabb.max.y;
        box[4].z = aabb.min.z;
        
        box[5].x = aabb.min.x;
        box[5].y = aabb.max.y;
        box[5].z = aabb.max.z;
        
        box[6].x = aabb.max.x;
        box[6].y = aabb.max.y;
        box[6].z = aabb.max.z;
        
        box[7].x = aabb.max.x;
        box[7].y = aabb.max.y;
        box[7].z = aabb.min.z;
        
        SoraMesh::Ptr mesh = new SoraMesh;
        
        SoraMesh::FaceList& vl = mesh->mFaces;        
        
        if(mat->getMaterialType() == SoraMaterial::WireFrame) {
            vl.append(box[0]);
            vl.append(box[1]);
            
            vl.append(box[0]);
            vl.append(box[3]);
            
            vl.append(box[0]);
            vl.append(box[4]);
            
            vl.append(box[5]);
            vl.append(box[1]);
            
            vl.append(box[5]);
            vl.append(box[6]);
            
            vl.append(box[5]);
            vl.append(box[4]);
            
            vl.append(box[2]);
            vl.append(box[1]);
            
            vl.append(box[2]);
            vl.append(box[3]);
            
            vl.append(box[2]);
            vl.append(box[6]);
            
            vl.append(box[7]);
            vl.append(box[6]);
            
            vl.append(box[7]);
            vl.append(box[3]);
            
            vl.append(box[7]);
            vl.append(box[4]);
        } else {
            
            
        }
        
        return new SoraModel(mesh, mat);
    }
        
    void getSphereVertex(float r, float a, float b, sora::SoraVertex& vertex) {
        float sina = sinf(sora::DegreeToRadius(a));
        vertex.x = r * sina * cosf(sora::DegreeToRadius(b));
        vertex.y = r * sina * sinf(sora::DegreeToRadius(b));
        vertex.z = r * cosf(sora::DegreeToRadius(a));
        
        float heightM = sora::SoraPerlinNoise::Gen(vertex.x / r , vertex.y / r , vertex.z / r);
        
        vertex.x += vertex.x * heightM * 0.01;
        vertex.y += vertex.y * heightM * 0.01;
        vertex.z += vertex.z * heightM * 0.01;
        
        vertex.ty = acosf(vertex.z / r) / sora::F_PI;
        if(vertex.ty > 0.0f) {
            vertex.tx = acosf(vertex.x/(r*sinf(sora::F_PI * vertex.ty))) / sora::F_PI*2;
        } else {
            vertex.tx = (sora::F_PI + acosf(vertex.x/(r*sinf(sora::F_PI*vertex.ty)))) / sora::F_PI*2;
        }
        
        vertex.col = 0xffffffff;
      //  vertex.col = sora::SoraColorRGBA(vertex.x / r, vertex.y / r, vertex.z / r, 1.0f);
    }
  
    SoraModel::Ptr SoraModelLoader::BuildModelFromSphere(const SoraSphere& sphere, uint32 slices, SoraMaterial::Ptr mat) {
        int w = 2 * slices, h = slices;
        float a = 0.0f, b = 0.0f;
        float hStep = 180.0f/(h-1);
        float wStep = 360.0f/w;
        int length = w * h;
        int i, j;
        
        SoraMesh::Ptr mesh = new SoraMesh();
        
        SoraVertexArray& vl = mesh->mFaces;        
        SoraVertexArray vertices(length, 0);
        
        for(a = 0.0, i = 0; i < h; i++, a += hStep)
            for(b = 0.0, j = 0; j < w; j++, b += wStep)  
                getSphereVertex(sphere.radius(), a, b, vertices[i*w+j]); 
        
        for(i=0; i < h-1; i++) {
            for(j=0; j < w-1; j++) {
                vl.append(vertices[i*w+j]);
                vl.append(vertices[i*w+j+1]);
                vl.append(vertices[(i+1)*w+j+1]);
                
                vl.append(vertices[(i+1)*w+j+1]);
                vl.append(vertices[(i+1)*w+j]);
                vl.append(vertices[i*w+j]);
            }
            vl.append(vertices[i*w+j]);
            vl.append(vertices[i*w]);
            vl.append(vertices[(i+1)*w]);
            
            vl.append(vertices[(i+1)*w]);
            vl.append(vertices[(i+1)*w+j]);
            vl.append(vertices[i*w+j]);
        }        
        
        SoraModel::Ptr model = new SoraModel(mesh, mat);
        model->getTransform().setPosition(sphere.center());
        return model;
    }
    
    SoraModel::Ptr SoraModelLoader::BuildModelFromPlane(const SoraPlane& plane, SoraMaterial::Ptr mat) {
        return SoraModel::Ptr();
    }

    
} // namespace sora
