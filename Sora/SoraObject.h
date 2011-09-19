#ifndef _SORA_OBJECT_H
#define _SORA_OBJECT_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"
#include "SoraAutoPtr.h"
#include "SoraMovable.h"
#include "SoraFunction.h"
#include <algorithm>
#include <list>


namespace sora {
    
    class SoraObjectHandle;
    class SoraAbstractModifierAdapter;

	class SORA_API SoraObject: public SoraEventHandler {
	public:
        friend class SoraObjectHandle;
        
		SoraObject();
		virtual ~SoraObject();

		virtual	int32	update(float dt);
		virtual void	render();
        
        // inherited from SoraEventHandler
        virtual void    onUpdate(float dt);
        
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
        virtual void    delAll();
    	
		virtual void    setPosition(float x, float y);
        float   getPositionX() const;
        float   getPositionY() const;
        void    getPosition(float* x, float* y) const;
        
        float   getAbsolutePositionX() const;
        float   getAbsolutePositionY() const;
        
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
        
        void setName(const StringType& name);
        void setName(SoraStringId n);
		SoraStringId getName() const;
        
        void moveTo(float x, float y, float t);
        
        typedef SoraFunction<void(SoraObject*)> NotificationFunc;
        void moveToAndNotify(float x, float y, float t, const NotificationFunc& onFinish);
        
    public:
        
        void addModifierAdapter(SoraAbstractModifierAdapter* adapter);
        void removeModifierAdapter(SoraAbstractModifierAdapter* adapter);
        bool hasModifierAdapter() const;
        void clearModifierAdapters();
        
    private:
        
        typedef std::list<SoraAbstractModifierAdapter*> ModifierAdapterList;
        ModifierAdapterList mModifierAdapters;
		
	protected:
		
        SoraObject* operator[](const SoraString& name);
        
	protected:
       
		SoraObject* mParent;
        SoraObject* mSubObjects;
        SoraObject* mNext;
		
		SoraMovable mPosition;
        
		uint32 mType;
        
        int32 mSubObjectSize;
        SoraStringId mName;
        
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
