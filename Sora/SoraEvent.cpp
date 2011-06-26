#include "SoraEvent.h"

namespace sora {

SoraEventHandler::~SoraEventHandler() {
	Handlers::iterator it = _handlers.begin();
	while(it != _handlers.end()) {
		delete it->second;
		++it;
	}
	_handlers.clear();
}

void SoraEventHandler::handleEvent(SoraEvent* event) {
#ifdef SORA_USE_RTTI
	Handlers::iterator it = _handlers.find(SoraTypeInfo(typeid(*event)));
#else
	Handlers::iterator it = _handlers.find(SoraTypeInfo(event->getEventIdentifier()));
#endif
	if(it != _handlers.end()) {
		it->second->exec(event);
	}
}

} // namespace sora