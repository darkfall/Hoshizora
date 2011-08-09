//
//  birthListener.h
//  Sora
//
//  Created by Ruiwei Bu on 8/9/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_birthListener_h
#define Sora_birthListener_h

#include <string>

#include "guichan/event.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
    /**
     * Interface for listening for birth events from widgets.
     *
     * @see Widget::addBirthListener, Widget::removeBirthListener
     * @author Robert Bu(darkfall)
     * @since Guichan for Hoshizora
     */
    class GCN_CORE_DECLSPEC BirthListener
    {
    public:
        
        /**
         * Destructor.
         */
        virtual ~BirthListener() { }
        
        /**
         * Called when a widget births(added to a container). It is used to be able to recieve
         * a notification when a death of a widget occurs.
         *
         * @param event The event of the death.
         */
        virtual void birth(const Event& event) = 0;
        
    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of DeathListener,
         * therefore its constructor is protected.
         */
        BirthListener() { }
        
    };
}

#endif
