//
//  SoraModel.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/9/11.
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
    
    SoraAABB3 SoraModel::getBoudingBox() const {
        return SoraAABB3::AABB3FromVertices(mMesh->mFaces.begin(), mMesh->mFaces.size());
    }
    
    void SoraModel::render() {
        SoraCore::Ptr->pushTransformMatrix();
        SoraCore::Ptr->getRenderSystem()->multTransformMatrix(getTransform().getTransformMatrix());
        
        mMaterial->attachShaderToRender();
        switch(mMaterial->getMaterialType()) {
            case SoraMaterial::WireFrame:
                SoraCore::Ptr->renderWithVertices(mMaterial->getTexture(0), BLEND_DEFAULT, mMesh->mFaces.begin(), mMesh->mFaces.size(), Line);
                break;
            case SoraMaterial::Solid:
                SoraCore::Ptr->renderWithVertices(mMaterial->getTexture(0), BLEND_DEFAULT, mMesh->mFaces.begin(), mMesh->mFaces.size(), Triangle);
                break;
            case SoraMaterial::PointCloud:
                SoraCore::Ptr->renderWithVertices(mMaterial->getTexture(0), BLEND_DEFAULT, mMesh->mFaces.begin(), mMesh->mFaces.size(), Point);
                break;
            default:
                break;
        }
        mMaterial->detachShaderFromRender();
        
        SoraCore::Ptr->popTransformMatrix();
    }
    
    int32 SoraModel::update() {
        return 0;
    }
    
} // namespace sora