/*
 *  sprite.h
 *  OpenGLES Study 1
 *
 *  Created by DarkfallGriffin on 7/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MSPRITE_H_
#define _MSPRITE_H_

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "SoraTexture.h"
#include "SoraCore.h"
#include "SoraColor.h"

namespace sora {


class mTexture;

class mSprite {
public:
	//构造一个精灵
	//tex->精灵贴图
	//(x,y)->精灵在贴图上的开始坐标
	//width->精灵宽度
	//height->精灵长度
	mSprite(SoraTexture* tex, float x, float y, float width, float height);
	mSprite(SoraTexture* tex);
	
	//构造一个精灵
	//sprPath->精灵名称, 相对路径
	mSprite(const char* sprPath);
	~mSprite();
	
	//更新精灵状态,如果存在特效,则是必须
	void update();
	
	void setDeleteTextureWhenRelease(bool flag)					{isInitWithTexturePath = flag; }
//	void render(float cx, float cy, float angel=0.0f);
//	void renderWithScale(float cx, float cy, float xScale=1.0f, float yScale=1.0f, float angel=0.0f);
	
	
	//设置精灵位置
	//在RenderSystem渲染精灵时如果使用未带指定渲染坐标的渲染函数则会使用此处的坐标渲染
	//默认为(0,0)
	void setPosition(float x, float y)							{ cX = x; cY = y;}
	
	void setScale(float xs, float ys)							{xScale = xs; yScale = ys; }
	//设置精灵颜色
	//r, g, b, a->红, 绿, 蓝, 透明度
	//只有当RenderSystem允许贴图颜色混合时才会有效
	void setColor(float r, float g, float b, float a);
	void setColorWithoutAlpha(float r, float g, float b);
	
	void setAlpha(float a);
	
	void setRotation(float rad) { degree = rad; }
	
	//设置贴图中心
	//(cx,cy)->坐标(相对精灵左上角)
	void setCenter(float cx, float cy);
	
	//设置贴图渲染范围
	//(x,y)->开始坐标(相对贴图左上角)
	//w->宽度
	//h->高度
	void setTextureRect(float x, float y, float w, float h);
	
	//获取精灵宽度和高度
	float getWidth() const {return mWidth; }
	float getHeight() const {return mHeight; }
	
	//设置是否允许横向/纵向翻转
	//hFlip->横向翻转
	void setHFlip(bool hFlip) {mHFlipped = hFlip; }
	//vFlip->纵向翻转
	void setVFlip(bool vFlip) {mVFlipped = vFlip; }
	
	void render(float32 x, float32 y, float32 angle);
	
	
	bool mHFlipped;
	bool mVFlipped;
	
	float cX;
	float cY;
	
	float xScale;
	float yScale;
	
	float mX;
	float mY;
	float mWidth;
	float mHeight;
	
	float mTX0;
	float mTY0;
	float mTX1;
	float mTY1;
	
	SoraColorRGBA mColor;
	
	float centerX;
	float centerY;
	
	float mXOffset;
	float mYOffset;
	
	float degree;
	
	SoraTexture *texture;	
	bool isInitWithTexturePath;
};

} // namespace sora
#endif