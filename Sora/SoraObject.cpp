#include "SoraObject.h"
#include "SoraHandleManager.h"
#include "SoraObjectHandle.h"

namespace sora {

	SoraObject::SoraObject(): mType(0), mParent(0), mPosx(0.f), mPosy(0.f) {
        mUniqueId = GetNextUniqueId();
        mHandleId = FindFreeHandleSlot();
        
        // register this object to ObjectHandles
        SoraObjectHandle handle(this);
	}

	SoraObject::~SoraObject(){
		if(mParent) mParent->del(this);
        
        FreeHandleSlot(mHandleId);
	}
	
	uint32 SoraObject::update(float32 dt){
		SubObjectList::iterator itObj = mSubObjs.begin();
		while(itObj != mSubObjs.end()) {
			(*itObj)->update(dt);
			++itObj;
		}
	
		return 0;
	}
	
	void SoraObject::render() {
		SubObjectList::iterator itObj = mSubObjs.begin();
		while(itObj != mSubObjs.end()) {
			(*itObj)->render();
			++itObj;
		}
	}
	
	void SoraObject::setPosition(float32 _x, float32 _y) {
		mPosx = _x;
		mPosy = _y;
	}
	
	void SoraObject::getPosition(float32& _x, float32& _y) {
		_x = getPositionX();
		_y = getPositionY();
	}
	
	float32 SoraObject::getPositionX() {
        if(!mParent)
            return mPosx;
        return mPosx+mParent->getPositionX();
	}
	
	float32 SoraObject::getPositionY() {
        if(!mParent)
            return mPosy;
        return mPosy+mParent->getPositionY();
	}

	void SoraObject::add(SoraObject* o){
		mSubObjs.push_back(o);
        o->mParent = this;
	}
    
    void SoraObject::setParent(SoraObject* o) {
        this->mParent = o;
    }
/*
void SoraObject::addlua(LuaPlus::LuaObject p) {
	if(p.IsUserData()) {
		add(static_cast<SoraObject*>(*(void**)p.GetUserData()));
	}
}*/

	void SoraObject::del(SoraObject* o){
		mSubObjs.remove(o);
        o->mParent = 0;
	}
	
	SoraObject::SubObjectList SoraObject::getObjList() const {
		return mSubObjs;
	}
	
	SoraObject* SoraObject::getParent() const {
		return mParent;
	}

	SoraObject* SoraObject::getObjByName(const SoraString& n) {
        return getObjByName(str2id(n));
	}
    
    SoraObject* SoraObject::getObjByName(stringId sid) {        
		if(mName == sid) {
			return this;
		}
		
		SubObjectList::iterator itObj = mSubObjs.begin();
		while(itObj != mSubObjs.end()) {
			if((*itObj)->getName() == sid)
				return *itObj;
			++itObj;
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

} // namespace sora
