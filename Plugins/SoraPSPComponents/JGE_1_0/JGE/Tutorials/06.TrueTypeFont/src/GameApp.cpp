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

	mTTFont1 = NULL;
	mTTFont2 = NULL;
	mTTFSmall = NULL;

}


GameApp::~GameApp()
{
}


void GameApp::Create()
{

	mTTFont1 = new JTTFont();
	mTTFont1->Load("BorisBlackBloxx.ttf", 64);

	mTTFont2 = new JTTFont();			
	mTTFont2->Load(mTTFont1, 20);		// Create the 2nd font using font info and data from the 1st one, but with a different size.

	mTTFSmall = new JTTFont(JTTFont::CACHE_IMAGE_512x512);					// We want to cache all the ASCII characters so we specify a large cache image.
	mTTFSmall->Load("steelfib.ttf", 32, JTTFont::MODE_PRECACHE_ASCII);		// Load font and cache all the ASCII characters at the same time.
	mTTFSmall->Unload();													// Since we have all the characters we need in the cache, we can delete the 
																			//	font bitmap to save some memory.

}


void GameApp::Destroy()
{

	SAFE_DELETE(mTTFont1);
	SAFE_DELETE(mTTFont2);
	SAFE_DELETE(mTTFSmall);

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

	mTTFont1->SetColor(ARGB(255,255,255,255));
	mTTFont1->RenderString("Hello(Size 64)", 10, 10);

	mTTFont2->SetColor(ARGB(255,255,255,255));
	mTTFont2->RenderString("G'day ! (Size 20)", 10, 96);

	mTTFont2->SetColor(ARGB(255,255,0,0));
	mTTFont2->RenderString("I am HOT!", 310, 100);

	mTTFSmall->SetColor(ARGB(255,255,64,255));
	mTTFSmall->RenderString("She sells sea shells on the seashore. (Size 32)", 10, 200);

	mTTFont1->SetColor(ARGB(192,0,0,255));
	mTTFont1->RenderString("SO COOL!", 50, 120);

	mTTFont1->SetColor(ARGB(96,0,128,0));
	mTTFont1->RenderString("SO COOL!", 55, 125);

}


void GameApp::Pause()
{
}


void GameApp::Resume()
{
}
