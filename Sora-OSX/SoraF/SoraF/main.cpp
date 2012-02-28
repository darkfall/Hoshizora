#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraTimestamp.h"
#include "SoraConfigUtil.h"

#include "SoraDirectoryIterator.h"
#include "SoraFileUtility.h"
#include "SoraFont.h"
#include "SoraText.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"
#include "SoraMath.h"

#include "SoraFunction.h"

#include "cmd/SoraConsole.h"

#include "SoraTask.h"
#include "SoraShape.h"
#include "SoraShader.h"

#include "SoraLogger.h"
#include "SoraResourceFile.h"

#include "SoraRenderSystem.h"

#include "SoraSpriteManager.h"

sora::SoraPhysicBody* body;

float x = 0;
float y = 0;
float z = -0;

float xpos = 0.f;
float ypos = 0.f;
float zpos = 0.f;

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraSprite.h"

#include "SoraVertexList.h"

float s = 1.f;

#include "util/SoraPointTemplate.h"


float xp = 512, yp = 384;

#include "Sora3D/SoraModel.h"
#include "Sora3D/SoraModelLoader.h"

sora::SoraModel::Ptr sphereModel;
sora::SoraModel::Ptr boxModel;

#include "math/SoraPerlinNoise.h"

#include "debug/SoraGlobalProfiler.h"
#include "debug/SoraAutoProfile.h"

#include "SoraShaderParamObject.h"

#include "SoraParticleF/SoraParticleFSystem.h"
#include "SoraParticleF/SoraParticleFBasicEmitter.h"
#include "SoraParticleF/SoraParticleFRenderer.h"

sora::SoraParticleFSystem pfSys;

