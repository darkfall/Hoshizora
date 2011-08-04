/*
 *  SoraBooter.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraBooter.h"

#include "SoraDllHelper.h"

#include "../SoraCore.h"

#include "../SoraRenderSystem.h"
#include "../SoraMiscTool.h"
#include "../SoraTimer.h"
#include "../SoraInput.h"
#include "../SoraFontManager.h"
#include "../SoraSoundSystem.h"
#include "../SoraResourceManager.h"
#include "../SoraPluginManager.h"

namespace sora {
	
	typedef SoraRenderSystem* (*create_render_system)(void);
	typedef SoraInput* (*create_input)(void);
	typedef SoraMiscTool* (*create_misctool)(void);
	typedef SoraTimer* (*create_timer)(void);
	typedef SoraSoundSystem* (*create_sound_system)(void);
	typedef SoraResourceManager* (*create_resource_manager)(void);
	typedef SoraPluginManager* (*create_plugin_manager)(void);
	typedef SoraFontManager* (*create_font_manager)(void);

	void SoraBooter::loadExPlugins(const SoraWString& path) {
		std::vector<SoraWString> plugins;
		SORA->enumFilesInFolder(plugins, path);
		if(plugins.size() != 0) {
			for(size_t i=0; i<plugins.size(); ++i) {
				
				SoraDynLibrary dll;
                				
				if(dll.open(sora::ws2sfast(plugins[i]).c_str())) {
					create_render_system fnrs = (create_render_system)dll.getProc("create_render_system");
					if(fnrs != NULL) {
						SoraRenderSystem* rs = fnrs();
						if(rs != NULL)
							SORA->registerRenderSystem(rs);
					}
					
					create_input fni = (create_input)dll.getProc("create_input");
					if(fni) {
						SoraInput* input = fni();
						if(input != NULL)
							SORA->registerInput(input);
					}
					
					create_misctool fnmt = (create_misctool)dll.getProc("create_misctool");
					if(fnmt != NULL) {
						SoraMiscTool* ms = fnmt();
						if(ms != NULL)
							SORA->registerMiscTool(ms);
					}
					
					create_timer fnt = (create_timer)dll.getProc("create_timer");
					if(fnt != NULL) {
						SoraTimer* timer = fnt();
						if(timer != NULL)
							SORA->registerTimer(timer);
					}
					
					create_sound_system fnss = (create_sound_system)dll.getProc("create_sound_system");
					if(fnss != NULL) {
						SoraSoundSystem* ss = fnss();
						if(ss != NULL)
							SORA->registerSoundSystem(ss);
					}
					
					create_resource_manager fnrm = (create_resource_manager)dll.getProc("create_resource_manager");
					if(fnrm != NULL) {
						SoraResourceManager* rm = fnrm();
						if(rm != NULL)
							SORA->registerResourceManager(rm);
					}
					
					create_plugin_manager fnpm = (create_plugin_manager)dll.getProc("create_plugin_manager");
					if(fnpm != NULL) {
						SoraPluginManager* pm = fnpm();
						if(pm != NULL)
							SORA->registerPluginManager(pm);
					}
					
					create_font_manager fnfm = (create_font_manager)dll.getProc("create_font_manager");
					if(fnfm != NULL) {
						SoraFontManager* fm = fnfm();
						if(fm != NULL)
							SORA->registerFontManager(fm);
					}
					
				}
				
			}
		}
	}

} // namespace sora