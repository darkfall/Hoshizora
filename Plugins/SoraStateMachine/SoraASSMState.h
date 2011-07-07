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

#include "SoraASSMAction.h"

#include <list>

namespace sora {
	
	/*
	 ASSM (A Simple State Machine) State implemention
	 */

	template<typename entity_type>
	class SoraASSMState: public SoraNamedObject {
	public:
        typedef SoraASSMState<entity_type>* PASSM_STATE;
				
		virtual void onEnter() = 0;
		virtual void onExecute() = 0;
		virtual void onLeave() = 0;
			
		// actions in action list would be run in sequence
		void addAction(SoraASSMAction<entity_type>* action) { 
			actionList.addObject(action);
			action->reset();
		}
        
		void removeAction(SoraASSMAction<entity_type>* action) { 
            actionList.delObject(action);
        }
        
        bool runActionList() {
            SoraNamedObjectList actions = getActionList();
            size_t finishCount = 0;
            SoraNamedObjectList::Iterator itAction = actions.begin();
            while(itAction != actions.end()) {
                SoraASSMAction<entity_type>* pAction = (SoraASSMAction<entity_type>*)(*itAction);
                pAction->setOwner(mOwner);
                pAction->run();
                if(pAction->isFinished())
                    ++finishCount;
            }
            if(finishCount == actions.count()) {
                return true;
            }
            return false;
        }
		
		SoraNamedObjectList& getActionList() { 
            return actionList;
        }
		
        void setOwner(entity_type* owner) {
            mOwner = owner;
        }
        entity_type* getOwner() const {
            return mOwner;
        }
        
	private:
		SoraNamedObjectList actionList;
        
        entity_type* mOwner;
	};
	
} // namespace sora



#endif // FSM_STATE_H_