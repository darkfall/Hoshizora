//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include <JApp.h>

class JTexture;
class hgeDistortionMesh;
class hgeFont;

#define TEXTURE_WIDTH		256
#define TEXTURE_HEIGHT		256

class GameApp:	public JApp
{
	
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	virtual void Resume();

private:
 	JTexture* mTex;

 	hgeDistortionMesh* mDistortionMesh;
 	hgeFont* mFont;

 	int mRows;
 	int mCols;
 	float mCellw;
 	float mCellh;

 	float mMeshx;
 	float mMeshy;

};

#endif
