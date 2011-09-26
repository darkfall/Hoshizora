//
//  SoraComponent.h
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponent_h
#define Sora_SoraComponent_h

#include "prerequisites.h"
#include "factory/SoraFactory.h"
#include "SoraLightWeightEntity.h"

namespace sora {
    
    /**
     * Helper macro to define a component class
     * Must be used within a class that derives from SoraComponent
     **/
    
    /**
     * Helper macro to define component name
     * Also insert a static GetName method to help fast cast between types
     **/
#define SORA_DEF_COMPONENT(cls) \
    virtual std::string getName() const { \
        return std::string(#cls); \
    } \
    static const std::string& GetName() { \
        static std::string name(#cls); \
        return name; \
    }
        
    class SORA_API SoraComponent {
    public:
        /**
         * When adding a Heavy weight component in Component Holder
         * All existing components would be informed
         * Otherwise only the component itself would be informed of the existence
         * of other components
         **/
        SoraComponent(SoraLightWeightEntity* owner, bool heavyWeight=true):
        mHeavyWeight(heavyWeight),
        mPropertyNoClassName(false),
        mOwner(0) {
            setOwner(owner);
        }
                
        virtual ~SoraComponent() {}
        
        SoraLightWeightEntity* getOwner() const {
            return mOwner;
        }
        
        void setOwner(SoraLightWeightEntity* entity) {
            mOwner = entity;
            onSetOwner(entity);
        }
        
        bool isHeavyWeight() const {
            return mHeavyWeight;
        }
        
        /**
         * If true, when adding property, would not append component class name 
         **/
        void setPropertyNoClassName(bool flag) {
            mPropertyNoClassName = flag;
        }
        
        bool isPropertyNoClassName() const {
            return mPropertyNoClassName;
        }
        
        /**
         * All components are required to have a unique name
         * Better be the class name
         * Suggest use SORA_IMPL_COMPONENT to implement this in your own class
         **/
        virtual std::string getName() const = 0;
        
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
        virtual void onUpdate(float dt) { }
        
        /**
         * Message that triggers when component being rendered
         * Useful for SoraObject architecture
         * Cause we seperated update and render logic
         * This is sometimes required if the component needs to do some render stuff
         **/
        virtual void onRender() { }
        
        virtual void onSetOwner(SoraLightWeightEntity* entity) { }
        
        template<typename T>
        void addProperty(const std::string& name, const T& val);
        
        SoraPropertyBase* getProperty(const std::string& prop) const;
        
    private:
        bool mHeavyWeight;
        bool mPropertyNoClassName;
        SoraLightWeightEntity* mOwner;
    };
    
    template<typename T>
    void SoraComponent::addProperty(const std::string& name, const T& val) {
        if(mOwner != 0) {
            if(mPropertyNoClassName) {
                mOwner->addProperty(MakeProperty(name, val));
            } else {
                mOwner->addProperty(MakeProperty(getName()+"."+name, val));
            }
        }
        else 
            THROW_SORA_EXCEPTION(RuntimeException, "caught component without owner");
    }
    
    typedef SoraAbstractFactory<SoraComponent, SoraComponent*(SoraLightWeightEntity*)> SoraComponentFactory;
    
    template<typename T>
    static void RegisterComponent(const SoraString& name, const T& fn) {
        SoraComponentFactory::Instance()->reg(name, fn);
    }
    
    template<typename T>
    static void RegisterComponent() {
        SoraComponentFactory::Instance()->reg_ctor<T>(T::GetName());
    }
    
    static SoraComponent* CreateComponent(const SoraString& name, SoraLightWeightEntity* owner) {
        return SoraComponentFactory::Instance()->createInstance(name, owner);
    }
    
    static void DestroyComponent(SoraComponent* comp) {
        sora_assert(comp);
        if(comp)
            delete comp;
    }
    
    template<typename T>
    inline T* ComponentCast(SoraComponent* comp) {
        if(!comp)
            return 0;
        
        if(comp->getName() == T::GetName()) {
            return static_cast<T*>(comp);
        }
        return 0;
    }
    
#define SORA_IMPL_COMPONENT(cls) \
    SORA_STATIC_RUN_CODE_I(sora_comp_reg##cls, ::sora::RegisterComponent<cls>())
    
} // namespace sora

#endif
