/*
 *  CoreCmds.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/6/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CORE_CMDS_H_
#define SORA_CORE_CMDS_H_

#include "SoraPlatform.h"
#include "SoraEvent.h"

namespace sora {
	
	class SoraConsoleEvent;
	class SoraCore;
	
	class SoraCoreCmdHandler: public SoraEventHandler, public SoraSingleton<SoraCoreCmdHandler> {
	protected:
		friend class SoraSingleton<SoraCoreCmdHandler>;
		SoraCoreCmdHandler();
		
	public:
		sora::SoraCore* sora;
		
		void onCmdEvent(SoraConsoleEvent* cev);
	};
	
} // namespace sora

#endif // SORA_CORE_CMDS_H_