class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
public:
    GameInitState() {
        
    //    camera->lookAt(512.f, 384.f, -500.f, 512.f, 384.f, 500.f, 0, 0, 1);
        
    }
    
    void onRender() {
        
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_DOWN))
            x -= 1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_UP))
            x += 1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_LEFT))
            y -= 1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_RIGHT))
            y += 1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_Q))
            z -= 1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_E))
            z += 1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_W))
            s -= 0.01f;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_S))
            s += 0.01f;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_I))
            yp -= 1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_K))
            yp += 1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_J))
            xp -= 1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_L))
            xp += 1;
        //     camera->setPosition(x, y, z);
        
        
        //  camera->setPosition(x, y, z);
        //   z -= 1.f;
        
        sora::SoraRenderSystem* rs = 
        sora::SoraCore::Ptr->getRenderSystem();
        
        sora::SoraGameApp::BeginScene();
        
        sora::SoraMatrix4 rotMat = 
        sora::SoraMatrix4::RotMat(sora::DegreeToRadius(x), 
                                  sora::DegreeToRadius(y), 
                                  sora::DegreeToRadius(z));        
        sora::SoraMatrix4 viewMat = 
        sora::SoraMatrix4::TransMat(xp, yp, 0) * 
        rotMat * 
        sora::SoraMatrix4::ScaleMat(s, s, s);
        
        rs->switchTo2D();
        // render a 2d texture
        mBg.enable3D(false);
        mBg.render(0, 0);
        
        
        rs->switchTo3D();
        rs->setTransformMatrix(viewMat);
        
        {
            // render a model
   //         sphereModel->render();
            
            // render a buffer
        }
        rs->endScene();
        
        sora::SoraCore::Ptr->switchTo2D();
        
        pfSys.render();

        mText2.enable3D(false);
        mText2.setPosition(500.f, 600.f);
        mText2.render();
        
        sora::SoraCore::Ptr->renderLine(10.f, 100.f, 10.f, 0.f, sora::Color::Red);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 100.f, sora::Color::Blue);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 0.f, sora::Color::Green);
        
        sora::SoraCore::Ptr->setTransform();
        mFont->render(0.f, 80.f, L"|#00FFFF|O");
        mFont->print(0.f, 600.f, sora::SoraFont::AlignmentLeft, L"fps : %f\nvertex: %d", sora::SoraCore::Ptr->getFPS(), 
                     sphereModel->getMesh()->getVertexCount()
                     );
        
        sora::SoraGameApp::EndScene();
        
    }
    
    void onUpdate(float dt) {   
        sora::SoraRenderBuffer::Ptr myRenderBuffer;
        sora::SoraRenderBuffer::Ptr myIndexBuffer;
        
        sphereModel->update(dt);
        pfSys.update(dt);
    }
    
    void onKeyEvent(sora::SoraKeyEvent* keyEvent) {
    }
    
    void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
    }
    
    void onEnter() {
        sora::SoraTask* task = 
            new sora::SoraTask("ResourceLoad", 
                               sora::Bind(this, 
                                          &GameInitState::load));
        sora::SoraTaskManager::StartTask(task);
        
        WaitForTaskFinish(task);
        
        sphereModel->getMaterial()->setTexture(0, sora::SoraTexture::LoadFromFile("kagami2.jpg"));
        
        
        sora::SoraParticleFRenderer* renderer = new sora::SoraParticleFRenderer();
        renderer->setTeture(sora::SoraCore::Instance()->createTexture("particles.png"));
        renderer->setTextureRect(sora::SoraRect(0, 0, 32,32));
        
        pfSys.addRenderer(renderer);
        
        sora::SoraParticleFBasicEmitter* emitter = new sora::SoraParticleFBasicEmitter();
        emitter->setMaxSpeed(50);
        emitter->setMinSpeed(10);
        
        emitter->setMaxAngle(sora::SoraVector(sora::F_PI, sora::F_PI));
        emitter->setMinAngle(sora::SoraVector(-sora::F_PI, 0));
        
        emitter->setStartColor(sora::Color::Red);
        emitter->setEndColor(sora::Color::Blue.a(0));
        
        emitter->setMinLifeTime(10.f);
        emitter->setMaxLifeTime(20.f);
        
        emitter->setMinSpin(sora::SoraVector3());
        emitter->setMaxSpin(sora::SoraVector3());
        
        emitter->setStartScale(sora::SoraVector3(1, 1, 1));
        emitter->setEndScale(sora::SoraVector3(1, 1, 1));
        
        emitter->setEmissionNum(10);
        emitter->setEmissionDuration(2);
        emitter->setEmissionRepeatTime(-1);
        emitter->setEmissionInterval(1);
        
        pfSys.addEmitter(emitter);
        pfSys.setPosition(sora::SoraCore::Ptr->getScreenWidth() / 2, 
                          sora::SoraCore::Ptr->getScreenHeight() / 2);
    }
    
    void load(sora::SoraTask* task) {
        sphereModel = sora::SoraModelLoader::BuildModelFromSphere(sora::SoraSphere(sora::SoraVector3(0.0, 0.0, 0.0), 200.f), 
                                                                  50, 
                                                                  new sora::SoraMaterial(sora::SoraMaterial::Solid));
        
     //   sora::SoraAABB3 abox = sphereModel->getBoudingBox();
      //  boxModel = sora::SoraModelLoader::BuildModelFromAABB(abox, new sora::SoraMaterial(sora::SoraMaterial::WireFrame));

        sora::SoraResourceFile fontData("cour.ttf");
        mFont = sora::SoraFont::LoadFromMemory(fontData, fontData.size(), 20, "BankGothic");
        if(!mFont) {
            sora::SoraCore::Instance()->messageBox("Error loading font", "error", MB_OK | MB_ICONERROR);
        } else {
            mText.setFont(mFont);
            
            mText2.setFont(mFont);
            mText2.setText(L"|#00FFFF|Hello |#FF0000|World! |#CDCDCD|Sora ~ |#FFDEAD|Chan~");
            mText2.setPosition(0.f, sora::SoraCore::Instance()->getScreenHeight()-mFont->getHeight()*5.5);
        }
        
        registerEventFunc(this, &GameInitState::onKeyEvent);  
        
     //   sphereModel->moveTo(1000, 10000, 100);

    }
    
private:
    sora::SoraFont* mFont;
    sora::SoraText mText;
    sora::SoraText mText2;
    sora::SoraSprite mBg;
    sora::SoraSprite mBg2;
    
    sora::SoraShape mShape;
    
    sora::SoraShader* mShader;
    sora::SoraSprite sc1;
    sora::SoraSprite sc2;
    
};

#include "util/SoraDictionary.h"

int inc(int a) {
    return (a & 1) ? (inc(a >> 1) << 1) : (a | 1); 
    
}

int main(int argc, char* argv[]) { 
    printf("%d, %d, %d, %d\n", inc(inc(inc(inc(inc(inc(1)))))), inc(2), inc(3), inc(1000));
        
    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    app.addState(new GameInitState, "init");
    app.run("init");
    
    return 0;

}