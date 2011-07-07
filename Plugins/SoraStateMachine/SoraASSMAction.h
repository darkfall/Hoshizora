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
	
    template<typename entity_type>
	class SoraASSMAction: public SoraNamedObject {
	public:
		// initialize
		virtual void reset() {}
		
		// on action starts to run
		virtual void onEnter() {}
		
		
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
        
        void setOwner(entity_type* owner) {
            mOwner = owner;
        }
        entity_type* getOwner() const {
            return mOwner;
        }
        
	private:
		bool bFinished;
		bool bEntered;
        
        entity_type* mOwner;
    };
	
} // namespace sora

#endif