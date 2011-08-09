//
//  style.h
//  Sora
//
//  Created by Ruiwei Bu on 8/9/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_style_h
#define Sora_style_h

#include "guichan/color.hpp"
#include "guichan/platform.hpp"

namespace gcn {

    /**
     * The base class contains the style configuration of the gui widgets
     * Such as base color, foreground color etc
     * It can also be configured to contain image paths of widget elements
     * For custom widgets
     *
     * @author Robert Bu(darkfall)
     * @since GuiChan for Hoshizora
     */
    
    class GCN_CORE_DECLSPEC Style {
    public:
        /**
         * Sets the global background color of all widgets
         *
         * @param The background color
         */
		static void setGlobalBackgroundColor(const Color& col);
        
        /**
         * Sets the global foreground color of all widgets
         *
         * @param The foreground color
         */
		static void setGlobalForegroundColor(const Color& col);
        
        /**
         * Sets the global base color of all widgets
         *
         * @param The base color
         */
		static void setGlobalBaseColor(const Color& col);
        
        /**
         * Sets the global selection color of all widgets
         *
         * @param The selection color
         */
		static void setGlobalSelectionColor(const Color& col);
        
        /**
         * Gets the global background color of all widgets
         * 
         */
		static Color getGlobalBackgroundColor();
        
        /**
         * Gets the global foreground color of all widgets
         */
		static Color getGlobalForegroundColor();
		
        /**
         * Gets the global base color of all widgets
         */
        static Color getGlobalBaseColor();
		
        /**
         * Gets the global selection color of all widgets
         */
        static Color getGlobalSelectionColor();
        
    private:
        /**
         * Holds the global foreground color
         */
		static Color mGlobalForegroundColor;
        /**
         * Holds the global background color
         */
		static Color mGlobalBackgroundColor;
		/**
         * Holds the global base color
         */
        static Color mGlobalBaseColor;
        /**
         * Holds the global selection color
         */
		static Color mGlobalSelectionColor;
    };
    
} // namespace gcn


#endif
