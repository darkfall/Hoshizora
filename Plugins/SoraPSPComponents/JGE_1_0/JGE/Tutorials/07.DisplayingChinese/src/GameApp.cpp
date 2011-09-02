//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>
#include <JGBKFont.h>
#include <JSoundSystem.h>

#include <JTTFont.h>
#include <Encoding.h>

#include "GameApp.h"

GameApp::GameApp()
{
	mBitmapFont12 = NULL;
	mBitmapFont16 = NULL;
	mTTFont = NULL;
	mTTFont2 = NULL;

}


GameApp::~GameApp()
{
}


void GameApp::Create()
{

 	mBitmapFont12 = new JGBKFont();
 	mBitmapFont12->Init("ASC12", "GBK12", 12);		// Load bitmap font.
 
 	mBitmapFont16 = new JGBKFont();
 	mBitmapFont16->Init("ASC16", "GBK16", 16);

	mTTFont = new JTTFont();
	mTTFont->Load("gkai00mp.ttf", 32);				// Load TTF font.

	mTTFont2 = new JTTFont();						// Create a different size font from the data of another one.
	mTTFont2->Load(mTTFont, 60);



}


void GameApp::Destroy()
{
	
	SAFE_DELETE(mTTFont);
	SAFE_DELETE(mTTFont2);
	SAFE_DELETE(mBitmapFont12);
	SAFE_DELETE(mBitmapFont16);

}


void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))	// do a screen shot when TRIANGLE is pressed
	{
		char s[80];
		sprintf(s, "ms0:/screenshot.png");
		JRenderer::GetInstance()->ScreenShot(s);
	}

	if (engine->GetButtonClick(PSP_CTRL_CROSS))		// exit when the CROSS is pressed
	{
		engine->End();
		return;
	}

}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();

	renderer->EnableTextureFilter(false);
	
	renderer->ClearScreen(ARGB(0,0,0,0));

	
	u8 text[] = "点阵汉字测试!";
	u8 poem1[] = "床前明月光, 疑是地上霜.";
	u8 poem2[] = "举头望明月,";
	u8 poem3[] = "低头思故乡.";


 	mBitmapFont12->RenderString(text, SCREEN_WIDTH_F/2, 30, JGETEXT_CENTER);
 	mBitmapFont16->RenderString(text, SCREEN_WIDTH_F, 60, JGETEXT_RIGHT);
	
	mTTFont->RenderString(poem1, 10, 100);
	mTTFont2->RenderString(poem2, 25, 135);
	mTTFont2->RenderString(poem3, 55, 195);
}


void GameApp::Pause()
{
}


void GameApp::Resume()
{
}
