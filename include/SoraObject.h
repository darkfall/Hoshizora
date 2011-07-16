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

	class SORA_API SoraObject: public SoraEventHandler, public SoraNamedObject, public AutoListElement<SoraObject> {
	public:
        friend class SoraObjectHandle;
        
		typedef std::list<SoraObject*> SubObjectList;
		
		SoraObject();
		virtual ~SoraObject();

		virtual	uint32	update(float32 dt);
		virtual void	render();
		
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
		
		virtual void    setPosition(float32 _x, float32 _y);
		virtual float32 getPositionX();
		virtual float32 getPositionY();
        virtual void    getPosition(float32& _x, float32& _y);
		
		SubObjectList   getObjList() const;
		SoraObject*     getParent() const;
        void            setParent(SoraObject* obj);
				
		SoraObject* getObjByName(const SoraString& n);
        SoraObject* getObjByName(stringId sid);
		
		uint32  getType() const;
		void    setType(uint32 t);
	
        SoraHandle      getHandleId() const;
        SoraUniqueId    getUniqueId() const;
        
	protected:
		SoraObject* mParent;
		SubObjectList mSubObjs;
		
		float32 mPosx;
        float32 mPosy;
		uint32 mType;
        
        SoraHandle mHandleId;
        SoraUniqueId mUniqueId;
    };

} // namespace sora

#endif
