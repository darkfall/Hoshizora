//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------
//
// Graphics was taken from the website of Daniel Cook:
//
//		http://www.lostgarden.com/
//
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>
#include <JSprite.h>
#include <JFileSystem.h>

#include "GameApp.h"


GameApp::GameApp()
{
	mBgTex = NULL;
	mBg = NULL;
	mSpriteTex = NULL;
	mBoy = NULL;
	mGirl = NULL;
}


GameApp::~GameApp()
{
}


void GameApp::Create()
{

	JRenderer* renderer = JRenderer::GetInstance();

	// Load background texture (texture format can be PNG, JPG or GIF)
	mBgTex = renderer->LoadTexture("bg.jpg");

	// Load texture for the characters (use TEX_TYPE_USE_VRAM to load texture into VRAM on PSP and this can speed up the rendering)
	mSpriteTex = renderer->LoadTexture("planet.png", TEX_TYPE_USE_VRAM);
	
	mBg = new JQuad(mBgTex, 0, 0, 480, 272);		// Create background quad for rendering.

	// Now let's create the image quads for the characters. The two characters are stored in one texture
	// so we have to know the location and size of each one to make the quads.

	mBoy = new JQuad(mSpriteTex, 0, 0, 67, 88);
	mBoy->SetHotSpot(33.5f, 44.0f);				// Set the pivot point to the middle.

	mGirl = new JQuad(mSpriteTex, 68, 0, 68, 90);
	mGirl->SetHotSpot(34.0f, 45.0f);

	
}


void GameApp::Destroy()
{

	SAFE_DELETE(mBgTex);
	SAFE_DELETE(mBg);
	SAFE_DELETE(mSpriteTex);
	SAFE_DELETE(mBoy);
	SAFE_DELETE(mGirl);

}


void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		// Do a screen shot when the TRIANGLE button is pressed
	{
		char s[80];
		sprintf(s, "ms0:/screenshot.png");
		JRenderer::GetInstance()->ScreenShot(s);
	}

	if (engine->GetButtonClick(PSP_CTRL_CROSS))			// Exit when the CROSS button is pressed
	{
		engine->End();
		return;
	}

}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();
	
	renderer->RenderQuad(mBg, 0, 0);

	renderer->RenderQuad(mBoy, 430, 150, 0.0f, 0.7f, 0.7f);			// Scale down the characters a little bit when rendering.
	renderer->RenderQuad(mGirl, 320, 235, 0.0f, 0.7f, 0.7f);

}


void GameApp::Pause()
{

}


void GameApp::Resume()
{

}
