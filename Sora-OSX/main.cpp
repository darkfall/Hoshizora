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
#include "SoraPostEffect.h"

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

    }
}

#include "SoraGraphicAlgorithm.h"

SORA_DEF_CONSOLE_EVT_FUNC(msetCommands, "set_lx,set_ly,set_rx,set_ry,set_max_iteration,set_max_size,set_iteration_increment,set_cr,set_cg,set_cb");

sora::SoraPhysicBody* body;

float x = 0.f;
float y = 0.f;

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraSprite.h"

sora::SoraVertex vt[24];

class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
public:
    GameInitState() {
        vt[0].x = 300.f; vt[0].y = 300.f; vt[0].z = -100.f;
        vt[1].x = 600.f; vt[1].y = 300.f; vt[1].z = -100.f;
        vt[2].x = 600.f; vt[2].y = 600.f; vt[2].z = -100.f;
        vt[3].x = 300.f; vt[3].y = 600.f; vt[3].z = -100.f;
        vt[4].x = 300.f; vt[4].y = 300.f; vt[4].z = 100.f;
        vt[5].x = 600.f; vt[5].y = 300.f; vt[5].z = 100.f;
        vt[6].x = 600.f; vt[6].y = 600.f; vt[6].z = 100.f;
        vt[7].x = 300.f; vt[7].y = 600.f; vt[7].z = 100.f;
    }
    
    void onRender() {
        sora::SoraGameApp::BeginScene();
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_DOWN))
            x -= 0.001f;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_UP))
            x += 0.001f;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_LEFT))
            y -= 0.001f;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_RIGHT))
            y += 0.001f;
        
        sora::SoraMatrix4 myView = sora::SoraMatrix4::RotMat(x, y, 0.f);
        
        for(int i=0; i<16;++i) {
            printf("%f, ", myView.x[i]);
            if((i+1)%4==0)
                printf("\n");
        }
        printf("\n");

        sora::SoraCore::Ptr->getRenderSystem()->setTransformMatrix(myView);
        
      //  sora::SoraCore::Ptr->renderWithVertices(0, BLEND_DEFAULT_Z, &vt[0], 8, sora::SORA_TRIANGLES_STRIP);
        mBg.setZ(-100.f);
        mBg.render(100.f, 100.f);
        
        mBg.setZ(100.f);
        mBg.render(100.f, 100.f);
        
        
        mText2.setPosition(100.f, 100.f);
        mText2.render();
        
        
        sora::SoraCore::Ptr->renderLine(10.f, 100.f, 10.f, 0.f, sora::Color::Red);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 100.f, sora::Color::Blue);
        sora::SoraCore::Ptr->getRenderSystem()->renderLine(10.f, 100.f, 110.f, 0.f, sora::Color::Green);
        
        sora::SoraCore::Ptr->setTransform();
        mFont->render(0.f, 80.f, L"|#00FFFF|O");
        
        
        sora::SoraGameApp::EndScene();
    }
    
    void onUpdate(float dt) {
        mBg.update(dt);
        mBg2.update(dt);
        
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_R)) {
            mShader = mBg.attachFragmentShader("MandelbrotSet.cg", "MandelbrotSet");
            if(!mShader) {
                sora::SoraCore::Instance()->messageBox("Error loading MadelbrotSet shader, see log for detail error", "error", MB_OK | MB_ICONERROR);
            }
        }
        
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_LEFT)) 
            lx -= 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_RIGHT)) 
            lx += 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_UP)) 
            ly -= 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_DOWN)) 
            ly += 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_W)) 
            ry -= 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_S)) 
            ry += 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_A)) 
            rx -= 0.01f;
        if(sora::SoraCore::Instance()->keyDown(SORA_KEY_D)) 
            rx += 0.01f;
        
        if(mShader) {
            mShader->setParameter1f("lx", lx);
            mShader->setParameter1f("ly", ly);
            mShader->setParameter1f("rx", rx);
            mShader->setParameter1f("ry", ry);
            
            mShader->setParameter1f("cr", cr);
            mShader->setParameter1f("cg", cg);
            mShader->setParameter1f("cb", cb);
            mShader->setParameter1f("max_size", maxsize);
            mShader->setParameter1f("max_iteration", maxiteration);
            mShader->setParameter1f("iteration_increment", iterinc);
        }
        
        mText.setText(sora::StringType(sora::vamssg("lx: %.2f ly: %.2f rx: %.2f ry: %.2f\nmax_size: %.2f max_iteration: %.2f iteration_increment: %.2f", lx, ly, rx, ry, maxsize, maxiteration, iterinc)).getw());
        mText.enableRenderToSprite(true);
        
    }
    
    void onKeyEvent(sora::SoraKeyEvent* keyEvent) {
    }
    
    void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
        float d =  atan2f(to.y - from.y,
                          to.x - from.x);
        mBg.getPhysicBody()->applyLinearImpulse(cosf(d),
                                                sinf(d),
                                                50.f,
                                                0.f);
    }
    
    void onEnter() {
        sora::SoraCore::Instance()->registerPhysicWorld(new sora::SoraBox2dPhysicWorld(0.f, 1.f, true));
        
        
        
        sc1.setTexture(sora::SoraTexture::LoadFromFile("bg-optd.png"));
       
        mBg.setTexture(sora::SoraTexture::LoadFromFile("test.png"));
      //  mBg.setColor(0xFF00FF00);
        
     /*   mBg.setPosition(200.f, 200.f);
        mBg.createPhysicBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::DynamicBody),
                             sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(100.f, 
                                                                                          100.f, 0.f, 0.f, 100.f)),
                                                        1.f,
                                                        0.f,
                                                        0.f);
        */
        mBg2.setTexture(sora::SoraTexture::CreateEmpty(100.f, 100.f));
        mBg2.setColor(0xFFFF0000);
        mBg2.setPosition(400.f, 200.f);
        mBg2.createPhysicBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::DynamicBody),
                              sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(100.f, 100.f, 50.f, 5.f, 0.f)),
                              1.f,
                              0.f,
                              0.f);
        
        body = sora::SoraPhysicWorld::CreateBody(sora::SoraPhysicBodyDef(sora::SoraPhysicBodyDef::StaticBody, sora::SoraVector(0.f, 600.f)));
        body->createFixture(sora::SoraPhysicFixtureDef(sora::SoraPhysicShape::BoxAsShape(800.f, 10.f, 400.f, 5.f, 0.f)));
        
        mShader = /* mBg.attachFragmentShader("MandelbrotSet.cg", "MandelbrotSet")*/ 0;
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

        mText2.enableRenderToSprite(true);
        mText.setStyle(sora::SoraText::AlignmentRight);
        
        mShape.setTexture(sora::SoraTexture::LoadFromFile("background.png"));
        mShape.enableRenderToSprite(true);

    }
    
    void load(sora::SoraTask* task) {
        
        mShape = sora::SoraShape::Arc(200.f, 200.f, 150.f, 0.f, sora::DegreeToRadius(90.f), 3.f, 0xFFFFFFFF);
        mShape.enableOutline(3.f, 0xFFFF0000);
        mShape.setClosed(true);
        
        sora::SoraResourceFile fontData("cour.ttf");
        mFont = sora::SoraFont::LoadFromMemory(fontData, fontData.size(), 60, "BankGothic");
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
            sora::SoraCore::Instance()->messageBox("Error loading MadelbrotSet shader, see log for detail error", "error", MB_OK);
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
#include "SoraRenderSystemExtension.h"

int main(int argc, char* argv[]) {    
        
    sora::SoraCore::Ptr->enableRenderSystemExtension(sora::SORA_EXTENSION_FSAA);
    
    sora::SoraCore::Ptr->setRenderSystemExtensionParam(sora::SORA_EXTENSION_FSAA, 4);

    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    app.addState(new GameInitState, "init");
    app.run("init");
    
    return 0;

}