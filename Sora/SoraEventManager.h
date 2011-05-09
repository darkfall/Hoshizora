#ifndef SORA_EVENT_MANAGER_H
#define SORA_EVENT_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraEvent.h"

#include <map>
#include <vector>

#include "SoraTimerEvent.h"
#include "QuickList.h"
#include "hash.h"
#include "SoraKeyInfo.h"

namespace sora {
	
	/****
	 ALL Events handled by SoraEventManager would not be deleted
	 You should care about the memory by yourself
	 ALL Timered events would be deleted automatically except those passed by yourself
	 ****/
	
	class SoraEventHandlerPack: public SoraEventHandler {
		friend class SoraEventManager;
		
	public:
		SoraEventHandlerPack();
		SoraEventHandlerPack& add(SoraEventHandler* handler) ;
		void onTimerEvent(const SoraTimerEvent* ev);
		void publishEvent(SoraEvent* ev);
		
		void unRegister(SoraEventHandler* handler);
		
	protected:
		typedef std::list<SoraEventHandler*> EVENT_HANDLER_CONT;
		EVENT_HANDLER_CONT evHandlers;
	};

	class SoraEventManager: public SoraSingleton<SoraEventManager> {
		friend class SoraSingleton<SoraEventManager>;
		
		SoraEventManager(): currTime(0.f) {}
	public:
		//~SoraEventManager();

		void SORACALL registerEvent(const SoraString& eventName, SoraEventHandler* handler, SoraEvent* ev);
		void SORACALL unregisterEvent(const SoraString& eventName);
		void SORACALL unregisterHandlerFromEvent(const SoraString& eventName, SoraEventHandler* handler);

		/*
		 if ev == 0 in registerEvent
		 then call this sendMessage will do nothing
		 */
		void SORACALL sendMessage(const SoraString& eventName, const SoraString& params, ulong32 receiver=0);
		/*
		 publish events to a all handlers under eventName
		 */
		void SORACALL sendMessage(const SoraString& eventName, SoraEvent* ev, ulong32 receiver=0);
		
		/*
		 create a timer event
		 handler would receive a event, event type = SoraTimerEvent after time seconds
		 */
		void SORACALL createTimerEvent(SoraEventHandler* handler, float32 time, bool repeat=false);
		void SORACALL createTimerEvent(const SoraEventHandlerPack& pack, float32 time, bool repeat=false);
		/*
		 register a timer event
		 inheritate your class from SoraTimerEvent to pass your data to handler
		 */
		void SORACALL registerTimerEvent(SoraEventHandler* handler, SoraTimerEvent* ev, float32 time, bool repeat=false);
		void SORACALL unregisterTimerEvent(SoraEventHandler* handler);
		void SORACALL registerTimerEvent(const SoraEventHandlerPack& handler, SoraTimerEvent* ev, float32 time, bool repeat=false);
		void SORACALL unregisterTimerEvent(SoraEventHandlerPack* handler);
		
		/*
		 you should call this in your key event pool in order to let components know there is a key input event 
		 */
		void SORACALL publishInputEvent(SoraKeyEvent* kev);
		/*
		 register key input event handlers
		 */
		void SORACALL registerInputEventHandler(SoraEventHandler* handler);
		void SORACALL unregisterInputEventHandler(SoraEventHandler* handler);
		
		void SORACALL update(float32 dt);

	private:
		class SoraEventInfo {
		public:
			friend class SoraEventManager;

			SoraEventHandler* handler;
			SoraEvent* ev;

			SoraString eventId;

			SoraEventInfo(const SoraString& ev, SoraEventHandler* h, SoraEvent* e) :
				eventId(ev), handler(h), ev(e) {}
		};
		
		typedef uint32 EVENT_ID;
		typedef std::vector<SoraEventInfo> EVENT_LIST;
		typedef hash_map<EVENT_ID, EVENT_LIST > EVENT_MAP;
		EVENT_MAP evMap;
		
		class SoraTimerEventInfo {
		public:
			friend class SoraEventManager;
			
			~SoraTimerEventInfo() {
				if(internalte) {
					delete ev;
					ev = 0;
				}
			}
			
			SoraEventHandlerPack handlerPack;
			SoraTimerEvent* ev;
			
			float32 time;
			float32 currTime;
			float32 totalTime;
			bool repeat;
			bool internalte;
			
			void update(float32 dt) {
				currTime += dt;
				totalTime += dt;
				if(currTime >= time) {
					ev->setTime(currTime);
					ev->setTotalTime(totalTime);
					handlerPack.onTimerEvent(ev);
					
					currTime = 0.f;
				}
			}
			
			SoraTimerEventInfo(SoraEventHandler* h, float32 _time, float32 _currTime, bool _repeat) :
				time(_time), repeat(_repeat), internalte(true), currTime(_currTime), totalTime(0.f) {
					ev = new SoraTimerEvent;
					handlerPack.add(h);
				}
			SoraTimerEventInfo(SoraEventHandler* h, SoraTimerEvent* _ev, float32 _time, float32 _currTime, bool _repeat) :
				ev(_ev), time(_time), repeat(_repeat), internalte(false), currTime(_currTime), totalTime(0.f) {
					handlerPack.add(h);
				}
			SoraTimerEventInfo(const SoraEventHandlerPack& pack, float32 _time, float32 _currTime, bool _repeat):
				handlerPack(pack), time(_time), repeat(_repeat), currTime(_currTime), totalTime(0.f) {
					ev = new SoraTimerEvent;
				}
			SoraTimerEventInfo(const SoraEventHandlerPack& pack, SoraTimerEvent* _ev, float32 _time, float32 _currTime, bool _repeat) :
				ev(_ev), time(_time), repeat(_repeat), internalte(false), handlerPack(pack), currTime(_currTime), totalTime(0.f) {
				}
		};
		
		typedef std::list<SoraTimerEventInfo*> TIMER_EVENT_LIST;
		TIMER_EVENT_LIST tevList;
		float32 currTime;
		
		typedef std::list<SoraEventHandler*> INPUT_EVENT_HANDLER_LIST;
		INPUT_EVENT_HANDLER_LIST iehList;
		
		inline void freeTimerEvent(TIMER_EVENT_LIST::iterator ittev);
	};
	
	class SoraLuaEvent: public SoraEvent {
	public:
		SoraLuaEvent(const SoraString& _func, const SoraString& _param) {
			func = _func;
			param = _param;
		}
		
		std::string func;
		std::string param;
	};
	
#define SORA_EVENT_MANAGER SoraEventManager::Instance()
} // namespace sora


#endif