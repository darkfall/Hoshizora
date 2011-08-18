#include "SoraEvent.h"
#include "SoraSystemEvents.h"
#include "SoraPreDeclare.h"

#include "event/SoraEventWorld.h"

namespace sora {

    SoraEventHandler::~SoraEventHandler() {
        Handlers::iterator it = _handlers.begin();
        while(it != _handlers.end()) {
            delete it->second;
            ++it;
        }
        _handlers.clear();
        
        if(this->isInWorld())
            mWorld->leave(this);
    }
    
    void SoraEventHandler::handleEvent(SoraEvent* event) {
        sora_assert(event);
#ifdef SORA_USE_RTTI
        Handlers::iterator it = _handlers.find(SoraTypeInfo(typeid(*event)));
#else
        Handlers::iterator it = _handlers.find(SoraTypeInfo(event->getEventIdentifier()));
#endif
        if(it != _handlers.end()) {
            it->second->exec(event);
        }
    }
    
    void SoraEventHandler::handleSystemEvent(SoraSystemEvent* evt) {
        if(mEnableUpdate && evt->getType() == SYS_EVT_UPDATE) {
            update(evt->getDelta());
            if(mUpdateReceiveEvent)
                handleEvent(evt);
        }
    }
    
    void SoraEventHandler::handleWorldEvent(SoraEvent* evt) {
        if(isEnabled()) {
            handleEvent(evt);
        }
    }
    
    SoraEventHandler::SoraEventHandler():
    mEnabled(false),
    mWorld(NULL),
    mChannel(),
    mEnableUpdate(false),
    mUpdateReceiveEvent(false) {
        mChannel.fill();
    }
                
    bool SoraEventHandler::isEnabled() {
        return mEnabled;
    }
    
    bool SoraEventHandler::isEnableUpdate() const {
        return mEnableUpdate;
    }
    
    void SoraEventHandler::enableUpate(bool receiveEvent) {
        mUpdateReceiveEvent = receiveEvent;
        mEnableUpdate = true;
    }
    
    void SoraEventHandler::disableUpdate(void) {
        mEnableUpdate = false;
    }
    
    void SoraEventHandler::enable(SoraEventWorld* world) {
        mWorld = world;
        mEnabled = true;
        
        onEnable();
    }
    
    void SoraEventHandler::disable() {
        mEnabled = false;
        onDisable();
        mWorld = NULL;
    }
    
    bool SoraEventHandler::isInWorld(SoraEventWorld* world) const {
        if(mWorld) {
            return mWorld == world;
        }
        
        return false;
    }

    bool SoraEventHandler::isInWorld() const {
        return mWorld != NULL;
    }
    
    bool SoraEventHandler::listenning(const SoraEventChannel& channel) {
        return mChannel.listenning(channel);
    }
    
    void SoraEventHandler::setChannel(const SoraEventChannel& channel) {
        mChannel = channel;
    }
    
    const SoraEventChannel SoraEventHandler::getChannel() const {
        return mChannel;
    }
    
    void SoraEventHandler::addChannel(const SoraEventChannel& channel) {
        mChannel.add(channel);
    }
    
    void SoraEventHandler::removeChannel(const SoraEventChannel& channel) {
        mChannel.remove(channel);
    }
    
    void SoraEventHandler::clearChannel() {
        mChannel.clear();
    }
    
    SoraEventWorld* SoraEventHandler::getWorld() {
        return mWorld;
    }
    
    void SoraEventHandler::onEnable() {
        
    }
    
    void SoraEventHandler::onDisable() {
        
    }
    
    uint32 SoraEventHandler::update(float32 dt) {
        return 0;
    }
    
} // namespace sora