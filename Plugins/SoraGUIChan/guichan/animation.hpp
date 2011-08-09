//
//  animation.h
//  Sora
//
//  Created by Ruiwei Bu on 8/9/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_animation_h
#define Sora_animation_h

#include "guichan/platform.hpp"
#include "guichan/modifier.hpp"

namespace gcn {
    
    /**
     * The base class represents a kind of gui animation
     * Animation details are archived by subclasses
     * It a kind of modifier so it's safe to apply a animation 
     * to a widget by calling the addModifier function of the widget
     * 
     * @author Robert Bu(darkfall)
     * @since GuiChan for Hoshizora
     */
    
    class GCN_CORE_DECLSPEC Animation: public Modifier {
    public:
        Animation(int time);
        
        virtual ~Animation() {}
        
        /**
         * Inherited from Modifier
         * Would update the time info of the animation
         * Subclasses should call this during their update to get corretly time info
         * for example
         *
         *  void myUpdate(Widget* widget) {
         *      Animation::update(widget);
         *      float ratio = getCurrTimeRatio();
         *     
         *      // set the alpha of the widget
         *      widget->setAlpha(ratio * 255);
         *  }
         */
        virtual void update(Widget* widget);
        
        /**
         * Reset the animation
         * 
         * @param The new total time, default = 0, use the same total time as before
         */
        virtual void reset(int time=0);
        
        /**
         * Inherited from Modifier
         */
        virtual void setFinish(bool flag);
       
        /**
         * Get the total time of the animation
         */
        int getTotalTime() const;
        
        /**
         * Get the current time of the animation
         * Current time would increase by 1 every update loop
         */
        int getCurrTime() const;
        
        /**
         * Get the current time ratio of the aniamtion
         * Equals to getCurrTime() / getTotalTime()
         * Would return 1 if the animation had finished
         */
        float getCurrTimeRatio() const;
        
    protected:
        int mTotalTime;
        int mCurrTime;
    };
    
} // namespace gcn



#endif
