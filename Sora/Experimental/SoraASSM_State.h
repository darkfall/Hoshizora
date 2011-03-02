/*
 *  FSM_State.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef FSM_STATE_H_
#define FSM_STATE_H_

#include "SoraEvent.h"
#include "SoraNamedObject.h"

#include "stringId.h"

#include "SoraASSM_Action.h"

#include <list>

namespace sora {
	
	/*
	 ASSM (A Simple State Machine) State implemention
	 */

	template<typename entity_type>
	class SoraASSM_State: public SoraNamedObject {
	public:
		typedef SoraASSM_State<entity_type>* PASSM_STATE;
		
		virtual ~SoraASSM_State() {}
				
		virtual void onEnter(entity_type*) = 0;
		virtual void onExecute(entity_type*) = 0;
		virtual void onLeave(entity_type*) = 0;
			
		// actions in action list would be run in sequence
		void addAction(SoraASSM_Action* action) { 
			actionList.addObject(action);
			action->reset();
		}
		void removeAction(SoraASSM_Action* action) { actionList.delObject(action); }
		
		SoraNamedObjectList& getActionList() { return actionList; }
		
	private:
		SoraNamedObjectList actionList;
	};
	
} // namespace sora



#endif // FSM_STATE_H_