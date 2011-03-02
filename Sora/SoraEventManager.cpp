#include "SoraEventManager.h"

#include "hash.h"
//#include "SoraLuaStateManager.h"

namespace sora {

	void SoraEventManager::registerEvent(const SoraString& eventName, SoraEventHandler* handler, SoraEvent* ev) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		evMap[eid].push_back(SoraEventInfo(eventName, handler, ev));
	}

	void SoraEventManager::sendMessage(const SoraString& eventName, const SoraString& params, ulong32 hreceiver) {
		SoraEventHandler* receiver = (SoraEventHandler*)hreceiver;
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent == evMap.end()) {
			// ·¢ËÍ¸øluaobjects
		//	SoraLuaObjectManager::Instance()->sendMessage(eventName, params, hreceiver);
			return;
		}

		EVENT_LIST elist = itEvent->second;
		if(elist.size() != 0) {
			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler != NULL && itEvent->ev != NULL) {
					if(receiver == NULL) {
						itEvent->handler->handleEvent(itEvent->ev);
					}
					else
						if(itEvent->handler == receiver)
							itEvent->handler->handleEvent(itEvent->ev);
				} else {
					if(itEvent->handler == NULL) {
						itEvent = elist.erase(itEvent);
						continue;
					}
				}
				++itEvent;
			}
		}

	//	SoraLuaObjectManager::Instance()->sendMessage(eventName, params, hreceiver);
	}
	
	void SoraEventManager::sendMessage(const SoraString& eventName, SoraEvent* ev, ulong32 hreceiver) {
		SoraEventHandler* receiver = (SoraEventHandler*)hreceiver;
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent == evMap.end()) {
			return;
		}
		
		EVENT_LIST elist = itEvent->second;
		if(elist.size() != 0) {
			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler != NULL) {
					if(receiver == NULL) {
						itEvent->handler->handleEvent(ev);
					}
					else
						if(itEvent->handler == receiver)
							itEvent->handler->handleEvent(ev);
				} else {
					if(itEvent->handler == NULL) {
						itEvent = elist.erase(itEvent);
						continue;
					}
				}
				++itEvent;
			}
		}
	}
	
	void SoraEventManager::unregisterEvent(const SoraString& eventName) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent != evMap.end())
			evMap.erase(itEvent);
	}
	
	void SoraEventManager::unregisterHandlerFromEvent(const SoraString& eventName, SoraEventHandler* handler) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent != evMap.end()) {
			EVENT_LIST elist = itEvent->second;

			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler == handler) {
					elist.erase(itEvent);
				}
				++itEvent;
			}
		}
	}
	
	void SoraEventManager::createTimerEvent(SoraEventHandler* handler, float32 time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(handler, time+currTime, currTime, repeat)); 
	}
	
	void SoraEventManager::registerTimerEvent(SoraEventHandler* handler, SoraTimerEvent* ev, float32 time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(handler, ev, time+currTime, currTime, repeat)); 
	}
	
	void SoraEventManager::registerTimerEvent(const SoraEventHandlerPack& pack, SoraTimerEvent* ev, float32 time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(pack, ev, time+currTime, currTime, repeat));
	}
	
	void SoraEventManager::freeTimerEvent(TIMER_EVENT_LIST::iterator ittev) {
		delete (*ittev);
		(*ittev) = 0;
	}
	
	void SoraEventManager::update(float32 dt) {
		currTime += dt;
		if(tevList.size() != 0) {
			TIMER_EVENT_LIST::iterator ittev = tevList.begin();
			while(ittev != tevList.end()) {
				if((*ittev)->time < currTime) {
					(*ittev)->handlerPack.onTimerEvent((*ittev)->ev);
					if(!(*ittev)->repeat) {
						freeTimerEvent(ittev);
						ittev = tevList.erase(ittev);
						continue;
					} else {
						(*ittev)->time = (*ittev)->time*2-(*ittev)->currTime;
					}
					if((*ittev)->handlerPack.evHandlers.empty()) {
						freeTimerEvent(ittev);
						ittev = tevList.erase(ittev);
					}
				}
				
				++ittev;
			}
		}
	}
	
	void SoraEventManager::unregisterTimerEvent(SoraEventHandler* handler) {
		TIMER_EVENT_LIST::iterator ittev = tevList.begin();
		while(ittev != tevList.end()) {
			(*ittev)->handlerPack.unRegister(handler);
			if((*ittev)->handlerPack.evHandlers.empty()) {
				freeTimerEvent(ittev);
				tevList.erase(ittev);
			}
			++ittev;
		}
	}
	
	void SoraEventManager::unregisterTimerEvent(SoraEventHandlerPack* pack) {
		TIMER_EVENT_LIST::iterator ittev = tevList.begin();
		while(ittev != tevList.end()) {
			if(pack == (&(*ittev)->handlerPack)) {
				freeTimerEvent(ittev);
				tevList.erase(ittev);
			}
			++ittev;
		}
	}
	
} // namespace sora