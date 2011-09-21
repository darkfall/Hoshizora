#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "mainWindow.h"

#include "SoraTimestamp.h"

#include "../SoraParticleEditor/peMainWindow.h"

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

class GameInitState: public sora::SoraGameState {
public:
    void onRender() {
        sora::SoraGameApp::BeginScene();
        mBg.render();
        mText.render();
        sora::SoraGameApp::EndScene();
    }
    
    void onUpdate(float dt) {
        mBg.update(dt);
    }
    
    void onEnter() {
        sora::SoraResourceFileAuto fontData("Bank Gothic Medium BT.ttf");
        mFont = sora::SoraFont::LoadFromMemory(fontData, fontData.size(), 40, "BankGothic");
        if(mFont) {
            mText.set(L"Hello World!", mFont);
            mText.renderToSprite();
            
            sora::SoraFont::Release(mFont);
        }
        
        mBg.setTexture(sora::SoraTexture::LoadFromFile("test.png"));
        mBg.moveTo(100, 100, 1);
        mBg.fadeToAndNotify(0.f, 1.f, sora::Bind(this, &GameInitState::onFadeFinish));
        
        mPostEffect.attachFragmentShader("HDRBlur.cg", "HDRBlur");
        
        mMusic = sora::SoraSoundSystem::LoadMusicFromFile("02.mp3");
        if(mMusic) {
            mMusic->setRepeat(true);
            mMusic->play();
        }
    }
    
    void onFadeFinish(sora::SoraSprite* obj) {
        obj->fadeTo(1.f, 2.f);
        obj->addEffect(sora::CreateEffectFade(0.f, 1.f, 1.f, sora::IMAGE_EFFECT_PINGPONG));
    }
    
private:
    sora::SoraFont* mFont;
    sora::SoraText mText;
    sora::SoraSprite mBg;
    sora::SoraMusicFile* mMusic;
    
    sora::SoraPostEffect mPostEffect;
};

static int count = 0;
void asyncLoad(void* data,ulong32 size,void* userdata) {
    printf("** loaded data with size: %d, c : %d\n", size, count);
    sora::SoraResourceManager::FreeResourceFile(data);
    ++count;
}
struct myClass {
    void asyncLoad(void* data,ulong32 size,void* userdata) {
        printf("** loaded data with size: %d, c : %d\n", size, count);
        sora::SoraResourceManager::FreeResourceFile(data);
        ++count;
    }
};

int main(int argc, char* argv[]) {    
        
    sora::SoraGameAppDef def("config.xml");
    sora::SoraGameApp app(def);
    
    sora::SoraResourceManager::LoadAndAttachResourcePack("Textures.zip");
    
    myClass* obj = new myClass;
    for(int i=0; i<1000; ++i) {
        sora::SoraResourceManager::LoadResourceFileAsync("Textures/Particle006.png", asyncLoad);
        sora::SoraResourceManager::LoadResourceFileAsync("02.mp3", sora::Bind(obj, &myClass::asyncLoad));
    }
    
    app.addState(new GameInitState, "init");
    app.run("init");
    
    return 0;

}