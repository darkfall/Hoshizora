//
//  SoraComponent.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponent_h
#define Sora_SoraComponent_h

#include "../property/SoraDynRTTIClass.h"
#include "../message/SoraMessageEvent.h"

namespace sora {
    
    /**
     * Helper macro to define a component class
     * Must be used within a class that derives from SoraComponent
     **/
    
#define SORA_IMPL_COMPONENT(cls) \
    virtual std::string getName() const { \
        return std::string(#cls); \
    }
    
    class SoraEntity;
    
    struct SORA_API SoraComponent: public SoraDynRTTIClass {
        /**
         * When adding a Heavy weight component in Component Holder
         * All existing components would be informed
         * Otherwise only the component itself would be informed of the existence
         * of other components
         **/
        SoraComponent(SoraEntity* owner, bool heavyWeight=true):
        SoraDynRTTIClass(getName()),
        mHeavyWeight(heavyWeight),
        mOwner(owner) {
            
        }
                
        virtual ~SoraComponent() {}
        
        /**
         * Owner set/get function
         * Use with care
         **/
        SoraEntity* getOwner() const {
            return mOwner;
        }
        
        void setOwner(SoraEntity* entity) {
            mOwner = entity;
        }
        
        bool isHeavyWeight() const {
            return mHeavyWeight;
        }
        
        /**
         * All components are required to have a unique name
         * Better be the class name
         * Suggest use SORA_IMPL_COMPONENT to implement this in your own class
         **/
        virtual std::string getName() const {
			return "ComponentBase";
		}
        
        /**
         * Components must implement message protocol 
         **/
        virtual void onMessage(SoraMessageEvent* message) = 0;
        
        /**
         * A heavy weight component would be informed if a new component
         * have been added to the holder
         * All component would receive this message when it's being added
         * to a holder
         **/
        virtual void onComponentAdd(SoraComponent* comp) { };
        
        /**
         * Only heavy weight component would receive this message
         * when a component has been removed from the holder
         **/
        virtual void onComponentRemove(SoraComponent* comp) { };
        
        /**
         * Message that triggers when component being added to a holder
         **/
        virtual void onInit() { }
        
        /**
         * Message that triggers when component being removed from a holder
         **/
        virtual void onRemove() { }
        
        /**
         * Message that triggers when component being updated
         **/
        virtual void onUpdate() { }
        
        /**
         * Message that triggers when component being rendered
         * Useful for SoraObject architecture
         * Cause we seperated update and render logic
         * This is sometimes required if the component needs to do some render stuff
         **/
        virtual void onRender() { }
        
    private:
        bool mHeavyWeight;
        SoraEntity* mOwner;
    };
    
} // namespace sora

#endif
