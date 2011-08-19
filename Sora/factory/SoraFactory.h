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
#include "../SoraAutoPtr.h"
#include "../function/SoraFunction.h"

#include "../uncopyable.h"


namespace sora {
    
    template<typename inter>
    inter* SoraFactoryCtorDelegate(void* sender, NamedValueList* param) {
        return new inter();
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
        typedef SoraFunction<T(NamedValueList*)> CreatorFn;
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
            reg(instanceName, CreatorFn(SoraFactoryCtorDelegate<product>));
        }
        
    private:
        CreatorFnMap mCreators;
    };
    
} // namespace sora



#endif
