//
//  SoraMainWindow.cpp
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#include "SoraMainWindow.h"
#include "SoraCore.h"
#include "SoraMath.h"

bool MainWindow::updateFunc() {
    return false;
}

bool MainWindow::renderFunc() {
    sora::SoraCore* inst = sora::SoraCore::Instance();
  
    inst->beginScene();
    
    if(mFont) {
        mFont->setLineWidth(0.f);
        mFont->setLineRotation(0.f);
        mFont->setCharRotation(0.f);
        mFont->setColor(0xFFFFFFFF);
        mFont->setScale(1.f);
        mFont->render(50.f, 0.f, L"Hello World from Hoshizora!");

        mFont->setLineWidth(150.f);
        mFont->render(50.f, 150.f, L"最大行宽150像素下字符的渲染, 超出部分自动换行");
        
        
        mFont->render(50.f, 50.f, sora::FONT_ALIGNMENT_LEFT, L"来自Sora引擎的Hello World! :)");

		mFont->render(100.f, 200.f, sora::FONT_ALIGNMENT_CENTER, L"中对齐");

        mFont->render(300.f, 100.f, sora::FONT_ALIGNMENT_RIGHT, L"右对齐");

        mFont->setLineWidth(0.f);
        mFont->setColor(0xFFFF0000, 0);
        mFont->setColor(0xFF00FF00, 1);
        mFont->setColor(0xFF0000FF, 2);
        mFont->setColor(0xFFFFFFFF, 3);
        mFont->render(50.f, 450.f, L"顶点着色支持");
        
        mFont->setColor(0xFFFFFFFF);
        mFont->setLineRotation(90.f);
        mFont->render(50.f, 300.f, L"渲染角度设置");
        
        mFont->setLineRotation(45.f, true);
        mFont->render(100.f, 300.f, L"单字符渲染角度设置");
        
        mFont->setLineRotation(0.f);
        mFont->setScale(1.3f);
        mFont->render(300.f, 300.f, L"字体图像放大功能");
        
        mFont->setScale(1.f);
        mFont->print(0.f, getWindowHeight()-30.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %.4f", sora::SORA->getFPS());
    }

    sora::SORA->endScene();
    
    return false;
}

void MainWindow::init() {
    sora::SORA->setFPS(60);
    mFont = sora::SORA->createFont(L"simsum.ttf", 24);
    if(!mFont)
        THROW_SORA_EXCEPTION("Error creating font");
}