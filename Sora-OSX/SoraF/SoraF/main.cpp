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

float cube[] = {
    100.f, 100.f, -100.f,
    300.f, 100.f, -100.f,
    300.f, 100.f, 100.f,
    100.f, 100.f, 100.f,
    
    100.f, 300.f, 100.f,
    300.f, 300.f, 100.f,
    300.f, 300.f, -100.f,
    100.f, 300.f, -100.f,
    
    100.f, 100.f, -100.f,
    300.f, 100.f, -100.f,
    300.f, 300.f, -100.f,
    100.f, 300.f, -100.f,
    
    
    300.f, 100.f, -100.f,
    300.f, 100.f, 100.f,
    300.f, 300.f, 100.f,
    300.f, 300.f, -100.f,
    
    
    300.f, 100.f, 100.f,
    100.f, 100.f, 100.f,
    100.f, 300.f, 100.f,
    300.f, 300.f, 100.f,
    
    
    100.f, 100.f, -100.f,
    100.f, 100.f, 100.f,
    100.f, 300.f, 100.f,
    100.f, 300.f, -100.f,
};

float tex[] = {
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f, 
    0.f, 1.f,
};

float color[] = {
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000
};

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

sora::SoraRenderBuffer::Ptr vertexBuffer;
sora::SoraRenderBuffer::Ptr indexBuffer;

struct myVertex {
    float x, y, z;
    float nx, ny, nz;
    uint32 col;
  //  float tx, ty;
};

sora::SoraTextureHandle movieTexture;

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
            sphereModel->render();
            
            // render a buffer
        }
        rs->endScene();
        
        /*   sphereModel->getTransform().setPosition(xp, yp, 0.f);
         sphereModel->getTransform().setRotation(sora::SoraQuaternion(sora::DegreeToRadius(x), sora::DegreeToRadius(y), sora::DegreeToRadius(z), 0));
         sphereModel->getTransform().setScale(s, s, s);*/
        /*  
         sora::SoraCore::Ptr->getRenderSystem()->setTransformMatrix(vertexL.getTransform().getTransformMatrix());
         sora::SoraAABB3 abox = sora::SoraAABB3::AABB3FromVertices(vertexL.getVertexList().begin(), vertexL.getVertexList().size());
         
         sora::SoraVertexArray arr = abox.buildVertexArray();
         
         sora::SoraCore::Ptr->renderWithVertices(0, BLEND_DEFAULT, arr.begin(), 24, sora::Line);
         */
        
        
        boxModel->render();
        
        mBg.enable3D(true);
        mBg.getTransform().setRotation(x, y, z);
        
        //  mBg.getTransform().setScale(1.5f, 1.f, 1.f);
        mBg.setPosition(512, 384);
        mBg.setCenter(mBg.getSpriteWidth()/2, mBg.getSpriteHeight()/2);
        //    mBg.render();
        
        mText2.enable3D(true);
        mText2.getTransform().setRotation(0.f, 0.f, 1.f);
        mText2.setPosition(512.f, 384.f);
        //  mText2.render();
        
        sora::SoraCore::Ptr->switchTo2D();
        
        mText2.enable3D(false);
        mText2.setPosition(500.f, 600.f);
        mText2.render();
        
        sora::SoraCore::Ptr->renderLine(10.f, 100.f, 10.f, 0.f, sora::Color::Red);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 100.f, sora::Color::Blue);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 0.f, sora::Color::Green);
        
        sora::SoraCore::Ptr->setTransform();
        mFont->render(0.f, 80.f, L"|#00FFFF|O");
        mFont->print(0.f, 600.f, sora::SoraFont::AlignmentLeft, L"fps : %f\nmodel: vertex: %d, UV: %d, normal: %d, face: %d", sora::SoraCore::Ptr->getFPS(), 
                     boxModel->getMesh()->vertexCount(),
                     boxModel->getMesh()->UVCount(),
                     boxModel->getMesh()->normalCount(),
                     boxModel->getMesh()->faceCount()
                     
                     );
        
        sora::SoraGameApp::EndScene();
        

    }
    
    void onUpdate(float dt) {   
        sora::SoraRenderBuffer::Ptr myRenderBuffer;
        sora::SoraRenderBuffer::Ptr myIndexBuffer;
        
        sphereModel->update(dt);
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
        sora::SoraTaskManager::StartAsyncTask(task);
        
        WaitForTaskFinish(task);
        
        sphereModel->getMaterial()->setTexture(0, sora::SoraTexture::LoadFromFile("kagami2.jpg"));
        
    }
    
    void load(sora::SoraTask* task) {
        sphereModel = sora::SoraModelLoader::BuildModelFromSphere(sora::SoraSphere(sora::SoraVector3(0.0, 0.0, 0.0), 200.f), 
                                                                  50, 
                                                                  new sora::SoraMaterial(sora::SoraMaterial::Solid));
        
        sora::SoraAABB3 abox = sphereModel->getBoudingBox();
        boxModel = sora::SoraModelLoader::BuildModelFromAABB(abox, new sora::SoraMaterial(sora::SoraMaterial::WireFrame));

        
        mShape = sora::SoraShape::Arc(300.f, 300.f, 150.f, 0.f, sora::DegreeToRadius(90.f), 3.f, 0xFFFFFFFF);
        mShape.enableOutline(3.f, 0xFFFF0000);
        mShape.setClosed(true);
        
        
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
        sphereModel->moveTo(1000, 10000, 100);

    }
    
    void onFadeFinish(sora::SoraSprite* obj) {
        obj->fadeTo(1.f, 2.f);
        obj->addEffect(sora::CreateEffectFade(0.f, 1.f, 1.f, sora::ImageEffectPingpong));
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

int main(int argc, char* argv[]) {    
        
    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    app.addState(new GameInitState, "init");
    app.run("init");
    
    return 0;

}