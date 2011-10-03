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
    class SoraPhysicBody;
    class SoraPhysicFixtureDef;
    class SoraPhysicBodyDef;
    class SoraMessageEvent;

	class SORA_API SoraObject: public SoraEventHandler {
	public:
        friend class SoraObjectHandle;
        
		SoraObject();
		virtual ~SoraObject();

		virtual	int32	update(float dt);
		virtual void	render();
        
        // inherited from SoraEventHandler
        virtual void    onUpdate(float dt);
        
        virtual void    onPositionChange(float x, float y);
        virtual void    onParentChange(SoraObject* parent);
        virtual void    onMessage(SoraMessageEvent* message);
        
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
        virtual void    delAll();
    	
        void    setPosition(float x, float y);
        float   getPositionX() const;
        float   getPositionY() const;
        void    getPosition(float* x, float* y) const;
        
        float   getAbsolutePositionX() const;
        float   getAbsolutePositionY() const;
        
        void    setParent(SoraObject* obj);
        
		SoraObject*     getObjList() const;
		SoraObject*     getParent() const;
        
        int32           getObjSize() const;
        SoraObject*     getNext() const;
				
		SoraObject* getObjByName(const SoraString& n);
        SoraObject* getObjByName(SoraStringId sid);
		
		uint32  getType() const;
		void    setType(uint32 t);
        
        SoraHandle          getHandleId() const;
        SoraUniqueId        getUniqueId() const;
        SoraObjectHandle    getHandle();
        
        void setName(const StringType& name);
        void setName(SoraStringId n);
		SoraStringId getName() const;
        
        void moveTo(float x, float y, float t);
        
        typedef SoraFunction<void(SoraObject*)> NotificationFunc;
        void moveToAndNotify(float x, float y, float t, const NotificationFunc& onFinish);
        
        /**
         * If a physic body is binded, the object would update itself and move itself to
         * the body's position everyframe
         **/
        void            attachPhysicBody(SoraPhysicBody* body, bool autoRelease=false);
        void            createPhysicBody(const SoraPhysicFixtureDef& fixtureDef, float mass, float centerX, float centerY);
        void            detachPhysicBody();
        SoraPhysicBody* getPhysicBody() const;
        
    public:
        
        void addModifierAdapter(SoraAbstractModifierAdapter* adapter);
        void removeModifierAdapter(SoraAbstractModifierAdapter* adapter);
        bool hasModifierAdapter() const;
        void clearModifierAdapters();
        
    private:
        void  setNext(SoraObject* obj);

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
        
        bool mAutoReleasePhysicBody;
        SoraPhysicBody* mPhysicBody;
        
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
