#ifndef SORA_EVENT_H
#define SORA_EVENT_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include <map>
#include "SoraTypeInfo.h"
#include "stringId.h"

namespace sora {

    class SoraEventHandler;
	
#define SORA_EVENT_IDENTIFIER(ident) \
	virtual sora::stringId getEventIdentifier() const { \
		return ident; \
	} \
    
	class SORA_API SoraEvent {
	protected:
		virtual ~SoraEvent() {};
        
    public:
        SoraEvent(): pEventSource(NULL), bConsumed(false) {
#ifndef SORA_USE_RTTI
			eventIdentifier = 0;
#endif
		}

        void setSource(SoraEventHandler* source) { pEventSource = source; }
        SoraEventHandler* getSource() const { return pEventSource; }
		
		void setName(stringId _name) { name = _name; }
		stringId getName() const { return name; }
		
		// consume a event
		void consume() { bConsumed = true; }
		bool isConsumed() const { return bConsumed; }
		
#ifndef SORA_USE_RTTI
		virtual stringId getEventIdentifier() const = 0;
#endif
		
		// base event not serializable
		virtual bool serializable() { return false; }
		
    protected:
		bool bConsumed;
		
		stringId name;
        SoraEventHandler* pEventSource;
		
#ifndef SORA_USE_RTTI
		stringId eventIdentifier;
#endif
	};

	class SORA_API SoraHandlerFunctionBase {
	public:
		virtual ~SoraHandlerFunctionBase() {};
		void exec(SoraEvent* event) { call(event);}

	private:
		virtual void call(SoraEvent*) = 0;
	};

	template <class T, class EventT>
	class SoraMemberFunctionHandler : public SoraHandlerFunctionBase {
	public:
		typedef void (T::*MemberFunc)(EventT*);
		SoraMemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

		void call(SoraEvent* event) {
			(_instance->*_function)(static_cast<EventT*>(event));
		}

	private:
		T* _instance;
		MemberFunc _function;
	};
	
	template<typename EventT>
	class SoraFuncFunctionHandler: public SoraHandlerFunctionBase  {
	public:
		typedef void (*EventFunc)(EventT* event);
		 
		SoraFuncFunctionHandler(EventFunc func): _func(func) {}
		
		void call(SoraEvent* ev) {
			_func(static_cast<EventT*>(ev));
		}
		
	private:
		EventFunc _func;
	};

	class SORA_API SoraEventHandler {
	public:
		~SoraEventHandler();
		void handleEvent(SoraEvent*);

		template <class T, class EventT>
		void registerEventFunc(T*, void (T::*memFn)(EventT*));
		
		template <class EventT>
		void registerEventFunc(void (*evFn)(EventT*));
		
		// base eventHandler not serializable
		virtual bool serializable() { return false; }
	
	private:
		typedef std::map<SoraTypeInfo, SoraHandlerFunctionBase*> Handlers;
		Handlers _handlers;
	};


	template <class T, class EventT>
	void SoraEventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*)) {	
#ifdef SORA_USE_RTTI
		_handlers[SoraTypeInfo(typeid(EventT))]= new SoraMemberFunctionHandler<T, EventT>(obj, memFn);
#else
		EventT tmp;
		_handlers[SoraTypeInfo(tmp.getEventIdentifier())] = new SoraMemberFunctionHandler<T, EventT>(obj, memFn);
#endif
	}
	
	template <class EventT>
	void SoraEventHandler::registerEventFunc(void (*evFn)(EventT*)) {
#ifdef SORA_USE_RTTI
		_handlers[SoraTypeInfo(typeid(EventT))] = new SoraFuncFunctionHandler<EventT>(evFn);
#else
		EventT tmp;
		_handlers[SoraTypeInfo(tmp.getEventIdentifier())] = new SoraFuncFunctionHandler<EventT>(evFn);
#endif
	}

} // namespace sora

#endif