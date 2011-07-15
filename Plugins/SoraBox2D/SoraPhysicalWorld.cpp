#include "SoraPhysicalWorld.h"
#include "SoraPlugin.h"
#include "SoraCore.h"

#ifdef SORA_PHYSICAL_THREAD
#include "SoraThread/SoraThread.h"
#include "SoraThread/SoraCountDownLatch.h"
#include "Debug/SoraAutoProfile.h"
#endif

#include "SoraPhysicalObject.h"

namespace sora {
    
    class SoraPhysicalWorldPlugin: public SoraFrameListener {
    public:
        SoraPhysicalWorldPlugin(SoraPhysicalWorld* world): mWorld(world) {
#ifdef SORA_PHYSICAL_THREAD            
            frameStarted = false;
            task.setAsMemberFunc(&SoraPhysicalWorldPlugin::threadFunc, this);
            
            thread.start(task);
#endif
        }
        ~SoraPhysicalWorldPlugin() {
#ifdef SORA_PHYSICAL_THREAD
    
#endif
        }
        
        void onFrameStart() {
#ifdef SORA_PHYSICAL_THREAD
            frameStarted = true;
            frameFinish = false;
#else
            mWorld->setFrameTime(SORA->getDelta());
            mWorld->stepWorld();
#endif
        }
        
        void onFrameEnd() {
#ifdef SORA_PHYSICAL_THREAD
            while(!frameFinish) {
                msleep(1);
            }
            
            SoraGlobalProfiler::Instance()->printProfile("PhysicalWorldThread");
#endif
        }
        
#ifdef SORA_PHYSICAL_THREAD
        void threadFunc(void* arg) {
            SoraMutexGuard lock(mutex);

            for(;;) {
                if(frameStarted) {
                    PROFILE("PhysicalWorldThread");
                        
                    if(mWorld) {
                        mWorld->setFrameTime(SORA->getDelta());
                        mWorld->stepWorld();
                    }
                    frameStarted = false;
                    frameFinish = true;
                } else {
                    msleep(1);
                }
            }
            
        }
#endif
        
    private:
        SoraPhysicalWorld* mWorld;
#ifdef SORA_PHYSICAL_THREAD
        SoraThread thread;
        SoraMutexLock mutex;
        SoraThreadTask task;
        
        bool frameStarted;
        bool frameFinish;
#endif
    };
    
    class SoraB2ContactListener: public b2ContactListener {
    public:
        
