//
//  HashStringMap.h
//  Sora
//
//  Created by Ruiwei Bu on 9/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_HashStringMap_h
#define Sora_HashStringMap_h

#include "SoraStringId.h"
#include <map>
#include <string>
namespace sora {
    
    template<typename VALUE>
    class HashStringMap {
    public:
        typedef std::map<SoraStringId, VALUE> MapType;
        typedef typename MapType::iterator iterator;
        typedef typename MapType::const_iterator const_iterator;
        typedef std::pair<std::string, VALUE> PairType;
        
        HashStringMap() {
        }
        
        VALUE& operator[](const std::string& name) {
            return mMap[GetUniqueStringId(name), false];
        }
                    
        void insert(const PairType& pair) {
            mMap.insert(std::make_pair(GetUniqueStringId(pair.first, false), pair.second));
        }
                        
        VALUE& operator[](SoraStringId sid) {
            return mMap[sid];
        }
        
        iterator begin() {
            return mMap.begin();
        }
            
        iterator end() {
            return mMap.end();
        }
        
        const_iterator begin() const {
            return mMap.begin();
        }
        
        const_iterator end() const {
            return mMap.end();
        }
        
    private:
        
        MapType mMap;
    };
    
};



#endif
