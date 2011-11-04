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

sora::SoraPhysicBody* body;

float x = 510.f;
float y = 384.f;
float z = -500.f;

float xpos = 0.f;
float ypos = 0.f;
float zpos = 0.f;

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraSprite.h"

#include "SoraVertexList.h"

sora::SoraVertexList vertexL(sora::LineLoop);
sora::SoraShape shape;

void sphereMap(float x, float y, float z, float r, sora::SoraVertex& vertex) {
    vertex.ty = acosf(z/r) / sora::F_PI;
    if(vertex.ty > 0.0f) {
        vertex.tx = acosf(x/(r*sinf(sora::F_PI * vertex.ty))) / sora::F_PI*2;
    } else {
        vertex.tx = (sora::F_PI + acosf(x/(r*sinf(sora::F_PI*vertex.ty)))) / sora::F_PI*2;
    }
}

void getVertex(float r, float a, float b, sora::SoraVertex& vertex, float x, float y) {
    
    float sina = sinf(sora::DegreeToRadius(a));
    vertex.x = x+r * sina * cosf(sora::DegreeToRadius(b));
    vertex.y = y+r * sina * sinf(sora::DegreeToRadius(b));
    vertex.z = r * cosf(sora::DegreeToRadius(a));
    vertex.col = 0xFFFFFFFF;
    sphereMap(vertex.x-x, vertex.y-y, vertex.z, r, vertex);
}

sora::SoraVertex* getPointMatrix(GLfloat radius,GLint slices, float x, float y)
{
    int i,j,w=2*slices,h=slices;
    float a=0.0,b=0.0;
    float hStep=180.0/(h-1);
    float wStep=360.0/w;
    int length=w*h;
    sora::SoraVertex *matrix;
    matrix=(sora::SoraVertex *)malloc(length*sizeof(sora::SoraVertex));
    if(!matrix)return NULL;
    for(a=0.0,i=0;i<h;i++,a+=hStep)
        for(b=0.0,j=0;j<w;j++,b+=wStep)  
            getVertex(radius,a,b,matrix[i*w+j],x,y); 
    return matrix;
}

void buildSphere(GLfloat radius,GLint slices, float x, float y)
{
    int i=0,j=0,w=2*slices,h=slices;
    sora::SoraVertex *mx;
    mx=getPointMatrix(radius,slices,x,y);
    for(;i<h-1;i++)
    {
        for(j=0;j<w-1;j++) {
            vertexL.vertex(mx[i*w+j]);
            vertexL.vertex(mx[i*w+j+1]);
            vertexL.vertex(mx[(i+1)*w+j+1]);
            vertexL.vertex(mx[(i+1)*w+j]);
        }
        vertexL.vertex(mx[i*w+j]);
        vertexL.vertex(mx[i*w]);
        vertexL.vertex(mx[(i+1)*w]);
        vertexL.vertex(mx[(i+1)*w+j]);
    }
    free(mx);
}

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


void build(int detail) {
    vertexL.clear();
    buildSphere(200.f, 10 * detail, 0.f, 0.f);
}

void texx(const SoraString& tex) {
    vertexL.bindTexture(sora::SoraTexture::LoadFromFile(tex));

}

sora::Sora3DCamera* camera;


class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
public:
    GameInitState() {
        buildSphere(200.f, 10, 0.f, 0.f);
        
        shape.initWithVertexList(vertexL);
        
        camera = new sora::Sora3DCamera(sora::Sora3DCamera::Perspective);
        camera->setProjectionMatrix(sora::SoraMatrix4::PerspectiveMat(60.f, 1.33, 10.f, 3000.f));
        camera->setPosition(-512.f, -384.f, -1000.f);
    //    camera->lookAt(512.f, 384.f, -500.f, 512.f, 384.f, 500.f, 0, 0, 1);

    }
    
    void onRender() {
        sora::SoraGameApp::BeginScene();
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_DOWN))
            x -= 0.1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_UP))
            x += 0.1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_LEFT))
            y -= 0.1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_RIGHT))
            y += 0.1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_Q))
            z -= 0.1;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_E))
            z += 0.1;
        
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_W))
            s -= 0.01f;
        if(sora::SoraCore::Ptr->keyDown(SORA_KEY_S))
            s += 0.01f;
   //     camera->setPosition(x, y, z);


      //  camera->setPosition(x, y, z);
     //   z -= 1.f;
        
        sora::SoraMatrix4 rot = sora::SoraMatrix4::RotMat(x, y, z);
        sora::SoraMatrix4 translate = sora::SoraMatrix4::TransMat(512, 384, zpos);
        
        sora::SoraMatrix4 myView = sora::SoraMatrix4::TransMat(512, 384, zpos) * rot * sora::SoraMatrix4::ScaleMat(s, s, s) * sora::SoraMatrix4::TransMat(-512, -384, zpos) ;
      ///  sora::SoraCore::Ptr->getRenderSystem()->setTransformMatrix(myView);
        
      //  sora::SoraCore::Ptr->renderWithVertices(0, BLEND_DEFAULT_Z, &vt[0], 8, sora::TriangleStrip);
        
        
     //   vertexL.setPosition(200.f, 200.f);
     //   sora::SoraCore::Ptr->getRenderSystem()->setRenderState(sora::TextureWrap0, sora::TextureWrapClampToBoarder);
     //   sora::SoraCore::Ptr->getRenderSystem()->setRenderState(sora::TextureWrap1, sora::TextureWrapClampToBoarder);
        
        sora::SoraCore::Ptr->switchTo2D();
        // 2d
        mBg.enable3D(false);
    //    mBg.render(400.f, 400.f);
        

        sora::SoraCore::Ptr->switchTo3D();
   //     sora::SoraCore::Ptr->getRenderSystem()->setProjectionMatrix(sora::SoraMatrix4());
        
        vertexL.getTransform().setPosition(512, 384, 0.f);
        vertexL.getTransform().setRotation(x, y, z);
        vertexL.getTransform().setScale(s, s, s);
        vertexL.render();
        
        mBg.enable3D(true);
        mBg.getTransform().setRotation(x, y, z);
        
      //  mBg.getTransform().setScale(1.5f, 1.f, 1.f);
        mBg.setPosition(512, 384);
        mBg.setCenter(mBg.getSpriteWidth()/2, mBg.getSpriteHeight()/2);
     //   mBg.render();
        
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
        mFont->print(0.f, 600.f, sora::SoraFont::AlignmentLeft, L"fps : %f\nvertex: %d", sora::SoraCore::Ptr->getFPS(), vertexL.size());
        
        
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

      //  mText2.enableRenderToSprite(true);
        mText.setStyle(sora::SoraFont::AlignmentRight);
        
        mShape.setTexture(sora::SoraTexture::LoadFromFile("background.png"));
        mShape.enableRenderToSprite(true);

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
#include "SoraRenderSystemExtension.h"

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
    } else if(cmd == "detail") {
        build(param.asInt());
    } else if(cmd == "tex") {
        texx(param.get());
    } else if(cmd == "vertexmode") {
        switch(param.asInt()) {
            case 1:
                vertexL.setRenderMode(sora::LineLoop);
                break;
            case 2:
                vertexL.setRenderMode(sora::Quad);
                break;
            case 3:
                vertexL.setRenderMode(sora::Triangle);
                break;
        }
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