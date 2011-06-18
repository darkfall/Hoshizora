#include "SoraEventManager.h"

#include "hash.h"
//#include "SoraLuaStateManager.h"

namespace sora {
	
	SoraEventHandlerPack::SoraEventHandlerPack() {
		registerEventFunc(this, &SoraEventHandlerPack::onTimerEvent);
	}
	
	SoraEventHandlerPack& SoraEventHandlerPack::add(SoraEventHandler* handler) {
		evHandlers.push_back(handler);
		return *this;
	}
	
	void SoraEventHandlerPack::onTimerEvent(SoraTimerEvent* ev) {
		EVENT_HANDLER_CONT::iterator itHandler = evHandlers.begin();
		while(itHandler != evHandlers.end()) {
			if((*itHandler)) {
				SoraTimerEvent* ccev = const_cast<SoraTimerEvent*>(ev);
				(*itHandler)->handleEvent((ccev));
				++itHandler;
			} else {
				itHandler = evHandlers.erase(itHandler);
			}
		}
	}
	
	void SoraEventHandlerPack::publishEvent(SoraEvent* ev) {
		EVENT_HANDLER_CONT::iterator itHandler = evHandlers.begin();
		while(itHandler != evHandlers.end()) {
			if((*itHandler)) {
				ev->setSource((*itHandler));
				(*itHandler)->handleEvent(ev);
				++itHandler;
			} else {
				itHandler = evHandlers.erase(itHandler);
			}
		}
	}
	
	void SoraEventHandlerPack::unRegister(SoraEventHandler* handler) {
		evHandlers.remove(handler);
	}

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
						itEvent->ev->setSource(itEvent->handler);
						itEvent->handler->handleEvent(itEvent->ev);
					}
					else {
						if(itEvent->handler == receiver) {
							itEvent->ev->setSource(itEvent->handler);
							itEvent->handler->handleEvent(itEvent->ev);
						}
					}
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
						ev->setSource(itEvent->handler);
						itEvent->handler->handleEvent(ev);
					}
					else
						if(itEvent->handler == receiver) {
							ev->setSource(itEvent->handler);
							itEvent->handler->handleEvent(ev);
						}
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
		tevList.push_back(new SoraTimerEventInfo(handler, time, 0.f, repeat)); 
	}
	
	void SoraEventManager::registerTimerEvent(SoraEventHandler* handler, SoraTimerEvent* ev, float32 time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(handler, ev, time, 0.f, repeat)); 
	}
	
	void SoraEventManager::registerTimerEvent(const SoraEventHandlerPack& pack, SoraTimerEvent* ev, float32 time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(pack, ev, time, 0.f, repeat));
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
				
				(*ittev)->update(dt);
				if((*ittev)->handlerPack.evHandlers.empty()) {
					freeTimerEvent(ittev);
					ittev = tevList.erase(ittev);
					continue;
				}
				++ittev;
			}
		}
	}
	
	void SoraEventManager::unregisterTimerEvent(SoraEventHandler* handler) {
		TIMER_EVENT_LIST::iterator ittev = tevList.begin();
		while(ittev != tevList.end()) {
			if((*ittev)) {
				(*ittev)->handlerPack.unRegister(handler);
				if((*ittev)->handlerPack.evHandlers.empty()) {
					freeTimerEvent(ittev);
					tevList.erase(ittev);
				}
			} else {
				ittev = tevList.erase(ittev);
				continue;
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
	
	void SoraEventManager::publishInputEvent(SoraKeyEvent* kev) {
		INPUT_EVENT_HANDLER_LIST::iterator itHandler = iehList.begin();
		while(itHandler != iehList.end()) {
			if((*itHandler)) {
				(*itHandler)->handleEvent(kev);
				++itHandler;
				
				if(kev->isConsumed())
					break;
			}
		}
	}
	
	void SoraEventManager::registerInputEventHandler(SoraEventHandler* handler) {
		iehList.push_back(handler);
	}
	
	void SoraEventManager::unregisterInputEventHandler(SoraEventHandler* handler) {
		iehList.remove(handler);
	}
	
} // namespace sora