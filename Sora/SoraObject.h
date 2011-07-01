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
#include "SoraNamedObject.h"
#include "AutoContainer.h"

namespace sora {

	class SoraObject: public SoraEventHandler, public SoraNamedObject, public AutoListElement<SoraObject> {
	public:
		typedef std::list<SoraObject*> SUB_OBJECT_LIST;
		
		SoraObject();
		virtual ~SoraObject();

		virtual	uint32	update(float32 dt);
		virtual void	render();
		
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
		
		virtual void setPosition(float32 _x, float32 _y);
		virtual float32 getPositionX();
		virtual float32 getPositionY();
		// this uses getPositionX and getPositionY, so no need to be virtual 
		void getPosition(float32& _x, float32& _y);
		
		SUB_OBJECT_LIST getObjList() const;
		SoraObject* getParent() const;
        void setParent(SoraObject* obj);
				
		SoraObject* getObjByName(const SoraString& n);
		
		uint32 getType() const ;
		void setType(uint32 t);
	
	protected:
		SoraObject(const SoraObject&);
		SoraObject operator=(const SoraObject&);
		
		SoraObject* mParent;
		SUB_OBJECT_LIST mSubObjs;
		
		bool bFreeSubobjects;
		float32 mPosx, mPosy;
		uint32 mType;
		
		SoraString mName;
		uint32 mNameHash;
	};

} // namespace sora

#endif
