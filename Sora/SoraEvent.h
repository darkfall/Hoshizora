#ifndef SORA_EVENT_H
#define SORA_EVENT_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include <map>
#include "SoraTypeInfo.h"
#include "stringId.h"

namespace sora {

    class SoraEventHandler;
    
	class SoraEvent {
	protected:
		virtual ~SoraEvent() {};
        
    public:
        SoraEvent(): pEventSource(NULL) {}

        void setSource(SoraEventHandler* source) { pEventSource = source; }
        SoraEventHandler* getSource() const { return pEventSource; }
		
		void setName(stringId _name) { name = _name; }
		stringId getName() const { return name; }
		
		// consume a event
		void consume() { bConsumed = true; }
		bool isConsumed() const { return bConsumed; }
		
		// base event not serializable
		virtual bool serializable() { return false; }
        
    private:
		bool bConsumed;
		
		stringId name;
        SoraEventHandler* pEventSource;
	};

	class SoraHandlerFunctionBase {
	public:
		virtual ~SoraHandlerFunctionBase() {};
		void exec(const SoraEvent* event) { call(event);}

	private:
		virtual void call(const SoraEvent*) = 0;
	};

	template <class T, class EventT>
	class SoraMemberFunctionHandler : public SoraHandlerFunctionBase {
	public:
		typedef void (T::*MemberFunc)(EventT*);
		SoraMemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

		void call(const SoraEvent* event) {
			(_instance->*_function)(static_cast<EventT*>(event));
		}

	private:
		T* _instance;
		MemberFunc _function;
	};

	class SoraEventHandler {
	public:
		~SoraEventHandler();
		void handleEvent(const SoraEvent*);

		template <class T, class EventT>
		void registerEventFunc(T*, void (T::*memFn)(EventT*));
		
		// base eventHandler not serializable
		virtual bool serializable() { return false; }
	
	private:
		typedef std::map<SoraTypeInfo, SoraHandlerFunctionBase*> Handlers;
		Handlers _handlers;
	};


	template <class T, class EventT>
	void SoraEventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*)) {	
		_handlers[SoraTypeInfo(typeid(EventT))]= new SoraMemberFunctionHandler<T, EventT>(obj, memFn);
	}

} // namespace sora

#endif