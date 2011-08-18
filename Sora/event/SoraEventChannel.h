//
//  SoraEventChannel.h
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEventChannel_h
#define Sora_SoraEventChannel_h

#include "SoraPlatform.h"

namespace sora {
    
    /**
     * Original EventSystem concept from Orz GE Framework
     * Modified for work with SoraEvent
     **/
    
    namespace detail {
        
        template<class T, int I = (sizeof(T)>>2)>
        struct ChannelLength {
            enum {
                Left = (I<<3),
                Right = (sizeof(T)-I)<<3,
                Length = Left + Right
            };
        };
        
        template<class T>
        struct ChannelLength<T, 0> {
            enum {
                Left = 8,
                Right = (sizeof(T)-1)<<3,
                Length = Left + Right
            };
        };
           
        template<int N>
        class UserChannel {
        public:
            enum {Val = UserChannel<N-1>::Val << 1};
        };
        
        template<>
        class UserChannel<0> {
            enum {Val = 1 << ChannelLength<EventChannelType>::Left};
        };
        
        template<int N>
        class SystemChannel {
        public:
            enum {Val = SystemChannel<N-1>::Val << 1};
        };
        
        template<>
        class SystemChannel<0> {
        public:
            enum {Val = 1};
        };
        
    }
    
    enum {
        SystemChannelLength = detail::ChannelLength<EventChannelType>::Left,
        UserChannelLength = detail::ChannelLength<EventChannelType>::Right,
        AllChannelLength = detail::ChannelLength<EventChannelType>::Length
    };

    class SORA_API SoraEventChannel {
    public:
        inline SoraEventChannel();
        inline SoraEventChannel(const SoraEventChannel& channel);
        inline ~SoraEventChannel();
        
        inline SoraEventChannel& operator=(const SoraEventChannel& channel);
        
        inline void swap(SoraEventChannel& rhs) throw(); // never throw
        
        template<int N>
        inline SoraEventChannel& addUserChannel();
        
        template<int N>
        inline SoraEventChannel& addSystemChannel();
        
        template<int N>
        inline SoraEventChannel& removeUserChannel();
        
        template<int N>
        inline SoraEventChannel& removeSystemChannel();
        
        inline bool listenning(const SoraEventChannel& channel) const;
        
        inline bool empty() const;
        inline SoraEventChannel& clear();
        
        inline void add(const SoraEventChannel& channel);
        inline void remove(const SoraEventChannel& channel);
        
        inline SoraEventChannel& fill();
       /* inline SoraEventChannel& fillUserChannel();
        inline SoraEventChannel& fillSystemChannel();
        inline SoraEventChannel& clearUserChannel();
        inline SoraEventChannel& clearSystemChannel();*/
        
        inline static SoraEventChannel create();
        
        inline SoraEventChannel& addUserChannel(uint32 i) {
            mChannel = getUserChannel(i);
            return (*this);
        }
        
        inline SoraEventChannel& removeUserChannel(uint32 i) {
            mChannel &= ~getUserChannel(i);
            return (*this);
        }
        
    private:
        inline uint32 getUserChannel(uint32 i) {
            return static_cast<uint32>(1)<<i;
        }
        
        inline void init(const SoraEventChannel& channel);
        inline void addChannel(EventChannelType channel);
        inline void removeChannel(EventChannelType channel);
        
        EventChannelType mChannel;
    };
    
    template<int N>
    SoraEventChannel& SoraEventChannel::addUserChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < UserChannelLength);
        
        addChannel(detail::UserChannel<N>::Val);
        return (*this);
    }

    template<int N>
    SoraEventChannel& SoraEventChannel::removeUserChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < UserChannelLength);
        
        removeChannel(detail::UserChannel<N>::Val);
        return (*this);
    }
    
    template<int N>
    SoraEventChannel& SoraEventChannel::addSystemChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < SystemChannelLength);
        
        addChannel(detail::SystemChannel<N>::Val);
        return (*this);
    }
    
    template<int N>
    SoraEventChannel& SoraEventChannel::removeSystemChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < SystemChannelLength);
        
        removeChannel(detail::SystemChannel<N>::Val);
        return (*this);
    }
    
    SoraEventChannel SoraEventChannel::create() {
        return SoraEventChannel();
    }
    
    bool SoraEventChannel::listenning(const SoraEventChannel& channel) const {
        return (mChannel & channel.mChannel) != 0;
    }
    
    bool SoraEventChannel::empty() const {
        return mChannel == 0;
    }
    
    SoraEventChannel& SoraEventChannel::clear() {
        mChannel = 0;
		return *this;
    }
    
    void SoraEventChannel::add(const SoraEventChannel& channel) {
        addChannel(channel.mChannel);
    }
    
    void SoraEventChannel::remove(const SoraEventChannel& channel) {
        removeChannel(channel.mChannel);
    }
    
    void SoraEventChannel::addChannel(const EventChannelType channel) {
        mChannel |= channel;
    }
    
    void SoraEventChannel::removeChannel(const EventChannelType channel) {
        mChannel &= ~channel;
    }
    
    SoraEventChannel::SoraEventChannel():
    mChannel(0) {
        
    }
    
    SoraEventChannel& SoraEventChannel::fill() {
        mChannel = ~(static_cast<EventChannelType>(0));
        return (*this);
    }
    
   /* SoraEventChannel& SoraEventChannel::fillUserChannel() {
        fill():
        return (*this);
    }
    
    SoraEventChannel& SoraEventChannel::fillSystemChannel() {
        fill():
        return (*this);
    }
    
    SoraEventChannel& SoraEventChannel::clearUserChannel() {
        clear()
        return (*this);
    }
    
    SoraEventChannel& SoraEventChannel::clearSystemChannel() {
        clear()
        return (*this);
    }*/
    
    SoraEventChannel::SoraEventChannel(const SoraEventChannel& channel) {
        init(channel);
    }
    
    SoraEventChannel::~SoraEventChannel() {
        
    }
    
    SoraEventChannel& SoraEventChannel::operator =(const SoraEventChannel& rhs) {
        init(rhs);
        return (*this);
    }
    
    void SoraEventChannel::init(const SoraEventChannel& channel) {
        mChannel = channel.mChannel;
    }
    
    void SoraEventChannel::swap(SoraEventChannel& channel) throw() {
        std::swap(mChannel, channel.mChannel);
    }
    
    template<int N>
    static SoraEventChannel GetSystemChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < SystemChannelLength);
        
        static SoraEventChannel channel = SoraEventChannel::create();
        channel.addSystemChannel<N>();
        return channel;
    }
    
    template<int N>
    static SoraEventChannel GetUserChannel() {
        sora_static_assert(N >= 0);
        sora_static_assert(N < UserChannelLength);
        
        static SoraEventChannel channel = SoraEventChannel::create();
        channel.addUserChannel<N>();
        return channel;
    }
}



#endif
