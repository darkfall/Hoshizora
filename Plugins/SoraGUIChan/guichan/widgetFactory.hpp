//
//  widgetFactory.hpp
//  Sora
//
//  Created by Ruiwei Bu on 8/26/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef gcn_widgetFactory_hpp
#define gcn_widgetFactory_hpp

#include "guichan/factory.hpp"
#include "guichan/widget.hpp"

namespace gcn {
    
    /**
     * Widget factory typedef
     **/
    
    typedef AbstractFactory<Widget> WidgetFactory;
    
    /**
     * Helper function to register a widget
     *
     * @since Guichan for Hoshizora
     **/
    template<typename T>
    static void RegisterWidget(const std::string& name) {
        WidgetFactory::Instance().reg<T>(name);
    }
    
    static Widget* CreateWidget(const std::string& name) {
        return WidgetFactory::Instance().create(name);
    }
    
    /**
     * Register core widgets
     * Would be called automatically by gcn::Gui
     **/
    void RegisterCoreWidgets();
    
} // namespace gcn



#endif
