#include "SoraObject.h"
#include "SoraHandleManager.h"
#include "SoraObjectHandle.h"

namespace sora {

	SoraObject::SoraObject(): 
    mParent(NULL),
    mSubObjects(NULL), 
    mNext(NULL),
    mPositionSource(NULL),
    mPosition(SoraVector3()),
    mType(0), 
    mSubObjectSize(0) {
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
            obj = obj->next();
        }
        mNext = NULL;
        mSubObjects = NULL;
        mParent = NULL;
        FreeHandleSlot(mHandleId);
	}
	
	uint32 SoraObject::update(float32 dt) {
        if(mSubObjectSize != 0) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL) {
                obj->update(dt);
                obj = obj->next();
            }
        }
	
		return 0;
	}
	
	void SoraObject::render() {
        if(mSubObjectSize != 0) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL) {
                obj->render();
                obj = obj->next();
            }
        }
	}
    
    void SoraObject::setPositionSource(PositionSource source) {
        mPositionSource = source;
        mPosition = mPositionSource->getPosition();
    }
	
	void SoraObject::setPosition(float32 x, float32 y) {
        if(mPositionSource) {
            mPositionSource->setPosition(x, y);
            mPosition = mPositionSource->getPosition();
        } else {
            mPosition.x = x;
            mPosition.y = y;
        }
	}
	
	void SoraObject::getPosition(float32& x, float32& y) const {
		x = getPositionX();
		y = getPositionY();
	}
	
	float32 SoraObject::getPositionX() const {
        float32 x = mPositionSource ? mPositionSource->getPositionX() : mPosition.x;
        if(!mParent)
            return x;
        return x+mParent->getPositionX();
	}
	
	float32 SoraObject::getPositionY() const {
        float32 y = mPositionSource ? mPositionSource->getPositionY() : mPosition.y;

        if(!mParent)
            return y;
        return y+mParent->getPositionY();
	}
    
    float32 SoraObject::getAbsolutePositionX() const {
        return mPositionSource ? mPositionSource->getPositionX() : mPosition.x;
    }
    
    float32 SoraObject::getAbsolutePositionY() const {
        return mPositionSource ? mPositionSource->getPositionY() : mPosition.y;
    }

	void SoraObject::add(SoraObject* o){
        if(o == this) 
            return;
        
        if(mSubObjects) {
            SoraObject* obj = mSubObjects;
            while(obj->next() != NULL)
                obj = obj->next();
            obj->mNext = o;
            o->setParent(this);
        } else {
            mSubObjects = o;
            o->setParent(this);
        }
        
        ++mSubObjectSize;
	}
    
    SoraObject* SoraObject::next() const {
        return mNext;
    }
    
    void SoraObject::setNext(SoraObject* obj) {
        mNext = obj;
    }

	void SoraObject::del(SoraObject* o) {
        if(o == NULL || o == this)
            return;
        if(mSubObjects == o)
            mSubObjects = mSubObjects->next();
        else if(mSubObjects) {
            SoraObject* obj = mSubObjects;
            while(obj != NULL && obj->next() != o)
                obj = obj->next();
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
    
    SoraObject* SoraObject::getObjByName(stringId sid) {        
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
    }
    
    int32 SoraObject::getObjSize() const {
        return mSubObjectSize;
    }
    
    SoraObject* SoraObject::operator[](const SoraString& name) {
        return getObjByName(name);
    }
} // namespace sora
