//
//  SoraJsonFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraJsonFactory_h
#define Sora_SoraJsonFactory_h

#include "../SoraPlatform.h"
#include "../json/value.h"
#include "../function/SoraFunction.h"

#include "uncopyable.h"

namespace sora {
    
    template<typename T>
    class SORA_API SoraAbstractJsonFactory: uncopyable {
    public:
        typedef T* PointerType;
        typedef Json::Value ParameterType;
        
    public:
        SoraAbstractJsonFactory() {}
        virtual ~SoraAbstractJsonFactory() {}
        
        virtual const std::string& getTypeName() const = 0;
        
        virtual PointerType createInstance(const std::string& instanceName, const ParameterType& parameter) = 0;
    };
    
    
    template<class T, const char* TypeName>
    class SORA_API SoraJsonFactory: public SoraAbstractJsonFactory<T> {
    public:
        typedef SoraFunction<T(const Json::Value&)> CreatorFn;
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
    public:
        virtual const std::string& getTypeName() const {
            static std::string type(TypeName);
            return type;
        }
        
        virtual T* createInstance(const std::string& instanceName, const Json::Value& param) {
            typename CreatorFnMap::iterator itCreator = mCreators.find(instanceName);
            if(itCreator != mCreators.end()) {
                return itCreator->second(param);
            }
        }
        
        void reg(const std::string& instanceName, CreatorFn fn) {
            mCreators.insert(std::make_pair(instanceName, fn));
        }
    
    private:
        CreatorFnMap mCreators;
    };
    
} // namespace sora


#endif
