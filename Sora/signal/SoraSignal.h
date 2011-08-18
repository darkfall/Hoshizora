//
//  SoraSignal.h
//  Sora
//
//  Created by Ruiwei Bu on 8/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSignal_h
#define Sora_SoraSignal_h

#include "../SoraPlatform.h"
#include "../SoraAny.h"
#include "../SoraAutoPtr.h"
#include "../SoraDelegate.h"

#include <map>

namespace sora {
    
    /**
     * A Simple Signal Implemention based on the thought of boost::signal
     * However we don't use boost library now, so
     * Callback based on SoraDelegate
     *
     * Users can connect to a signal to show interest on a specific event
     * Users can disconnect to the signal at anytime
     * When a specific event happened, all connected function would be notified
     **/
    
    template<typename RT, typename TARG>
    class SoraSignal;
    class SoraConnection;
    
    namespace signal {
        
        class SignalImpl {
        public:                        
            typedef std::multimap<int, SoraConnection> ConnectionMap;
            typedef std::multimap<int, SoraConnection>::iterator IteratorType;
            ConnectionMap mConnections;
            
            void disconnect(void* sig, void* data) {
                SignalImpl* psig = reinterpret_cast<SignalImpl*>(sig);
                
                std::auto_ptr<IteratorType> slot(reinterpret_cast<IteratorType*>(data));
                mConnections.erase(*slot);
            }
        };
        
        template<typename RT, typename TARG>
        class Slot {
        public:
            Slot(): fn(NULL) {
                
            }
            ~Slot() {
                if(fn) {
                    delete fn;
                }
            }
            bool isValid() {
                return fn != NULL;
            }
            
            RT call(SignalImpl* sender, TARG arg) {
                return fn->notify(sender, arg);
            }
            
            typedef sora::SoraAbstractDelegate<RT, TARG> FuncType;
            FuncType* fn;
        };
        
        class ConnectionBase {
        public:
            virtual bool isValid() = 0;
            virtual void disconnect() = 0;
            
            SignalImpl* signal;
            SignalImpl::IteratorType* iter;

            typedef void (SignalImpl::*DisconnectFunc)(void*, void*);
            DisconnectFunc signal_disconnet;
        };
        
        template<typename RT, typename TARG>
        class BasicConnection: public ConnectionBase {
        public:
            sora::SoraAutoPtr<Slot<RT, TARG> > slot;
            
            bool isValid() {
                return slot.get() && signal_disconnet;
            }
            
            void disconnect() {
                if(isValid()) {
                    (signal->*signal_disconnet)(signal, iter);
                    signal = NULL;
                    signal_disconnet = NULL;
                }
            }
            
            bool operator ==(const BasicConnection &rhs) {
                return (slot.get()->fn == rhs.slot.get()->fn && signal == rhs.signal);
            }
        };
        
    } // namespace signal
  

    class SORA_API SoraConnection {
    public:
        SoraConnection():
        isControlling(false) {
        }
        ~SoraConnection() {
            if(isControlling)
                disconnect();
        }
        bool isValid() {
            sora_assert(con.get());
            return con->isValid();
        }
        
        void disconnect() {
            sora_assert(con.get());
            con->disconnect();
        }
        
        bool operator ==(const SoraConnection &rhs) {
            sora_assert(con.get());
            return con.get() == rhs.con.get();
        }
        
        /**
         * Signal managed functions
         * You should not call these functions
         **/
        
        template<typename RT, typename TARG>
        signal::Slot<RT, TARG>* slot() {
            sora_assert(con.get());
            signal::BasicConnection<RT, TARG>* rcon = static_cast<signal::BasicConnection<RT, TARG>* >(con.get());
            return rcon->slot.get();
        }
        
        void setControl(bool flag) {
            isControlling = flag;
        }
        
        void reset(signal::ConnectionBase* _con) {
            con.reset(_con);
        }
        
    private:
        bool isControlling;
        SoraAutoPtr<signal::ConnectionBase> con;
    };
    
    template<typename RT, typename ARG>
    class SORA_API SoraSignal: public signal::SignalImpl {
    public:
        typedef SoraConnection ConnectionType; 
        
        ConnectionType connect(const typename signal::Slot<RT, ARG>::FuncType& del) {
            return connect(0, del);
        }
        
        ConnectionType connect(int32 priority, const typename signal::Slot<RT, ARG>::FuncType& del) {
            signal::BasicConnection<RT, ARG>* con = new signal::BasicConnection<RT, ARG>;
            SoraConnection slotcon;
            slotcon.reset(con);
            
            std::auto_ptr<IteratorType> saved_iter(new IteratorType());
            IteratorType iter = mConnections.insert(std::make_pair(priority, slotcon));
            *saved_iter = iter;
            
            // connection controller
            iter->second.setControl(true);
            
            con->slot.reset(new signal::Slot<RT, ARG>);
            con->slot->fn = del.clone();
            con->signal = this;
            con->signal_disconnet = &signal::SignalImpl::disconnect;
            con->iter = saved_iter.release();
            
            return slotcon;
        }
        
        RT sig(ARG arg) {
            typename ConnectionMap::iterator itConnection = mConnections.begin();
            
            RT rtVal;
            while(itConnection != mConnections.end()) {
                signal::Slot<RT, ARG>* slot = itConnection->second.slot<RT, ARG>();
                rtVal = slot->call(this, arg);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(ARG arg) {
            sig(arg);
        }
    };
    
} // namespace sora


#endif