        void BeginContact(b2Contact* contact) {
            SoraPhysicalObject* fA = (SoraPhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
            SoraPhysicalObject* fB = (SoraPhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
            if(fA != NULL) {
                if(fA->getContactDelegate() != NULL) {
                    SoraPhysicalContactInfo info;
                    info.contactObject1 = fA;
                    info.contactObject2 = fB;
                    info.normal = contact->GetManifold()->localNormal;
                    info.position = contact->GetManifold()->localPoint;
                    info.isTouching = contact->IsTouching();
                    info.phase = CONTACT_BEGIN;
                    
                    fA->getContactDelegate()->notify(fA, info);
                }
            } 
            if(fB != NULL) {
                if(fB->getContactDelegate() != NULL) {
                    SoraPhysicalContactInfo info;
                    info.contactObject1 = fB;
                    info.contactObject2 = fA;
                    info.normal = contact->GetManifold()->localNormal;
                    info.position = contact->GetManifold()->localPoint;
                    info.isTouching = contact->IsTouching();
                    info.phase = CONTACT_BEGIN;
                    
                    fB->getContactDelegate()->notify(fB, info);
                }
            }
        }
        
        void EndContact(b2Contact* contact) {
            SoraPhysicalObject* fA = (SoraPhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
            SoraPhysicalObject* fB = (SoraPhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
            if(fA != NULL) {
                if(fA->getContactDelegate() != NULL) {
                    SoraPhysicalContactInfo info;
                    info.contactObject1 = fA;
                    info.contactObject2 = fB;
                    info.normal = contact->GetManifold()->localNormal;
                    info.position = contact->GetManifold()->localPoint;
                    info.isTouching = contact->IsTouching();
                    info.phase = CONTACT_END;
                    
                    fA->getContactDelegate()->notify(fA, info);
                }
            } 
            if(fB != NULL) {
                if(fB->getContactDelegate() != NULL) {
                    SoraPhysicalContactInfo info;
                    info.contactObject1 = fB;
                    info.contactObject2 = fA;
                    info.normal = contact->GetManifold()->localNormal;
                    info.position = contact->GetManifold()->localPoint;
                    info.isTouching = contact->IsTouching();
                    info.phase = CONTACT_END;
                    
                    fB->getContactDelegate()->notify(fB, info);
                }
            }
        }
    };
    
    static SoraPhysicalWorldPlugin* g_phy_plugin;
    
    SoraPhysicalWorld::SoraPhysicalWorld(): 
    velocityIteration(10), 
    positionIteration(8), 
    bInitialized(false), 
    bPluginInstalled(false), 
    coordScalar(100), 
    physicalWorld(NULL) {
        g_phy_plugin = new SoraPhysicalWorldPlugin(this);
        SORA->addFrameListener(g_phy_plugin);
    }
    
    SoraPhysicalWorld::~SoraPhysicalWorld() { 
        SORA->delFrameListener(g_phy_plugin);
        delete g_phy_plugin;
        g_phy_plugin = NULL;
    }

    void SoraPhysicalWorld::initBox2DWorld(float32 gravityx, float32 gravityy, bool doSleep) {
        if(physicalWorld && bInitialized) {
            delete physicalWorld;
        }
        physicalWorld = new b2World(b2Vec2(gravityx, gravityy), doSleep);
        if(!physicalWorld)
            THROW_SORA_EXCEPTION("Error creating box2d world");
        else {
            worldGravity = b2Vec2(gravityx, gravityy);
            bInitialized = true;
            
            SoraB2ContactListener* contactListener = new SoraB2ContactListener;
            physicalWorld->SetContactListener(contactListener);
        }
    }
    
    void SoraPhysicalWorld::stepWorld() {
        if(!bInitialized) return;
        
        physicalWorld->Step(dt, velocityIteration, positionIteration);
        physicalWorld->ClearForces();
    }
    
    void SoraPhysicalWorld::stepWorldEx(float32 dt, int32 vIteration, int32 pIteration) {
        if(!bInitialized) return;
        
        physicalWorld->Step(dt, vIteration, pIteration);
        physicalWorld->ClearForces();
    }
    
    b2Body* SoraPhysicalWorld::createBody(float32 px, float32 py, const b2Shape& box, bool isDynamic, float32 density, float friction) {
        if(!bInitialized) return NULL;
        
        b2BodyDef bodyDef;
        if(isDynamic) bodyDef.type = b2_dynamicBody;
        else bodyDef.type = b2_staticBody;
        
        bodyDef.position.Set(pixel2b2cor(px), pixel2b2cor(py));
        b2Body* body = physicalWorld->CreateBody(&bodyDef);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        
        body->CreateFixture(&fixtureDef);
        
        return body;
    }
    
    b2Body* SoraPhysicalWorld::getBodyList() {
        if(bInitialized)
            return physicalWorld->GetBodyList();
        return NULL;
    }
    
    b2Body* SoraPhysicalWorld::createBody(float32 px, float32 py, bool bDynamicBody) {
        if(!bInitialized) return NULL;
        
        b2BodyDef bodyDef;
        if(bDynamicBody) bodyDef.type = b2_dynamicBody;
        else bodyDef.type = b2_staticBody;
        
        bodyDef.position.Set(px, py);
        b2Body* body = physicalWorld->CreateBody(&bodyDef);
        return body;
    }
    
    b2Body* SoraPhysicalWorld::createBody(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef) {
        if(!bInitialized) return NULL;
        
        b2Body* body = physicalWorld->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        
        return body;
    }
    
    void SoraPhysicalWorld::createJoint(const b2JointDef& jdef) {
        if(!bInitialized) return;
        
        physicalWorld->CreateJoint(&jdef);
    }
    
    void SoraPhysicalWorld::destroyBody(b2Body* body) {
        if(!bInitialized) return;
        
        physicalWorld->DestroyBody(body);
    }
    
    void SoraPhysicalWorld::destroyJoint(b2Joint* joint) {
        if(!bInitialized) return;
        
        physicalWorld->DestroyJoint(joint);
    }
    
    int32 SoraPhysicalWorld::getBodyCount() const {
        if(!bInitialized) return 0;
        
        return physicalWorld->GetBodyCount();
        
    }
    
    int32 SoraPhysicalWorld::getJointCount() const {
        if(!bInitialized) return 0;
        
        return physicalWorld->GetJointCount();
    }
    
	// wrappers to help build shapes fast
    b2PolygonShape SoraPhysicalWorld::generateBox(float32 w, float32 h, float32 cx, float32 cy, float32 rot) {
		b2PolygonShape shape;
		shape.SetAsBox(pixel2b2cor(w), pixel2b2cor(h), b2Vec2(pixel2b2cor(cx), pixel2b2cor(cy)), 0.f);
		return shape;
	}
    
    b2CircleShape SoraPhysicalWorld::generateCircle(float32 r) {
		b2CircleShape shape;
		shape.m_radius = pixel2b2cor(r);
		return shape;
	}
    
    b2PolygonShape SoraPhysicalWorld::generatePolygen(b2Vec2* vertices, size_t count) {
		b2PolygonShape shape;
		shape.Set(vertices, count);
		return shape;
	}
    
    b2PolygonShape SoraPhysicalWorld::generateEdge(float32 v1, float32 v2, float32 v3, float32 v4) {
		b2PolygonShape shape;
		b2Vec2 bv1(v1, v2);
		b2Vec2 bv2(v3, v4);
		shape.SetAsEdge(pixel2b2cor(bv1), pixel2b2cor(bv2));
		return shape;
	}


} // namespace sora