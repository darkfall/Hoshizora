#ifndef _SORA_OBJECT_H
#define _SORA_OBJECT_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"
#include "SoraAutoPtr.h"
#include "SoraNamedObject.h"
#include "SoraMovable.h"
#include <algorithm>
#include <list>


namespace sora {
    
    class SoraObjectHandle;

	class SORA_API SoraObject: public SoraEventHandler, public SoraNamedObject {
	public:
        friend class SoraObjectHandle;
        
		SoraObject();
		virtual ~SoraObject();

		virtual	uint32	update(float dt);
		virtual void	render();
        
        // inherited from SoraEventHandler
        virtual void    onUpdate(float dt);
		
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
        virtual void    delAll();
        
        typedef SoraMovable* PositionSource;
        void setPositionSource(PositionSource source);
		
		virtual void    setPosition(float x, float y);
		virtual float getPositionX() const;
		virtual float getPositionY() const;
        virtual void    getPosition(float& x, float& y) const;
        
        virtual float getAbsolutePositionX() const;
        virtual float getAbsolutePositionY() const;
        
        virtual void    setParent(SoraObject* obj);
        
		SoraObject*     getObjList() const;
		SoraObject*     getParent() const;
        
        int32           getObjSize() const;
        SoraObject*     next() const;
				
		SoraObject* getObjByName(const SoraString& n);
        SoraObject* getObjByName(SoraStringId sid);
		
		uint32  getType() const;
		void    setType(uint32 t);
        
        void    setNext(SoraObject* obj);
	
        SoraHandle          getHandleId() const;
        SoraUniqueId        getUniqueId() const;
        SoraObjectHandle    getHandle();
        
        SoraObject* operator[](const SoraString& name);
        
	protected:
       
		SoraObject* mParent;
        SoraObject* mSubObjects;
        SoraObject* mNext;
		
		PositionSource mPositionSource;
        SoraVector3 mPosition;
        
		uint32 mType;
        
        int32 mSubObjectSize;
        
    private:
        /**
         *  handleId and UniqueId for the object cannot be copied
         **/
        SoraObject(const SoraObject& rhs);
        SoraObject& operator =(const SoraObject& rhs);
        
        SoraHandle mHandleId;
        SoraUniqueId mUniqueId;
    };

} // namespace sora

#endif
