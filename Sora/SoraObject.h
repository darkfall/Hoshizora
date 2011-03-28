#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"
#include "SoraAutoPtr.h"

//#include <LuaPlus.h>

#ifndef _SORA_OBJECT_H
#define _SORA_OBJECT_H

#include <algorithm>
#include <list>
#include "SoraAutoPtr.h"

namespace sora {

	class SoraObject: public SoraEventHandler {
	public:
		typedef SoraObject* PSORAOBJECT;
		typedef SoraObject* AP_OBJECT;
		typedef std::list<AP_OBJECT> SUB_OBJECT_LIST;
		
		SoraObject();
		virtual ~SoraObject();

		virtual	uint32	update(float32 dt);
		virtual void	render();
		
		virtual void	add(AP_OBJECT pobj);
		virtual void	del(AP_OBJECT pobj);
		
		virtual void setPosition(float32 _x, float32 _y);
		virtual void getPosition(float32& _x, float32& _y);
		virtual float32 getPositionX() const;
		virtual float32 getPositionY() const;
		
		SUB_OBJECT_LIST getObjList();
		AP_OBJECT getParent() const;
        void setParent(AP_OBJECT obj);
				
		AP_OBJECT getObjByName(const SoraString& n);
		
		uint32 getType() const { return type; }
		void setType(uint32 t) { type = t; }
	
	private:
		SoraObject(const SoraObject&);
		SoraObject operator=(const SoraObject&);
		
		PSORAOBJECT parent;
		SUB_OBJECT_LIST subobjs;
		
		bool bFreeSubobjects;
		float32 posx, posy;
		uint32 type;
		
		SoraString name;
		uint32 nameHash;
	};

} // namespace sora

#endif
