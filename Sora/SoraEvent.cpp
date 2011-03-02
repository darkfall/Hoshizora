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

void SoraEventHandler::handleEvent(const SoraEvent* event) {
	Handlers::iterator it = _handlers.find(SoraTypeInfo(typeid(*event)));
	if(it != _handlers.end()) {
		it->second->exec(event);
	}
}

} // namespace sora