#ifndef SORA_EVENT_MANAGER_H
#define SORA_EVENT_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraEvent.h"

#include <map>
#include <vector>

#include "SoraTimerEvent.h"
#include "SoraFileChangeEvent.h"
#include "SoraKeyInfo.h"
#include "SoraFileChangeEvent.h"

#include "QuickList.h"
#include "hash.h"

#include "support/md5lib.h"

namespace sora {
	
	/****
	 ALL Events handled by SoraEventManager would not be deleted
	 You should care about the memory by yourself
	 ALL Timered events would be deleted automatically except those passed by yourself
	 ****/
		
	class SORA_API SoraEventHandlerPack: public SoraEventHandler {
		friend class SoraEventManager;
		
	public:
		SoraEventHandlerPack();
		SoraEventHandlerPack& add(SoraEventHandler* handler) ;
		void onTimerEvent(SoraTimerEvent* ev);
		void publishEvent(SoraEvent* ev);
		
		void unRegister(SoraEventHandler* handler);
		
	protected:
		typedef std::list<SoraEventHandler*> EVENT_HANDLER_CONT;
		EVENT_HANDLER_CONT evHandlers;
	};

	class SORA_API SoraEventManager {
		SoraEventManager();
		~SoraEventManager();
		
	public:
        static SoraEventManager* Instance();
        
		//~SoraEventManager();

		void registerEvent(const SoraString& eventName, SoraEventHandler* handler, SoraEvent* ev);
		void unregisterEvent(const SoraString& eventName);
		void unregisterHandlerFromEvent(const SoraString& eventName, SoraEventHandler* handler);

		/*
		 if ev == 0 in registerEvent
		 then call this sendMessage will do nothing
		 */
		void sendMessage(const SoraString& eventName, const SoraString& params, ulong32 receiver=0);
		/*
		 publish events to a all handlers under eventName
		 */
		void sendMessage(const SoraString& eventName, SoraEvent* ev, ulong32 receiver=0);
		
		/*
		 create a timer event
		 handler would receive a event, event type = SoraTimerEvent after time seconds
		 */
		void createTimerEvent(SoraEventHandler* handler, float32 time, bool repeat=false);
		void createTimerEvent(const SoraEventHandlerPack& pack, float32 time, bool repeat=false);
		/*
		 register a timer event
		 inheritate your class from SoraTimerEvent to pass your data to handler
		 */
		void registerTimerEvent(SoraEventHandler* handler, SoraTimerEvent* ev, float32 time, bool repeat=false);
		void unregisterTimerEvent(SoraEventHandler* handler);
		void registerTimerEvent(const SoraEventHandlerPack& handler, SoraTimerEvent* ev, float32 time, bool repeat=false);
		void unregisterTimerEvent(SoraEventHandlerPack* handler);
		
		/*
		 you should call this in your key event pool in order to let components know there is a key input event 
		 */
		void publishInputEvent(SoraKeyEvent* kev);
		/*
		 register key input event handlers
		 */
		void registerInputEventHandler(SoraEventHandler* handler, int32 priority=0);
		void unregisterInputEventHandler(SoraEventHandler* handler);
		
		
		/*
		 register file change event handler
		 */
		void registerFileChangeEventHandler(const SoraWString& file, SoraEventHandler* handler);
		void unregisterFileChangeEventHandler(SoraEventHandler* handler);
		void setFileChangeDetectionInterval(float32 interval);
		
		void update(float32 dt);

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
			
			void update(float32 dt);
			
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
		
		typedef std::multimap<int32/* priority */, SoraEventHandler*> INPUT_EVENT_HANDLER_LIST;
		INPUT_EVENT_HANDLER_LIST iehList;
		
		inline void freeTimerEvent(TIMER_EVENT_LIST::iterator ittev);
		
		SoraFileChangeEventPublisher* mFileChangeEventPublisher;
	};
	
	class SoraLuaEvent: public SoraEvent {
	public:
		SoraLuaEvent(const SoraString& _func, const SoraString& _param) {
			func = _func;
			param = _param;
		}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744073439210198ULL);
#endif
		
		std::string func;
		std::string param;
	};
	
#define SORA_EVENT_MANAGER SoraEventManager::Instance()
} // namespace sora


#endif