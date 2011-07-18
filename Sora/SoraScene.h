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
#include "SoraCanvas.h"
#include "SoraLayer.h"

#include <map>

namespace sora {
    
    class SORA_API SoraScene: public SoraObject {
    public:
        SoraScene(int32 width, int32 height);
        ~SoraScene();
        
        int32	getWidth() const;
        int32	getHeight() const;
        void    adjustSize(int32 width, int32 height);
        
        void setParent(SoraObject* obj);
        
        void    setRotation(float32 rot);
        float32 getRotation() const;
        
        void setScale(float32 scaleh, float32 scalev);
        void getScale(float32* scaleh, float32* scalev);
        
        
        /**
         *  add function inherited from SoraObject
         *  would use obj->getName() as the name of the object
         *  add layerDepth = 0(bottom layer) for layer 
         *  if the object being added is a scene
         *  would treat as subscene
         **/
        void add(SoraObject* obj);
        /**
         *  Add a object to the scene
         *  @param obj, object to add
         *  @param layerDepth, the layer depth of a layer, if the layer does not exist, then 
                a new layer with layerDepth would be added to the scene
         **/
        void add(SoraObject* obj, int32 layerDepth);
        
        /**
         *  Add a layer to the scene
         *  @param layer, the layer to be added
         *  @retval, the new layer
         **/
        SoraLayer* addLayer(int32 layerDepth);
        
        /**
         *  Remove a layer from the scene
         *  Would remove all objects attached to the layer
         *  @param layerDepth, the depth of the layer to be removed
         **/
        void removeLayer(int32 layerDepth);
        
        /**
         *  Set the depth of the layer
         *  Would adjust render sequence
         *  Notice that use SoraLayer::setLayerDepth wouldn't have any effect on the scene
         *  @param layerDepth, the depth of the layer to be set
         *  @param newDepth, the new depth for the layer
         **/
        void setLayerDepth(int32 layerDepth, int32 newDepth);
        
        /**
         *  Combine two layers together
         *  The depth would be the depth of the first layer
         *  @param layer1, the first layer to combine
         *  @param layer2, the second layer to combine
         **/
        void combineLayer(int32 layer1, int32 layer2);
        
        SoraLayer* getLayer(int32 layerDepth) const;
        SoraLayer* operator[](int32 layerDepth) const;
        
		virtual void    render();
		virtual uint32  update(float32 dt);
        
    private:
        int32 mWidth, mHeight;
        int32 mRealWidth, mRealHeight;
        
        float32 mRotation;
        float32 mHScale, mVScale;
        
        typedef std::map<int32, SoraLayer*> LayerMap;
        LayerMap mLayers;
        
        SoraScene* mParentScene;
    };
    
    
} // namespace sora


#endif
