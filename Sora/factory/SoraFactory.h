//
//  SoraFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraFactory_h
#define Sora_SoraFactory_h

#include "../SoraPlatform.h"
#include "../SoraPreDeclare.h"

#include "../uncopyable.h"
#include "../SoraAutoPtr.h"

namespace sora {
    
    template<typename T>
    class SoraFactoryCreatorFuncBase {
    public:
        virtual T* doCreate(NamedValueList* param) = 0;
    };
    
    template<typename T>
    class SoraFactoryCreatorCFunc: public SoraFactoryCreatorFuncBase<T> {
    public:
        typedef T* (*CreatorFn)(NamedValueList*);
        SoraFactoryCreatorCFunc(CreatorFn fn):
        mFunc(fn) {
            
        }
        
        T* doCreate(NamedValueList* param) {
            return mFunc(param);
        }
        
    private:
        CreatorFn mFunc;
    };
    
    template<typename T, typename Obj>
    class SoraFactoryCreatorMemberFunc: public SoraFactoryCreatorFuncBase<T> {
    public:
        typedef T* (Obj::*CreatorFn)(NamedValueList*);
        SoraFactoryCreatorMemberFunc(Obj* obj, CreatorFn fn):
        mObj(obj),
        mFunc(fn) {
            
        }
        
        T* doCreate(NamedValueList* param) {
            return (mObj->*mFunc)(param);
        }
        
    private:
        CreatorFn mFunc;
        Obj* mObj;
    };
    
    template<typename T>
    class SORA_API SoraFactoryCreator {
    public:
        SoraFactoryCreator(typename SoraFactoryCreatorCFunc<T>::CreatorFn fn) {
            mCreatorImpl = new SoraFactoryCreatorCFunc<T>(fn);
        }
        
        template<typename Obj> 
        SoraFactoryCreator(Obj* obj, 
                           typename SoraFactoryCreatorMemberFunc<T, Obj>::CreatorFn fn) {
            mCreatorImpl = new SoraFactoryCreatorMemberFunc<T, Obj>(fn, obj);
        }
        
        ~SoraFactoryCreator() {
        }
        
        T* operator()(NamedValueList* param) {
            return (*mCreatorImpl).doCreate(param);
        }
        
    private:
        SoraAutoPtr<SoraFactoryCreatorFuncBase<T> > mCreatorImpl;
    };
    
    template<typename interface>
    interface* SoraFactoryCtorDelegate(void* sender, NamedValueList* param) {
        return new interface();
    }
    
    template<typename T>
    class SORA_API SoraAbstractFactory: uncopyable {
    public:
        typedef T* PointerType;
        typedef NamedValueList* ParameterType;
        typedef NamedValueList::iterator ParameterIterator;
        
    public:
        SoraAbstractFactory() {}
        virtual ~SoraAbstractFactory() {}
        
        virtual const std::string& getTypeName() const = 0;
        
        virtual PointerType createInstance(const std::string& instanceName, ParameterType parameter) = 0;
    };
    
    
    template<class T, const char* TypeName>
    class SORA_API SoraFactory: public SoraAbstractFactory<T> {
    public:
        typedef SoraFactoryCreator<T> CreatorFn;
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
    public:
        virtual const std::string& getTypeName() const {
            static std::string type(TypeName);
            return type;
        }
        
        virtual T* createInstance(const std::string& instanceName, NamedValueList* parameter=NULL) {
            typename CreatorFnMap::iterator itCreator = mCreators.find(instanceName);
            if(itCreator != mCreators.end()) {
                NamedValueList* param = parameter;
                if(param == NULL) {
                    static NamedValueList _emptyParamList;
                    param = &_emptyParamList;
                }
                return itCreator->second(param);
            }
        }
        
        void reg(const std::string& instanceName, CreatorFn fn) {
            mCreators.insert(std::make_pair(instanceName, fn));
        }
        
        template<typename product>
        void reg_ctor(const std::string& instanceName) {
            reg(instanceName, SoraFactoryCreator<product>(SoraFactoryCtorDelegate<product>));
        }
        
    private:
        CreatorFnMap mCreators;
    };
    
} // namespace sora



#endif
