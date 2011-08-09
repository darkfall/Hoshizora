#include "SoraObject.h"
#include "SoraHandleManager.h"
#include "SoraObjectHandle.h"

namespace sora {

	SoraObject::SoraObject(): 
    mParent(NULL),
    mSubObjects(NULL), 
    mNext(NULL),
    mPosx(0.f), 
    mPosy(0.f),
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
	
	void SoraObject::setPosition(float32 _x, float32 _y) {
		mPosx = _x;
		mPosy = _y;
	}
	
	void SoraObject::getPosition(float32& _x, float32& _y) const {
		_x = getPositionX();
		_y = getPositionY();
	}
	
	float32 SoraObject::getPositionX() const {
        if(!mParent)
            return mPosx;
        return mPosx+mParent->getPositionX();
	}
	
	float32 SoraObject::getPositionY() const {
        if(!mParent)
            return mPosy;
        return mPosy+mParent->getPositionY();
	}
    
    float32 SoraObject::getAbsolutePositionX() const {
        return mPosx;
    }
    
    float32 SoraObject::getAbsolutePositionY() const {
        return mPosy;
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
    
    SoraObject& SoraObject::operator =(const SoraObject& rhs) {
        if(&rhs != this) {
            mParent = rhs.mParent;
            mSubObjects = rhs.mSubObjects;
            mNext = rhs.mNext;
            
            mPosx = rhs.mPosx;
            mPosy = rhs.mPosy;
            mType = rhs.mType;
        }
        return *this;
    }
    
    SoraObject* SoraObject::operator[](const SoraString& name) {
        return getObjByName(name);
    }
} // namespace sora
