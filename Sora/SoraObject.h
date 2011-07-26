#ifndef _SORA_OBJECT_H
#define _SORA_OBJECT_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"
#include "SoraAutoPtr.h"
#include "SoraNamedObject.h"
#include <algorithm>
#include <list>


namespace sora {
    
    class SoraObjectHandle;

	class SORA_API SoraObject: public SoraEventHandler, public SoraNamedObject {
	public:
        friend class SoraObjectHandle;
        
		SoraObject();
		virtual ~SoraObject();

		virtual	uint32	update(float32 dt);
		virtual void	render();
		
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
        virtual void    delAll();
		
		virtual void    setPosition(float32 _x, float32 _y);
		virtual float32 getPositionX();
		virtual float32 getPositionY();
        virtual void    getPosition(float32& _x, float32& _y);
        
        virtual float32 getAbsolutePositionX();
        virtual float32 getAbsolutePositionY();
        
        virtual void    setParent(SoraObject* obj);
        
		SoraObject*     getObjList() const;
		SoraObject*     getParent() const;
        
        int32           getObjSize() const;
        SoraObject*     next() const;
				
		SoraObject* getObjByName(const SoraString& n);
        SoraObject* getObjByName(stringId sid);
		
		uint32  getType() const;
		void    setType(uint32 t);
        
        void        setNext(SoraObject* obj);
	
        SoraHandle          getHandleId() const;
        SoraUniqueId        getUniqueId() const;
        SoraObjectHandle    getHandle();
        
        /**
         *  handleId and UniqueId for the object cannot be copied
         **/
        SoraObject& operator =(const SoraObject& rhs);
        
        SoraObject* operator[](const SoraString& name);
        
	protected:
		SoraObject* mParent;
        SoraObject* mSubObjects;
        SoraObject* mNext;
		
		float32 mPosx;
        float32 mPosy;
		uint32 mType;
        
        int32 mSubObjectSize;
        
    private:
        SoraHandle mHandleId;
        SoraUniqueId mUniqueId;
    };

} // namespace sora

#endif
