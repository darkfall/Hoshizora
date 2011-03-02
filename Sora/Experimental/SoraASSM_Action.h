/*
 *  SoraASSM_Action
 *  Sora
 *
 *  Created by Griffin Bu on 2/17/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_ASSM_ACTION_H_
#define _SORA_ASSM_ACTION_H_

#include "SoraPlatform.h"
#include "SoraNamedObject.h"

namespace sora {
	
	class SoraASSM_Action: public SoraNamedObject {
	public:
		// initialize
		virtual void reset() = 0;
		
		// on action starts to run
		virtual void onEnter() = 0;
		
		
		virtual void onUpdate() {
			finish();
		}
		
		void finish() { bFinished = true; }
		bool isFinished() { return bFinished; }
		
		virtual SoraString onErrorCheck() {
			return "";
		}
		
		inline void run() {
			if(!bFinished) {
				if(!bEntered) {
					onEnter();
					bEntered = true;
				} else {
					onUpdate();
				}
			}
		}
	private:
		bool bFinished;
		bool bEntered;
	};
	
} // namespace sora

#endif