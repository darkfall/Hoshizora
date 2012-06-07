//
//  SoraModel.cpp
//  Sora
//
//  Created by Robert Bu on 11/9/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraModel.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"
#include "SoraSprite.h"

#include "SoraSphere.h"
#include "SoraPlane.h"

namespace sora {
    
    SoraModel::SoraModel(): 
    mMesh(new SoraMesh),
    mMaterial(new SoraMaterial) {
        
    }
    
    SoraModel::SoraModel(SoraMesh::Ptr mesh, SoraMaterial::Ptr mat):
    mMesh(mesh),
    mMaterial(mat) {
        
    }
    
    SoraModel::~SoraModel() {
        
    }
    
    SoraMesh::Ptr SoraModel::getMesh() const {
        return mMesh;
    }
    
    SoraMaterial::Ptr SoraModel::getMaterial() const {
        return mMaterial;
    }
    
    void SoraModel::setMesh(SoraMesh::Ptr mesh) {
        mMesh = mesh;
    }
    
    void SoraModel::setMaterial(SoraMaterial::Ptr mat) {
        mMaterial = mat;
    }
    
    void SoraModel::render() {
        SoraRenderSystem* rs = SoraCore::Ptr->getRenderSystem();
        SoraCore::Ptr->pushTransformMatrix();
        rs->multTransformMatrix(getTransform().getTransformMatrix());
        
        mMaterial->attachShaderToRender();
        switch(mMaterial->getMaterialType()) {
            case SoraMaterial::WireFrame:
                rs->renderBuffer(SoraTexture::TextureFromHandle(mMaterial->getTexture(0)),
                                 Line, 
                                 mMesh->getVertexBuffer(), 
                                 mMesh->getIndexBuffer());
                break;
            case SoraMaterial::Solid:
                rs->renderBuffer(SoraTexture::TextureFromHandle(mMaterial->getTexture(0)),
                                 Triangle, 
                                 mMesh->getVertexBuffer(), 
                                 mMesh->getIndexBuffer());
                break;
            case SoraMaterial::PointCloud:
                rs->renderBuffer(SoraTexture::TextureFromHandle(mMaterial->getTexture(0)),
                                 Point, 
                                 mMesh->getVertexBuffer(), 
                                 mMesh->getIndexBuffer());
                break;
            default:
                break;
        }
        mMaterial->detachShaderFromRender();
        
        SoraCore::Ptr->popTransformMatrix();
    }
    
    int32 SoraModel::update(float dt) {
        SoraObject::update(dt);
        return 0;
    }
    
} // namespace sora