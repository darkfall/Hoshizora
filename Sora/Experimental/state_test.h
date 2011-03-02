/*
 *  state_test.h
 *  Sora
 *
 *  Created by Griffin Bu on 2/7/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef STATE_TEST_H_
#define STATE_TEST_H_

#include "SoraASSM_State.h"
#include "SoraASSM_SM.h"
#include "SoraObject.h"

namespace sora {
	
	class BaseEntity;
	
	class EntityState_Combat_Pursuit: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Pursuit() {}
		
	public:
		static EntityState_Combat_Pursuit* Instance() {
			static EntityState_Combat_Pursuit* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Pursuit;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner) {}
		void onEnter(BaseEntity* pOwner) { printf("**** pursuit enter\n"); }
		void onLeave(BaseEntity* pOwner) {printf("**** pursuit leave\n");}
	};
	
	class EntityState_Combat_Evade: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Evade() {}
		
	public:
		static EntityState_Combat_Evade* Instance() {
			static EntityState_Combat_Evade* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Evade;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner) {}
		void onEnter(BaseEntity* pOwner) { printf("**** evade enter\n");  }
		void onLeave(BaseEntity* pOwner) { printf("**** evade leave\n");}
	};
	
	class EntityState_Combat_Attack: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Attack() {}
		
	public:
		static EntityState_Combat_Attack* Instance() {
			static EntityState_Combat_Attack* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Attack;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner) {}
		void onEnter(BaseEntity* pOwner) { printf("**** attack enter\n");  }
		void onLeave(BaseEntity* pOwner) {printf("**** attack leave\n");}
	};
	
	class BaseEntity: public SoraObject {
	public:
		BaseEntity() {
			stateMachine = new SoraASSM_SM<BaseEntity>(this, EntityState_Combat_Evade::Instance());
			
		//	stateMachine->defTrans(EntityState_Combat_Evade::Instance(), EntityState_Combat_Pursuit::Instance(), &lifeCompCond1, str2id("LifeLessThan100"))
		//				 .defTrans(EntityState_Combat_Pursuit::Instance(), EntityState_Combat_Attack::Instance(), &lifeCompCond2, str2id("LifeLessThan50"))
		//			     .defTrans(EntityState_Combat_Attack::Instance(), EntityState_Combat_Evade::Instance(), &lifeCompCond3, str2id("LifeEqualTo0"));
		//	stateMachine->delTrans(EntityState_Combat_Evade::Instance(), str2id("LifeLessThan100"));
		}
		
		uint32 update(float32 dt) {
			stateMachine->update();
			
			return 0;
		}
								   
		int32 getLife() const { return iLife; }
		void setLife(int32 life) { iLife = life; }

	private:
		SoraASSM_SM<BaseEntity>* stateMachine;
//		lifeComp1 lifeCompCond1;
//		lifeComp2 lifeCompCond2;
//		lifeComp3 lifeCompCond3;
		
		int32 iLife;
	};
		
/*
	class BaseEntity;
	
	class EntityState_Combat_Pursuit: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Pursuit() {}
		
	public:
		static EntityState_Combat_Pursuit* Instance() {
			static EntityState_Combat_Pursuit* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Pursuit;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Combat_Evade: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Evade() {}
		
	public:
		static EntityState_Combat_Evade* Instance() {
			static EntityState_Combat_Evade* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Evade;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Combat_Attack: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_Attack() {}
		
	public:
		static EntityState_Combat_Attack* Instance() {
			static EntityState_Combat_Attack* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_Attack;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Combat_CastMagic: public SoraASSM_State<BaseEntity> {
		EntityState_Combat_CastMagic() {}
		
	public:
		static EntityState_Combat_CastMagic* Instance() {
			static EntityState_Combat_CastMagic* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Combat_CastMagic;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Combat: public SoraASSM_HSM_State<BaseEntity> {
	public:
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner);
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Normal_Stand: public SoraASSM_State<BaseEntity> {
		EntityState_Normal_Stand() {}
		
	public:
		static EntityState_Normal_Stand* Instance() {
			static EntityState_Normal_Stand* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Normal_Stand;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Normal_Partrol: public SoraASSM_State<BaseEntity> {
		EntityState_Normal_Partrol() {}
		
	public:
		static EntityState_Normal_Partrol* Instance() {
			static EntityState_Normal_Partrol* pInstance = 0;
			if(!pInstance) pInstance = new EntityState_Normal_Partrol;
			return pInstance;
		};
		
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner) {}
		void onLeave(BaseEntity* pOwner) {}
	};
	
	class EntityState_Normal: public SoraASSM_HSM_State<BaseEntity> {
	public:
		void onExecute(BaseEntity* pOwner);
		void onEnter(BaseEntity* pOwner);
		void onLeave(BaseEntity* pOwner) {}
	};
	
	enum WeaponType {
		WEAPON_SWORD = 1,
		WEAPON_AXE = 2,
		WEAPON_STAFF = 4,
		WEAPON_233 = 8,
	};
	
	class BaseAttackEvent: public SoraEvent {
	public:
		BaseAttackEvent(int32 damage, WeaponType type, BaseEntity* who): iDamage(damage), iType(type), pWho(who) {}
		
		int32 getDamage() const { return iDamage; }
		WeaponType getType() const { return iType; }
		
		BaseEntity* getAttacker() const { return pWho; }
		
	private:
		int32 iDamage;
		WeaponType iType;
		BaseEntity* pWho;
	};
	
	class BaseWeapon {
	public:
		BaseWeapon(int32 damage, WeaponType type): iDamage(damage), iType(type) {}
		
		int32 getDamage() { return iDamage; }
		WeaponType getWeaponType() { return iType; }
		
		BaseEntity* getOwner() const { return pOwner; }
		void setOwner(BaseEntity* owner) { pOwner = owner; }
		
	private:
		int32 iDamage;
		WeaponType iType;
		
		BaseEntity* pOwner;
	};
	
	enum BaseEntityStates {
		STATE_COMBAT = 1,
		STATE_NORMAL = 2,
	};
	
	class BaseEntity: public SoraObject {
	public:
		BaseEntity(uint32 _life): iLife(_life), pCurrentEnemy(NULL), pWeapon(NULL), bDie(false) {
			registerEventFunc(this, &BaseEntity::onBeingAttacked);
			pEntityStateMachine = new SoraASSM_SM<BaseEntity>;
		}
		
		uint32 update(float32 dt) {
			pEntityStateMachine->update(this);
			return 0;
		}
		
		void setLife(int32 _life) { iLife = _life; }
		int32 getLife() const { return iLife; }
		
		void onBeingAttacked(const BaseAttackEvent* ev) {
			iLife -= ev->getDamage();
			if(iLife <= 0) {
				bDie = true;
				printf("*** %s died\n", getName().c_str(), ev->getDamage());
			}
			else {
				printf("%s: aaa!! life-%d\n", getName().c_str(), ev->getDamage());
			}

		}
		
		void attack(BaseEntity* who) {
			assert( who != NULL);
			
			if(pWeapon) {
				BaseAttackEvent attEv(pWeapon->getDamage(), pWeapon->getWeaponType(), this);
				who->handleEvent(&attEv);
			}
		}
		
		SoraString getName() const { return name; }
		void setName(const SoraString& n) { name = n; }
		
		void pickUpWeapon(BaseWeapon* weapon) {
			pWeapon = weapon;
			pWeapon->setOwner(this);
			
			printf("%s: I PICKED UP A WEAPON!!!\n		WeaponType=%d\n		WeaponDamage=%d\n", getName().c_str(), weapon->getWeaponType(), weapon->getDamage());
		}
		
		void dropWeapon() {
			pWeapon->setOwner(NULL);
			pWeapon = NULL;
		}
		bool isHaveWeapon() {
			return pWeapon != NULL;
		}
		
		bool isDied() { return bDie; }
		
		void changeState(BaseEntityStates statetype) {
			switch (statetype) {
				case STATE_COMBAT: pEntityStateMachine->changeState(&combatState, this); break;
				case STATE_NORMAL: pEntityStateMachine->changeState(&normalState, this); break;
			}
		}
		
		void setEnemy(BaseEntity* pEnemy) { pCurrentEnemy = pEnemy; }
		BaseEntity* getEnemy() const { return pCurrentEnemy; }
		
	private:
		bool bDie;
		int32 iLife;
		SoraString name;
					
		BaseEntity* pCurrentEnemy;
		BaseWeapon* pWeapon;
		
		EntityState_Combat combatState;
		EntityState_Normal normalState;
		
		SoraASSM_SM<BaseEntity>* pEntityStateMachine;
	};
	*/
	
} // namespace sora

#endif // STATE_TEST_H_