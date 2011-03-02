/*
 *  state_test.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 2/7/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "state_test.h"
#include "SoraMath.h"

#include "SoraCore.h"

namespace sora {

/*
	void EntityState_Combat_Pursuit::onExecute(BaseEntity* pOwner) {
		if(pOwner->getEnemy()) {
			BaseEntity* pEnemy = pOwner->getEnemy();
			
			printf("%s: pursuing %s!\n", pOwner->getName().c_str(), pEnemy->getName().c_str());
			
			if(getDistanceSqr(pOwner->getPositionX(), pOwner->getPositionY(),
							  pEnemy->getPositionX(), pEnemy->getPositionY()) < 10) {
				printf("%s: I'm nearing %s now!!\n", pOwner->getName().c_str(), pEnemy->getName().c_str());
				getParent()->changeState(EntityState_Combat_Attack::Instance(), pOwner);
			} else {
				pOwner->setPosition(pOwner->getPositionX()+(pEnemy->getPositionX()-pOwner->getPositionX())/10,
									pOwner->getPositionY()+(pEnemy->getPositionY()-pOwner->getPositionY())/10);
			}
		} else {
			pOwner->changeState(STATE_NORMAL);
		}
	}
	
	void EntityState_Combat_Evade::onExecute(BaseEntity* pOwner) {
		if(pOwner->getEnemy()) {
			BaseEntity* pEnemy = pOwner->getEnemy();
			
			printf("%s: aaaaaaaa!!!! help!!!! %s is going to kill me!!!!!\n", pOwner->getName().c_str(), pEnemy->getName().c_str());
			pOwner->setPosition(pOwner->getPositionX()+(-pEnemy->getPositionX()+pOwner->getPositionX())/20,
								pOwner->getPositionY()+(-pEnemy->getPositionY()+pOwner->getPositionY())/20);
	
			if(!pOwner->isHaveWeapon()) {
				switch(SORA->randomInt(0, 3)) {
					case 0: pOwner->pickUpWeapon(new BaseWeapon(10, WEAPON_SWORD)); break;
					case 1: pOwner->pickUpWeapon(new BaseWeapon(15, WEAPON_AXE)); break;
					case 2: pOwner->pickUpWeapon(new BaseWeapon(10, WEAPON_STAFF)); break;
					case 3: pOwner->pickUpWeapon(new BaseWeapon(20, WEAPON_233)); break;
				}
			}
		} else {
			printf("%s: hu.... I escaped!!!!\n", pOwner->getName().c_str());
			pOwner->changeState(STATE_NORMAL);
		}
	}
	
	void EntityState_Combat_Attack::onExecute(BaseEntity* pOwner) {
		if(pOwner->getEnemy()) {
			BaseEntity* pEnemy = pOwner->getEnemy();
			
		/*	if(getDistanceSqr(pOwner->getPositionX(), pOwner->getPositionY(),
							  pEnemy->getPositionX(), pEnemy->getPositionY()) < 10) {
				if(getParent()) getParent()->changeState(EntityState_Combat_Pursuit::Instance(), pOwner);
			}*/
/*
			if(pOwner->isHaveWeapon() && !pOwner->isDied())
				pOwner->attack(pEnemy);
			else {
				if(!pOwner->isHaveWeapon()) {
					printf("%s: I DONT HAVE A WEAPON!!!!\n", pOwner->getName().c_str());
					if(!pOwner->isHaveWeapon()) {
						switch(SORA->randomInt(0, 3)) {
							case 0: pOwner->pickUpWeapon(new BaseWeapon(10, WEAPON_SWORD)); break;
							case 1: pOwner->pickUpWeapon(new BaseWeapon(15, WEAPON_AXE)); break;
							case 2: pOwner->pickUpWeapon(new BaseWeapon(10, WEAPON_STAFF)); break;
							case 3: pOwner->pickUpWeapon(new BaseWeapon(20, WEAPON_233)); break;
						}
					}
				}
			}
			
			if(pEnemy->isDied()) {
				printf("%s: I killed %s!!!woooo~woooo!\n", pOwner->getName().c_str(), pEnemy->getName().c_str());
				pOwner->setEnemy(NULL);
				pOwner->changeState(STATE_NORMAL);
			}
		} else {
			pOwner->changeState(STATE_NORMAL);
		}
	}
	
	void EntityState_Combat_CastMagic::onExecute(BaseEntity* pOwner) {
	}
	
	void EntityState_Combat::onExecute(BaseEntity* pOwner) {
		getHSM()->update(pOwner);
		
		if(!pOwner->getEnemy()) {
			pOwner->changeState(STATE_NORMAL);
		}
	}
	
	void EntityState_Combat::onEnter(BaseEntity* pOwner) {
		if(pOwner->getEnemy() != NULL) {
			getHSM()->changeState(EntityState_Combat_Attack::Instance(), pOwner);
			
			printf("**** %s ENTERS COMBAT with %s!\n", pOwner->getName().c_str(), pOwner->getEnemy()->getName().c_str());
		} else {
			pOwner->changeState(STATE_NORMAL);
		}
	}

	void EntityState_Normal_Partrol::onExecute(BaseEntity* pOwner) {
		printf("%s: Today is a good day and I'm patrolling~~~~~\n", pOwner->getName().c_str());
		
		if(pOwner->getEnemy() != NULL)
			pOwner->changeState(STATE_COMBAT);
	}
	
	void EntityState_Normal_Stand::onExecute(BaseEntity* pOwner) {
	}
	
	void EntityState_Normal::onEnter(BaseEntity* pOwner) {
		printf("**** %s ENTERS PATROL!\n", pOwner->getName().c_str());

		getHSM()->changeState(EntityState_Normal_Partrol::Instance(), pOwner);
	}
	
	void EntityState_Normal::onExecute(BaseEntity* pOwner) {
		getHSM()->update(pOwner);
	}
	*/
			
} // namespace sora