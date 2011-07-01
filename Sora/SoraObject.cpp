#include "SoraObject.h"
#include "hash.h"
#include "stringId.h"

namespace sora {

	SoraObject::SoraObject(): mType(0), mParent(0), mNameHash(0), mPosx(0.f), mPosy(0.f) {
	}

	SoraObject::~SoraObject(){
		if(mParent) mParent->del(this);
	}
	
	uint32 SoraObject::update(float32 dt){
		SUB_OBJECT_LIST::iterator itObj = mSubObjs.begin();
		while(itObj != mSubObjs.end()) {
			(*itObj)->update(dt);
			++itObj;
		}
	
		return 0;
	}
	
	void SoraObject::render() {
		SUB_OBJECT_LIST::iterator itObj = mSubObjs.begin();
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
	
	SoraObject::SUB_OBJECT_LIST SoraObject::getObjList() const {
		return mSubObjs;
	}
	
	SoraObject* SoraObject::getParent() const {
		return mParent;
	}

	SoraObject* SoraObject::getObjByName(const SoraString& n) {
		if(mName.compare(n) == 0) {
			return this;
		}
		
		SUB_OBJECT_LIST::iterator itObj = mSubObjs.begin();
		stringId id = str2id(n);
		while(itObj != mSubObjs.end()) {
			if((*itObj)->getName() == id)
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

	

} // namespace sora
