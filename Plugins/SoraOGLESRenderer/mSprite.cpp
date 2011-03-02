/*
 *  sprite.cpp
 *  OpenGLES Study 1
 *
 *  Created by DarkfallGriffin on 7/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "mSprite.h"
#include "Wrapper.h"

namespace sora  {


mSprite::mSprite(SoraTexture* tex, float x, float y, float width, float height) {
	texture = tex;
	mX = x;
	mY = y;
	cX = x;
	cY = y;
	mWidth = width;
	mHeight = height;
	
	mColor = COLOR_WHITE;
	
	centerX = 0;
	centerY = 0;
	
	mHFlipped = false;
	mVFlipped = false;
	
	mXOffset = mYOffset = 0;
	
	setTextureRect(x, y, width, height);
	
	isInitWithTexturePath = false;
	
	xScale = 1.f;
	yScale = 1.f;
		
	degree = 0;
}

mSprite::mSprite(SoraTexture* tex) {
	texture = tex;
	mX = 0;
	mY = 0;
	cX = 0;
	cY = 0;
	mWidth = texture->mTextureWidth;
	mHeight = texture->mTextureHeight;
	
	
	mColor = COLOR_WHITE;


	centerX = 0;
	centerY = 0;
	
	mHFlipped = false;
	mVFlipped = false;
	
	mXOffset = mYOffset = 0;
	
	setTextureRect(0, 0, texture->mTextureWidth, texture->mTextureHeight);
	
	isInitWithTexturePath = false;
	
	xScale = 1.f;
	yScale = 1.f;
		
	degree = 0;
}

mSprite::mSprite(const char* sprPath) {
	texture = mTextureCreate(sprPath, false);
	

//	if(texture) {
		mX = 0;
		mY = 0;
		cX = 0;
		cY = 0;
		mWidth = texture->mTextureWidth;
		mHeight = texture->mTextureHeight;
	
	mColor = COLOR_WHITE;
	
		centerX = 0;
		centerY = 0;
	
		mHFlipped = false;
		mVFlipped = false;
	
		mXOffset = mYOffset = 0;
	
		setTextureRect(0, 0, texture->mTextureWidth, texture->mTextureHeight);
	
		isInitWithTexturePath = true;
	
		xScale = 1.f;
		yScale = 1.f;
	
		degree = 0;
}

mSprite::~mSprite() {
	if(isInitWithTexturePath)
		delete texture;
}

void mSprite::setTextureRect(float x, float y, float w, float h) 
{ 
	mX = x;
	mY = y;
	mWidth = w;
	mHeight = h;
	
//	centerX = x;
//	centerY = y;
	
	mTX0 = x/texture->mTextureWidth;
	mTX1 = (x+w)/texture->mTextureWidth;
	
	mTY0 = y/texture->mTextureHeight;
	mTY1 = (y+h)/texture->mTextureHeight;
}

void mSprite::setColor(float r, float g, float b, float a) {
	mColor = COLOR_WHITE;
}

void mSprite::setColorWithoutAlpha(float r, float g, float b) {
	mColor.r = r;
	mColor.g = g;
	mColor.b = b;
}

void mSprite::setAlpha(float a) {
	setColor(1.f, 1.f, 1.f, a);
}

void mSprite::setCenter(float cx, float cy) {
	centerX = cx;
	centerY = cy;
}

void mSprite::update() {
}
	
	void mSprite::render(float32 xo, float32 yo, float32 angle) {
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		
		yo = 480 - yo;
		
		float width = this->mWidth*this->xScale;
		float height = this->mHeight*this->yScale;
		float x = xo - this->centerX*this->xScale;
		float y = yo + this->centerY*this->yScale;
		
		float xx, yy;
		float cosAngle = cosf(angle);
		float sinAngle = sinf(angle);
		
		GLfloat uvs[] = {
			this->mTX0, this->mTY0,
			this->mTX1, this->mTY0,
			this->mTX0, this->mTY1,
			this->mTX1, this->mTY1,
			this->mTX0, this->mTY1,
			this->mTX1, this->mTY0
		};
		
		
		if (this->mHFlipped) {
			uvs[ 0] = this->mTX1;
			uvs[ 2] = this->mTX0;
			uvs[ 4] = this->mTX1;
			uvs[ 6] = this->mTX0;
			uvs[ 8] = this->mTX1;
			uvs[10] = this->mTX0;
		}
		
		if (this->mVFlipped) {
			uvs[ 1] = this->mTY1;
			uvs[ 3] = this->mTY1;
			uvs[ 5] = this->mTY0;
			uvs[ 7] = this->mTY0;
			uvs[ 9] = this->mTY0;
			uvs[11] = this->mTY1;
		}
		
		GLfloat vertices[] = {
			x,			y,
			x+width,	y,
			x,			y-height,
			x+width,	y-height,
			0, 0,
			0, 0
		};
		
		int idx = 8;
		if (angle != 0.0f) {
			idx = 0;
			
			for (int i=0;i<4;i++) {
				xx = (cosAngle*(vertices[idx]-xo) - sinAngle*(vertices[idx+1]-yo) + xo);
				yy = (sinAngle*(vertices[idx]-xo) + cosAngle*(vertices[idx+1]-yo) + yo); 
				
				vertices[idx] = xx;
				vertices[idx+1] = yy;
				idx += 2;
			}
			
		}
		
		vertices[idx++] = vertices[4];
		vertices[idx++] = vertices[5];
		
		vertices[idx++] = vertices[2];
		vertices[idx++] = vertices[3];
		
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		//glColorPointer (4, GL_FLOAT, 0, COLOR_WHITE);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
	}

} 