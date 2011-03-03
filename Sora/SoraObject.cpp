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
			itObj->pointer()->update(dt);
			++itObj;
		}
	
		return 0;
	}
	
	void SoraObject::render() {
		SUB_OBJECT_LIST::iterator itObj = subobjs.begin();
		while(itObj != subobjs.end()) {
			itObj->pointer()->render();
			++itObj;
		}
	}
	
	void SoraObject::setPosition(float32 _x, float32 _y) {
		posx = _x;
		posy = _y;
	}
	
	void SoraObject::getPosition(float32& _x, float32& _y) {
		_x = posx;
		_y = posy;
	}
	
	float32 SoraObject::getPositionX() {
		return posx;
	}
	
	float32 SoraObject::getPositionY() {
		return posy;
	}

	void SoraObject::add(AP_OBJECT o){
		subobjs.push_back(o);
	}
/*
void SoraObject::addlua(LuaPlus::LuaObject p) {
	if(p.IsUserData()) {
		add(static_cast<SoraObject*>(*(void**)p.GetUserData()));
	}
}*/

	void SoraObject::del(AP_OBJECT o){
		subobjs.remove(o);
	}
	
	SoraObject::SUB_OBJECT_LIST SoraObject::getObjList() {
		return subobjs;
	}
	
	SoraObject::AP_OBJECT SoraObject::getParent() const {
		return parent;
	}

	void SoraObject::setName(const SoraString& n) {
		name = n;
		nameHash = BKDRHash(n.c_str());
	}
	
	SoraObject::AP_OBJECT SoraObject::getObjByName(const SoraString& n) {
		if(name.compare(n) == 0) {
			return this;
		}
		
		SUB_OBJECT_LIST::iterator itObj = subobjs.begin();
		while(itObj != subobjs.end()) {
			if(itObj->pointer()->getNameHash() == str2id(n))
				return *itObj;
			++itObj;
		}
		return AP_OBJECT();
	}
	

} // namespace sora
