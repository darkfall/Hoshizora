#include "SoraObject.h"
#include "SoraHandleManager.h"
#include "SoraObjectHandle.h"
#include "SoraModifierAdapter.h"

#include "modifiers/SoraObjectModifiers.h"

#include "physics/SoraPhysicBody.h"
#include "physics/SoraPhysicWorld.h"

#include "message/SoraMessageEvent.h"

namespace sora {

	SoraObject::SoraObject(): 
    mParent(NULL),
    mSubObjects(NULL), 
    mNext(NULL),
    mPosition(SoraMovable()),
    mType(0), 
    mSubObjectSize(0),
    mAutoReleasePhysicBody(true),
    mPhysicBody(0) {
        mUniqueId = GetNextUniqueId();
        mHandleId = FindFreeHandleSlot();
        
        // register this object to ObjectHandles
        SoraObjectHandle handle(this);
	}

	SoraObject::~SoraObject(){
		if(mParent) mParent->del(this);
        SoraObject* obj = mSubObjects;
        while(obj != NULL) {
            obj->setParent(NULL);
            obj = obj->getNext();
        }
        mNext = NULL;
        mSubObjects = NULL;
        mParent = NULL;
        
        FreeHandleSlot(mHandleId);
        
        if(mAutoReleasePhysicBody && mPhysicBody) {
            SoraPhysicWorld::DestroyBody(mPhysicBody);
        }
	}
	
