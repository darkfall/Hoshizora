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
// Adapted from "tutorial05" of HGE. To use HGE's hepler classes, you need to include
// libhgetools in the PSP makefile.
//
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>

#include <hge/hgedistort.h>
#include <hge/hgefont.h>

#include "GameApp.h"


//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{
	mTex = NULL;
	mDistortionMesh = NULL;
	mFont = NULL;

 	mRows=8;
 	mCols=8;
 	mCellw=(float)(TEXTURE_WIDTH/(mCols-1));
 	mCellh=(float)(TEXTURE_HEIGHT/(mRows-1));
 	mMeshx=(SCREEN_WIDTH_F-TEXTURE_WIDTH)/2;
 	mMeshy=(SCREEN_HEIGHT_F-TEXTURE_HEIGHT)/2;
}


//-------------------------------------------------------------------------------------
// Destructor.
//
//-------------------------------------------------------------------------------------
GameApp::~GameApp()
{

}


//-------------------------------------------------------------------------------------
// This is the init callback function. You should load and create your in-game 
// resources here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Create()
{

	JRenderer* renderer = JRenderer::GetInstance();		
	mTex=renderer->LoadTexture("texture.jpg");

	// Create a distortion mesh
	mDistortionMesh=new hgeDistortionMesh(mCols, mRows);
	mDistortionMesh->SetTexture(mTex);
	mDistortionMesh->SetTextureRect(0,0,TEXTURE_WIDTH,TEXTURE_HEIGHT);
	mDistortionMesh->Clear(ARGB(0xFF,0xFF,0xFF,0xFF));

	// Load a font
	mFont=new hgeFont("font1.fnt");
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{

	SAFE_DELETE(mTex);

	SAFE_DELETE(mDistortionMesh);

	SAFE_DELETE(mFont);

}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	// do a screen shot when the TRIANGLE button is pressed
	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		
	{
		char s[80];

		// save screen shot to root of Memory Stick 
		sprintf(s, "ms0:/screenshot.png");				
		JRenderer::GetInstance()->ScreenShot(s);
	}

	// exit when the CROSS button is pressed
	if (engine->GetButtonClick(PSP_CTRL_CROSS))	
	{
		engine->End();
		return;
	}

	float dt = engine->GetDelta();		// Get time elapsed since last update.

	static float t=0.0f;
	static int trans=2;

	int i, j, col;
	float r, a, dx, dy;

	t+=dt;

	if (engine->GetButtonClick(PSP_CTRL_CIRCLE))	
	{
		if(++trans > 2) 
			trans=0;
		mDistortionMesh->Clear(ARGB(0xFF,0xFF,0xFF,0xFF));
	
	}
	
	// Calculate new displacements and coloring for one of the three effects
	switch(trans)
	{
		case 0:	
			for(i=1;i<mRows-1;i++)
			{
				for(j=1;j<mCols-1;j++)
				{
					mDistortionMesh->SetDisplacement(j,i,cosf(t*10+(i+j)/2)*5,sinf(t*10+(i+j)/2)*5,HGEDISP_NODE);
				}
			}
			break;

		case 1:	
			for(i=0;i<mRows;i++)
			{
				for(j=1;j<mCols-1;j++)
				{
					mDistortionMesh->SetDisplacement(j,i,cosf(t*5+j/2)*15,0,HGEDISP_NODE);
					col=int((cosf(t*5+(i+j)/2)+1)*35);

					// colour doesn't work the same as the original HGE version so the following line is commented out.
					// dis->SetColor(j,i,ARGB(0xFF,col,col,col));
				}
			}
			break;

		case 2:	
			for(i=0;i<mRows;i++)
			{
				for(j=0;j<mCols;j++)
				{
					r=sqrtf(powf(j-(float)mCols/2,2)+powf(i-(float)mRows/2,2));
					a=r*cosf(t*2)*0.1f;
					dx=sinf(a)*(i*mCellh-TEXTURE_HEIGHT/2)+cosf(a)*(j*mCellw-TEXTURE_WIDTH/2);
					dy=cosf(a)*(i*mCellh-TEXTURE_HEIGHT/2)-sinf(a)*(j*mCellw-TEXTURE_WIDTH/2);
					mDistortionMesh->SetDisplacement(j,i,dx,dy,HGEDISP_CENTER);
					col=int((cos(r+t*4)+1)*40);

					// colour doesn't work the same as the original HGE version so the following line is commented out.
					// dis->SetColor(j,i,ARGB(0xFF, col,(col/2), 0));
				}
			}
			break;
	}


}


//-------------------------------------------------------------------------------------
// All rendering operations should be done in Render() only.
// 
//-------------------------------------------------------------------------------------
void GameApp::Render()
{

	// get JRenderer instance
	JRenderer* renderer = JRenderer::GetInstance();		

	// clear screen to black
	renderer->ClearScreen(ARGB(0,0,0,0));

	// render the mesh
 	mDistortionMesh->Render(mMeshx, mMeshy);
 
 	mFont->printf(5, 5, HGETEXT_LEFT, "Press\nCIRCLE!");

}


//-------------------------------------------------------------------------------------
// This function is called when the system wants to pause the game. You can set a flag
// here to stop the update loop and audio playback.
//
//-------------------------------------------------------------------------------------
void GameApp::Pause()
{
	
}


//-------------------------------------------------------------------------------------
// This function is called when the game returns from the pause state.
//
//-------------------------------------------------------------------------------------
void GameApp::Resume()
{
	
}
