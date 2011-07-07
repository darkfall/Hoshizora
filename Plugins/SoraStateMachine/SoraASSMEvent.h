/*
 *  SoraASSM_Event.h
 *  Sora
 *
 *  Created by Griffin Bu on 2/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_ASSM_EVENT_H_
#define _SORA_ASSM_EVENT_H_

#include "SoraEvent.h"
#include "SoraNamedObject.h"

namespace sora {
	
	template<typename entity_type>
	class SoraASSMState;
	
	template<typename entity_type>
	class SoraASSMEvent: public SoraEvent {
	public:
		SoraASSMEvent(const SoraString& name) { setName(str2id(name)); }
		virtual ~SoraASSMEvent() {}
		
		void linkToState(SoraASSMState<entity_type>* state) { 
			assert(state != NULL);
			pLinkedState = state;
		}
		
		SoraASSMState<entity_type>* getLinkedState() const { 
            return pLinkedState;
        }
		
	private:
		SoraASSMState<entity_type>* pLinkedState;
	};
	
} // namespace sora


#endif // _SORA_ASSM_EVENT_H_