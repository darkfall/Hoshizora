#include "SoraObject.h"
#include "hash.h"
#include "stringId.h"

namespace sora {

	SoraObject::SoraObject(): type(0), parent(0), nameHash(0) {
	}

	SoraObject::~SoraObject(){
		if(parent) parent->del(this);
	}
	
	uint32 SoraObject::update(float32 dt){
		SUB_OBJECT_LIST::iterator itObj = subobjs.begin();
		while(itObj != subobjs.end()) {
			(*itObj)->update(dt);
			++itObj;
		}
	
		return 0;
	}
	
	void SoraObject::render() {
		SUB_OBJECT_LIST::iterator itObj = subobjs.begin();
		while(itObj != subobjs.end()) {
			(*itObj)->render();
			++itObj;
		}
	}
	
	void SoraObject::setPosition(float32 _x, float32 _y) {
		posx = _x;
		posy = _y;
	}
	
	void SoraObject::getPosition(float32& _x, float32& _y) {
		_x = getPositionX();
		_y = getPositionY();
	}
	
	float32 SoraObject::getPositionX() {
        if(!parent)
            return posx;
        return posx+parent->getPositionX();
	}
	
	float32 SoraObject::getPositionY() {
        if(!parent)
            return posy;
        return posx+parent->getPositionY();
	}

	void SoraObject::add(AP_OBJECT o){
		subobjs.push_back(o);
        o->parent = this;
	}
    
    void SoraObject::setParent(AP_OBJECT o) {
        this->parent = o;
    }
/*
void SoraObject::addlua(LuaPlus::LuaObject p) {
	if(p.IsUserData()) {
		add(static_cast<SoraObject*>(*(void**)p.GetUserData()));
	}
}*/

	void SoraObject::del(AP_OBJECT o){
		subobjs.remove(o);
        o->parent = 0;
	}
	
	SoraObject::SUB_OBJECT_LIST SoraObject::getObjList() const {
		return subobjs;
	}
	
	SoraObject::AP_OBJECT SoraObject::getParent() const {
		return parent;
	}

	SoraObject::AP_OBJECT SoraObject::getObjByName(const SoraString& n) {
		if(name.compare(n) == 0) {
			return this;
		}
		
		SUB_OBJECT_LIST::iterator itObj = subobjs.begin();
		stringId id = str2id(n);
		while(itObj != subobjs.end()) {
			if((*itObj)->getName() == id)
				return *itObj;
			++itObj;
		}
		return NULL;
	}
	

} // namespace sora