	int32 SoraObject::update(float dt) {
        if(mSubObjectSize != 0) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL) {
                obj->update(dt);
                obj = obj->getNext();
            }
        }
        
        if(mModifierAdapters.size() != 0) {
            ModifierAdapterList::iterator it = mModifierAdapters.begin();
            ModifierAdapterList::iterator end = mModifierAdapters.end();
            while(it != end) {
                SoraAbstractModifierAdapter* adapter = (*it);
                if(adapter->update(dt)) {
                    delete adapter;
                    it = mModifierAdapters.erase(it);
                    continue;
                }
                ++it;
            }
        }
        
        if(mPhysicBody) {
            SoraVector pos = mPhysicBody->getPosition();
            setPosition(pos.x, pos.y);
        }

		return 0;
	}
	
	void SoraObject::render() {
        if(mSubObjectSize != 0) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL) {
                obj->render();
                obj = obj->getNext();
            }
        }
	}
    
    void SoraObject::moveTo(float x, float y, float t) {
        addModifierAdapter(
                           CreateModifierAdapter(this,
                                                 new SoraObjectPositionModifier(getPositionX(),
                                                                                getPositionY(),
                                                                                x,
                                                                                y,
                                                                                t)));
        
    }
    
    void SoraObject::moveToAndNotify(float x, float y, float t, const SoraFunction<void(SoraObject*)>& onFinish) {
        addModifierAdapter(
                           CreateModifierAdapterWithNotification(this,
                                                                 new SoraObjectPositionModifier(getPositionX(),
                                                                                                getPositionY(),
                                                                                                x,
                                                                                                y,
                                                                                                t),
                                                                 onFinish));
                                                 
    }
    
    void SoraObject::onUpdate(float dt) {
        update(dt);
    }
	
	void SoraObject::setPosition(float x, float y) {
        mPosition.x = x;
        mPosition.y = y;
        onPositionChange(x, y);
	}
	
	void SoraObject::getPosition(float* x, float* y) const {
		*x = getPositionX();
		*y = getPositionY();
	}
	
	float SoraObject::getPositionX() const {
        float x = mPosition.x;
        if(!mParent)
            return x;
        return x+mParent->getPositionX();
	}
	
	float SoraObject::getPositionY() const {
        float y = mPosition.y;

        if(!mParent)
            return y;
        return y+mParent->getPositionY();
	}
    
    float SoraObject::getAbsolutePositionX() const {
        return mPosition.x;
    }
    
    float SoraObject::getAbsolutePositionY() const {
        return mPosition.y;
    }

	void SoraObject::add(SoraObject* o){
        if(o == this) 
            return;
        
        if(mSubObjects) {
            SoraObject* obj = mSubObjects;
            while(obj->getNext() != NULL)
                obj = obj->getNext();
            obj->mNext = o;
            o->setParent(this);
        } else {
            mSubObjects = o;
            o->setParent(this);
        }
        
        ++mSubObjectSize;
	}
    
    SoraObject* SoraObject::getNext() const {
        return mNext;
    }
    
    void SoraObject::setNext(SoraObject* obj) {
        mNext = obj;
    }

	void SoraObject::del(SoraObject* o) {
        if(o == NULL || o == this)
            return;
        if(mSubObjects == o)
            mSubObjects = mSubObjects->getNext();
        else if(mSubObjects) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL && obj->getNext() != o)
                obj = obj->getNext();
            if(obj != NULL) {
                obj->mNext = o->mNext;
                o->setParent(NULL);
                
                --mSubObjectSize;
            }
        }
	}
    
    void SoraObject::delAll() {
        mSubObjects = NULL;
    }
	
	SoraObject* SoraObject::getObjList() const {
		return mSubObjects;
	}
	
	SoraObject* SoraObject::getParent() const {
		return mParent;
	}

	SoraObject* SoraObject::getObjByName(const SoraString& n) {
        return getObjByName(GetUniqueStringId(n));
	}
    
    SoraObject* SoraObject::getObjByName(SoraStringId sid) {        
		if(mName == sid) {
			return this;
		}
		
		SoraObject* obj = mSubObjects;
        while(obj != NULL) {
            if(obj->getName() == sid) {
                return obj;
            }
        }
		return NULL;
    }
	
	uint32 SoraObject::getType() const { 
		return mType;
	}
	
	void SoraObject::setType(uint32 t) { 
		mType = t; 
	}

    SoraHandle SoraObject::getHandleId() const {
        return mHandleId;
    }
    
    SoraUniqueId SoraObject::getUniqueId() const {
        return mUniqueId;
    }

    SoraObjectHandle SoraObject::getHandle() {
        return SoraObjectHandle(this);
    }
    
    void SoraObject::setParent(SoraObject* obj) {
        mParent = obj;
        onParentChange(obj);
    }
    
    int32 SoraObject::getObjSize() const {
        return mSubObjectSize;
    }
    
    SoraObject* SoraObject::operator[](const SoraString& name) {
        return getObjByName(name);
    }
    
    void SoraObject::setName(const StringType& name) {
        setName(name.uniqueId());
    }
    
    void SoraObject::setName(SoraStringId n) { 
        mName = n; 
    }
   
    SoraStringId SoraObject::getName() const { 
        return mName;
    }
    
    void SoraObject::attachPhysicBody(SoraPhysicBody* body, bool autoRelease) {
        mPhysicBody = body;
        mAutoReleasePhysicBody = autoRelease;
    }
    
    void SoraObject::createPhysicBody(const SoraPhysicBodyDef& def, const SoraPhysicFixtureDef& fixtureDef, float mass, float centerX, float centerY) {        
        mPhysicBody = sora::SoraPhysicWorld::CreateBody(SoraPhysicBodyDef(def.mType, SoraVector(getPositionX(), getPositionY())));
        if(mPhysicBody) {
            mPhysicBody->createFixture(fixtureDef);
            mPhysicBody->setMass(mass, centerX, centerY);
            mAutoReleasePhysicBody = true;
        }
    }
    
    void SoraObject::detachPhysicBody() {
        mPhysicBody = 0;
    }
    
    SoraPhysicBody* SoraObject::getPhysicBody() const {
        return mPhysicBody;
    }
    
    void SoraObject::addModifierAdapter(SoraAbstractModifierAdapter* adapter) {
        mModifierAdapters.push_back(adapter);
    }
    
    void SoraObject::removeModifierAdapter(SoraAbstractModifierAdapter* adapter) {
        mModifierAdapters.remove(adapter);
    }
    
    bool SoraObject::hasModifierAdapter() const {
        return !mModifierAdapters.empty();
    }
    
    void SoraObject::clearModifierAdapters() {
        ModifierAdapterList::iterator it = mModifierAdapters.begin();
        ModifierAdapterList::iterator end = mModifierAdapters.end();
        for(; it != end; ++it) {
            delete (*it);
        }
        mModifierAdapters.clear();
    }
    
    void SoraObject::onPositionChange(float x, float y) {
        
    }
    
    void SoraObject::onParentChange(SoraObject* parent) {
        
    }
    
    void SoraObject::onMessage(SoraMessageEvent* message) {
        sora_assert(message);
        
        if(message->getReceiver() == this)
            this->onMessage(message);
        else {
            SoraObject* obj = getObjList();
            do {
                obj->onMessage(message);
                
                obj = obj->getNext();
            } while(!message->isConsumed() && obj != 0);
        }

    }
            
} // namespace sora
