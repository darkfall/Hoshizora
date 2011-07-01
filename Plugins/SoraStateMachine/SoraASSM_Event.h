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
	class SoraASSM_State;
	
	template<typename entity_type>
	class SoraASSM_Event: public SoraEvent {
	public:
		SoraASSM_Event(stringId name) { setName(name); }
		virtual ~SoraASSM_Event() {}
		
		void linkToState(SoraASSM_State<entity_type>* state) { 
			assert(state != NULL);
			pLinkedState = state;
		}
		
		SoraASSM_State<entity_type>* getLinkedState() const { return pLinkedState; }
		
	private:
		SoraASSM_State<entity_type>* pLinkedState;
	};
	
} // namespace sora


#endif // _SORA_ASSM_EVENT_H_