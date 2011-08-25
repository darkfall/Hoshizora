//
//  SoraConfigurationTree.h
//  Sora
//
//  Created by Ruiwei Bu on 8/24/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraConfigurationTree_h
#define Sora_SoraConfigurationTree_h

#include "../SoraPlatform.h"
#include "../SoraIteratorHelper.h"

#include <map>

namespace sora {

    // a configuration tree is a abstract layer to load xml / json or other
    // kind of configuration files
    // it maybe slower cause we need to map all values to this when loading a configuration file
    // so use with causion
    
    typedef SoraString ConfigKeyType;
    
    namespace config {
       
        
    }
    
    class SoraConfigurationValue {
    public:
        SoraConfigurationValue();
        ~SoraConfigurationValue();
        
        bool has(const ConfigKeyType& key);
        
        SoraConfigurationValue get(const ConfigKeyType& key);
        
        ConfigKeyType getName() const;
        SoraConfigurationValue getValue() const;
        
        bool isNode() const;
        bool isValue() const;
        
        std::string asString();
        std::wstring asWString();
        int32 asInt();
        long asLong();
        float asFloat();
        double asDouble();
                
    private:
        friend class SoraConfigurationLoader;
        
        void setAsNode(const SoraConfigurationValue& subValue);
        void setAsValue(const std::string& val);
        
        SoraConfigurationValue& getValue(const ConfigKeyType& key);
        
    private:
    };
    
} // namespace sora


#endif
