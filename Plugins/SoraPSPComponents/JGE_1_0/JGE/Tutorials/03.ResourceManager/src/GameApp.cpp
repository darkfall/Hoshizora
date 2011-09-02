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
#include <JResourceManager.h>

#include "GameApp.h"


GameApp::GameApp()
{
	mResourceMgr = NULL;

}


GameApp::~GameApp()
{
}


void GameApp::Create()
{

	mResourceMgr = new JResourceManager();
	mResourceMgr->LoadResource("planet.res");			// load game assets that are defined in the resource file
	
}


void GameApp::Destroy()
{

	SAFE_DELETE(mResourceMgr);

}


void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		// do a screen shot when the TRIANGLE button is pressed
	{
		char s[80];
		sprintf(s, "ms0:/screenshot.png");
		JRenderer::GetInstance()->ScreenShot(s);
	}

	if (engine->GetButtonClick(PSP_CTRL_CROSS))			// exit when the CROSS button is pressed
	{
		engine->End();
		return;
	}

}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();

	renderer->EnableTextureFilter(false);	// turn off bilinear filtering

	// retrieve quad from the resource manager to render it directly
	renderer->RenderQuad(mResourceMgr->GetQuad("bg"), SCREEN_WIDTH_F/2, SCREEN_HEIGHT_F/2);
	
	float x;
	float y;

	JQuad* quad;

	quad = mResourceMgr->GetQuad("Dirt Block");		// render some platforms
	x = quad->mWidth/2;
	y = SCREEN_HEIGHT_F-quad->mHeight/2;
	for (int i=0;i<5;i++)
	{
		renderer->RenderQuad(quad, x, y);
		x += quad->mWidth;
	}

	y-=48;
	x = quad->mWidth*2.5f;
	quad = mResourceMgr->GetQuad("Grass Block");
	for (int i=0;i<5;i++)
	{
		renderer->RenderQuad(quad, x, y);
		x += quad->mWidth;
	}

	x = quad->mWidth*3.5f; 
	y-=43;
	renderer->RenderQuad(mResourceMgr->GetQuad("Wall Block"), x, y);

	renderer->RenderQuad(mResourceMgr->GetQuad("Character Cat Girl"), 152, 173);	// render the characters
	
	renderer->RenderQuad(mResourceMgr->GetQuad("Character Boy"), 257, 125);

	renderer->RenderQuad(mResourceMgr->GetQuad("Tree Tall"), 51, 160);


}


void GameApp::Pause()
{

}


void GameApp::Resume()
{

}
