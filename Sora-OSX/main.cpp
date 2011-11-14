#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "mainWindow.h"

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

#include "SoraBox2dPhysics/SoraBox2dPhysicWorld.h"
#include "physics/SoraPhysicDef.h"
#include "physics/SoraPhysicShape.h"
#include "physics/SoraPhysicBody.h"

#include "SoraRenderSystem.h"

#include "SoraSpriteManager.h"

float lx, ly, rx, ry;
float maxsize, maxiteration, iterinc;
float cr, cg, cb;

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

sora::Sora3DCamera* camera;

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

#include "SoraVlcMoviePlayer/SoraVlcMoviePlayer.h"

sora::SoraShaderParameter ppppp;

sora::SoraVlcMoviePlayer moviePlayer;
sora::SoraTextureHandle movieTexture;

class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
public:
    GameInitState() {
        sphereModel = sora::SoraModelLoader::BuildModelFromSphere(sora::SoraSphere(sora::SoraVector3(0.0, 0.0, 0.0), 200.f), 
                                                                  50, 
                                                                  new sora::SoraMaterial(sora::SoraMaterial::Solid));
        
        sora::SoraAABB3 abox = sphereModel->getBoudingBox();
        boxModel = sora::SoraModelLoader::BuildModelFromAABB(abox, new sora::SoraMaterial(sora::SoraMaterial::WireFrame));
        
        camera = new sora::Sora3DCamera(sora::Sora3DCamera::Perspective);
        camera->setProjectionMatrix(sora::SoraMatrix4::PerspectiveMat(60.f, 1.33, 10.f, 3000.f));
        camera->setPosition(-512.f, -384.f, -1000.f);
        
    //    camera->lookAt(512.f, 384.f, -500.f, 512.f, 384.f, 500.f, 0, 0, 1);
        
    }
    
    void onRender() {

        sora::SoraGameApp::BeginScene();
        
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
        
        sora::SoraMatrix4 rot = sora::SoraMatrix4::RotMat(sora::DegreeToRadius(x), sora::DegreeToRadius(y), sora::DegreeToRadius(z));
        sora::SoraMatrix4 translate = sora::SoraMatrix4::TransMat(xp, yp, 0);
        
        sora::SoraMatrix4 myView = translate * rot * sora::SoraMatrix4::ScaleMat(s, s, s);
        
      //  sora::SoraCore::Ptr->renderWithVertices(0, BLEND_DEFAULT_Z, &vt[0], 8, sora::TriangleStrip);
        
        
     //   vertexL.setPosition(200.f, 200.f);
     //   sora::SoraCore::Ptr->getRenderSystem()->setRenderState(sora::TextureWrap0, sora::TextureWrapClampToBoarder);
     //   sora::SoraCore::Ptr->getRenderSystem()->setRenderState(sora::TextureWrap1, sora::TextureWrapClampToBoarder);
        
        sora::SoraCore::Ptr->switchTo2D();
        // 2d
        mBg.enable3D(false);
    //    mBg.render(400.f, 400.f);
        
        ppppp = z;

        sora::SoraCore::Ptr->switchTo3D();
        sora::SoraCore::Ptr->getRenderSystem()->setTransformMatrix(myView);
        
        {
            sora::SoraTimestamp ts;

       /*     sora::SoraCore::Ptr->getRenderSystem()->renderBuffer(0, 
                                                                 sora::Triangle, 
                                                                 vertexBuffer, 
                                                                 sora::SoraRenderBuffer::NullObject());
            */
            sphereModel->render();

        }
        
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

    }
    
    void onKeyEvent(sora::SoraKeyEvent* keyEvent) {
    }
    
    void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
    }
    
    void onEnter() {
        sora::SoraCore::Instance()->registerPhysicWorld(new sora::SoraBox2dPhysicWorld(0.f, 1.f, true));
        sora::SoraCore::Ptr->setFPS(60);
        
       // ppppp = sphereModel->getMaterial()->attachFragmentShader("Bloom.cg", "Bloom")->getParameter("rate");
        
        
        if(!moviePlayer.openMedia("[完全感覚Dreamer]打开姿势错误的罪恶王冠.mp4"))
            sora::SoraCore::Ptr->messageBox("Error opening movie", "Error", MB_OK);
        else {
            movieTexture = sora::SoraTexture::CreateEmpty(moviePlayer.getWidth(), moviePlayer.getHeight());
            moviePlayer.play();
            moviePlayer.bindTexture(movieTexture);
            sphereModel->getMaterial()->setTexture(0, movieTexture);
        }
                
    
        sc1.setTexture(sora::SoraTexture::LoadFromFile("bg-optd.png"));
       
        mBg.setTexture(sora::SoraTexture::LoadFromFile("test.png"));
      //  mBg.setColor(0xFF00FF00);
        
     /*   mBg.setPosition(300.f, 300.f);
        mBg.createPhysicBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::DynamicBody),
                             sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(100.f, 
                                                                                          100.f, 0.f, 0.f, 100.f)),
                                                        1.f,
                                                        0.f,
                                                        0.f);
        */
        mBg2.setTexture(sora::SoraTexture::CreateEmpty(100.f, 100.f));
        mBg2.setColor(0xFFFF0000);
        mBg2.setPosition(400.f, 300.f);
        mBg2.createPhysicBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::DynamicBody),
                              sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(100.f, 100.f, 50.f, 5.f, 0.f)),
                              1.f,
                              0.f,
                              0.f);
        
        body = sora::SoraPhysicWorld::CreateBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::StaticBody, sora::SoraVector(0.f, 600.f)));
        body->createFixture(sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(800.f, 10.f, 400.f, 5.f, 0.f)));
        
        mShader =  mBg.attachFragmentShader("MandelbrotSet.cg", "MandelbrotSet");
        
        lx = -2.5f;
        ly = 1.0f;
        rx = -2.f;
        ry = 2.f;
        maxsize = 2.f;
        maxiteration = 90.f;
        iterinc = 1.f;

        sora::SoraTask* task = new sora::SoraTask("ResourceLoad", sora::Bind(this, &GameInitState::load));
        sora::SoraTaskManager::StartAsyncTask(task);
        
        WaitForTaskFinish(task);
        
      //  mText2.setStyle(sora::SoraText::AlignmentCenter);

      //  mText2.enableRenderToSprite(true);
        mText.setStyle(sora::SoraFont::AlignmentRight);
        
        mShape.setTexture(sora::SoraTexture::LoadFromFile("background.png"));
        mShape.enableRenderToSprite(true);
        
      //  boxModel = sora::SoraModelLoader::LoadModel("old_key.obj");
        
    }
    
    void load(sora::SoraTask* task) {
      
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
               
        sora::SoraConfigParser parser;
        if(parser.open("mset.xml")) {
            if(parser.toNode("/mset")) {
                lx = parser.getFloat("lx", -2.5f);
                ly = parser.getFloat("ly", 1.0f);
                rx = parser.getFloat("rx", -2.f);
                ry = parser.getFloat("ry", 2.f);
                maxsize = parser.getFloat("max_size", 2.0f);
                maxiteration = parser.getFloat("max_iteration", 90.f);
                iterinc = parser.getFloat("iteration_increment", 1.f);
                cr = parser.getFloat("cr", 1.0f);
                cb = parser.getFloat("cb", 1.0f);
                cg = parser.getFloat("cg", 1.0f);
            }
        }
        
        if(!mShader) {
  //          sora::SoraCore::Instance()->messageBox("Error loading MadelbrotSet shader, see log for //detail error", "error", MB_OK);
        }
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

void msetCommands(sora::SoraConsoleEvent* evt) {
    
    std::string cmd = evt->getCmd();
    sora::StringType param = evt->getParams();
    if(cmd == "set_lx") {
        lx = param.asFloat();
        evt->pushResult("lx had been set to "+param);
    } else if(cmd == "set_ly") {
        ly = param.asFloat();
        evt->pushResult("ly had been set to "+param);
        
    } else if(cmd == "set_rx") {
        rx = param.asFloat();
        evt->pushResult("rx had been set to "+param);
        
    } else if(cmd == "set_ry") {
        ry = param.asFloat();
        evt->pushResult("ry had been set to "+param);
        
    } else if(cmd == "set_max_iteration") {
        maxiteration = param.asFloat();
        evt->pushResult("max iteration had been set to "+param);
        
    } else if(cmd == "set_max_size") {
        maxsize = param.asFloat();
        evt->pushResult("max_size had been set to "+param);
        
    } else if(cmd == "set_iteration_increment") {
        iterinc = param.asFloat();
        evt->pushResult("iteration increment had been set to "+param);
        
    } else if(cmd == "set_cr") {
        cr = param.asFloat();
        evt->pushResult("cr had been set to "+param);
        
    } else if(cmd == "set_cg") {
        cg = param.asFloat();
        evt->pushResult("cg had been set to "+param);
        
    } else if(cmd == "set_cb") {
        cb = param.asFloat();
        evt->pushResult("cb had been set to "+param);
    } else if(cmd == "tex") {
        sphereModel->getMaterial()->setTexture(0, sora::SoraTexture::LoadFromFile(param));
    }
}
#include "util/SoraDictionary.h"

#include "SoraGraphicAlgorithm.h"

SORA_DEF_CONSOLE_EVT_FUNC(msetCommands, "set_lx,set_ly,set_rx,set_ry,set_max_iteration,set_max_size,set_iteration_increment,set_cr,set_cg,set_cb, detail, tex, vertexmode");



int main(int argc, char* argv[]) {    
        
    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    app.addState(new GameInitState, "init");
    app.run("init");
    
    return 0;

}