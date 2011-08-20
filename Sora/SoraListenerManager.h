//
//  SoraListenerManager.h
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraListenerManager_h
#define Sora_SoraListenerManager_h

#include "SoraPlatform.h"
#include <string>
#include <map>
#include <deque>

namespace sora {
    
    template<typename T>
    struct SoraListenerManager {  
        typedef T ListenerType;

        typedef std::map<std::string, ListenerType*> ListenerMap;
        typedef std::deque<std::pair<ListenerType*, bool> > ListenerList;

        virtual ~SoraListenerManager() {
            while(mListenerList.size() != 0) {
                std::pair<ListenerType*, bool> val = mListenerList.pop();
                if(val.second) {
                    delete val.first;
                }
            }
        }
        
        
        void attachListener(ListenerType*, const std::string& tag = std::string(), bool autoRelease=false);
        
        ListenerType* getListener(const std::string& tag) const;
        
    protected:
        ListenerMap mListenerMap;
        
        ListenerList mListenerList;
    };
    
    template<typename T>
    void SoraListenerManager<T>::attachListener(ListenerType* listener, const std::string& tag, bool autoRelease) {
        sora_assert(listener);
        if(!listener)
            return;
        
        mListenerList.push_back(typename ListenerList::value_type(listener, autoRelease));
        if(!tag.empty()) {
            typename ListenerMap::const_iterator it = mListenerMap.find(tag);
            if(it != mListenerMap.end()) {
                mListenerMap.insert(std::make_pair(tag, listener));
            }
        }
    }
    
    template<typename T>
    T* SoraListenerManager<T>::getListener(const std::string& tag) const {
        typename ListenerMap::const_iterator it = mListenerMap.find(tag);
        if(it != mListenerMap.end()) 
            return it->second;
        return NULL;
    }
                    
} // namespace sora


#endif
