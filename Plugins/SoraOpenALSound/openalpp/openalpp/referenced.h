/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#ifndef OPENALPP_REFERENCED
#define OPENALPP_REFERENCED 1


#include <openalpp/export.h>

// #define THREAD_SAFE_REF_UNREF 1

#ifdef THREAD_SAFE_REF_UNREF
    #include <OpenThreads/ScopedLock>
    #include <OpenThreads/Mutex>
#endif

namespace openalpp {


// forward declar, declared after Refenced below.
class DeleteHandler;


/** Base class from providing referencing counted objects.*/
class OPENALPP_API Referenced
{

    public:
        Referenced() 
        {
           
           _refCount=0;
        }
        Referenced(const Referenced&) {
            _refCount=0;
        }

        inline Referenced& operator = (const Referenced&) { return *this; }

        friend class DeleteHandler;

        /** Set a DeleteHandler to which deletion of all referenced counted objects
          * will be delegated to.*/
        static void setDeleteHandler(DeleteHandler* handler);

        /** Get a DeleteHandler.*/
        static DeleteHandler* getDeleteHandler();


        /** increment the reference count by one, indicating that 
            this object has another pointer which is referencing it.*/
        inline void ref() const
        { 
#ifdef THREAD_SAFE_REF_UNREF
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_refMutex); 
#endif
            ++_refCount;
        }
        
        /** decrement the reference count by one, indicating that 
            a pointer to this object is referencing it.  If the
            reference count goes to zero, it is assumed that this object
            is no longer referenced and is automatically deleted.*/
        inline void unref() const;
        
        /** decrement the reference count by one, indicating that 
            a pointer to this object is referencing it.  However, do
            not delete it, even if ref count goes to 0.  Warning, unref_nodelete() 
            should only be called if the user knows exactly who will
            be resonsible for, one should prefer unref() over unref_nodelete() 
            as the later can lead to memory leaks.*/
        inline void unref_nodelete() const { --_refCount; }
        
        /** return the number pointers currently referencing this object. */
        inline int referenceCount() const { return _refCount; }

       
    protected:
        virtual ~Referenced();
        
#ifdef THREAD_SAFE_REF_UNREF
        mutable OpenThreads::Mutex  _refMutex;
#endif
        mutable int                 _refCount;
        
};


/** Class for override the default delete behavior so that users can implment their own object
  * deletion schemes.  This might be done to help implement protection of multiple threads from deleting
  * objects unintentionally.
  * Note, the DeleteHandler cannot itself be reference counted, otherwise it
  * would be responsible for deleting itself!
  * An static auto_ptr<> is used internally in Referenced.cpp to manage the 
  * DeleteHandler's memory.*/
class DeleteHandler
{
    public:

        virtual ~DeleteHandler() {}

        /** flush any cache of objects that need to be deleted by doing an actual delete.*/
        virtual void flush() {}
        
        inline void doDelete(const Referenced* object) { delete object; }
         
        /** Request the deletion of an object. 
          * Depending on users implementation of DeleteHandler, the delete of the object may occur 
          * straight away or be delayed until doDelete is called.
          * The default implementation does a delete straight away.*/
        virtual void requestDelete(const Referenced* object) { doDelete(object); }
};

inline void Referenced::unref() const
{
    bool needDelete = false;
    {
#ifdef THREAD_SAFE_REF_UNREF
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_refMutex); 
#endif
        --_refCount;
        needDelete = _refCount<=0;
    }
    if (needDelete)
    {
        if (getDeleteHandler()) getDeleteHandler()->requestDelete(this);
        else delete this;
    }
}

}

#endif
