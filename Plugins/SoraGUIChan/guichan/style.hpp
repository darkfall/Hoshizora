//
//  style.h
//  Sora
//
//  Created by Ruiwei Bu on 8/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_style_h
#define Sora_style_h

#include "guichan/color.hpp"
#include "guichan/platform.hpp"

#include <string>
#include <map>

namespace gcn {

    /**
     * The base class contains the style configuration of the gui widgets
     * Such as base color, foreground color etc
     * It can also be configured to contain image paths of widget elements
     * for custom widgets
     *
     * @author Robert Bu(darkfall)
     * @since GuiChan for Hoshizora
     */
    
    /**
     * Struct for containing style strings
     **/
    struct GCN_CORE_DECLSPEC StyleStringContainer {
       
        typedef std::map<std::string, std::string> StyleStringMap;

        /**
         * Get a style string with key
         **/
        std::string getStyleString(const std::string& key) const;
        
        /**
         * Set a style string
         **/
        void setStyleString(const std::string& key, const std::string& val);
                
    private:
        StyleStringMap mStyleStrings;
    };
    
    class GCN_CORE_DECLSPEC Style {
    public:
        Style();
        
        /**
         * Get the default global style for all widgets
         * Equals to StyleHolder::Instance().getStyle("default");
         **/
        static Style* DefaultStyle();
        
        /**
         * Sets the global background color of all widgets
         *
         * @param The background color
         */
        void setBackgroundColor(const Color& col);
        
        /**
         * Sets the global foreground color of all widgets
         *
         * @param The foreground color
         */
        void setForegroundColor(const Color& col);
        
        /**
         * Sets the global base color of all widgets
         *
         * @param The base color
         */
        void setBaseColor(const Color& col);
        
        /**
         * Sets the global selection color of all widgets
         *
         * @param The selection color
         */
        void setSelectionColor(const Color& col);
        
        /**
         * Gets the global background color of all widgets
         * 
         */
        Color getBackgroundColor();
        
        /**
         * Gets the global foreground color of all widgets
         */
        Color getForegroundColor();
		
        /**
         * Gets the global base color of all widgets
         */
        Color getBaseColor();
		
        /**
         * Gets the global selection color of all widgets
         */
        Color getSelectionColor();
        
        /**
         * Get a style string with key
         * This is useful for configuring widget appearances
         * @Param the key of the style string
         **/
        std::string getStyleString(const std::string& key) const;
        
        /**
         * Set a style string with key
         * This is useful for configuring widget appearances
         * For example, in your gui configuration, you can set button_surface to button_surface.png
         * Then in you button widget, you can get the button_surface value and load the image
         * @Param the key of the style string
         * @Param the style string value
         **/
        void setStyleString(const std::string& key, const std::string& val);
        
        /**
         * Get a style color with key
         **/
        gcn::Color getStyleColor(const std::string& key) const;
        
        /**
         * Set a style color with given key
         **/
        void setStyleColor(const std::string& key, const gcn::Color& col);
        
    private:
        /**
         * Holds the global foreground color
         */
        Color mGlobalForegroundColor;
        /**
         * Holds the global background color
         */
        Color mGlobalBackgroundColor;
		/**
         * Holds the global base color
         */
        Color mGlobalBaseColor;
        /**
         * Holds the global selection color
         */
        Color mGlobalSelectionColor;
        
        StyleStringContainer mStyleStrings;
        
        typedef std::map<std::string, gcn::Color> StyleColorMap;
        StyleColorMap mStyleColors;
    };
    
} // namespace gcn


#endif
