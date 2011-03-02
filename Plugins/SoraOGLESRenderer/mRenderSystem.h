/*
 *  mRenderSystem.h
 *  OpenGLES Study 1
 *
 *  Created by DarkfallGriffin on 7/8/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

//Original renderSystem from CCGameBox(CocoaChina) by dr_waston

#ifndef _MRENDERSYSTEM_H_
#define _MRENDERSYSTEM_H_

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "mSprite.h"
#include "SoraCore.h"

namespace sora {
#define MAX_VERTEX_BUFFER		4096

enum mViewMode {
	mViewModeNormal, 
	mViewModeLandscapeLeft, 
	mViewModeLandscapeRight
};

enum mImageFadeMode {
	mImageFadeIn,
	mImageFadeOut,
};

enum mScreenShiftMode {
	mScreenShiftVerticle,
	mScreenShiftHorzion,
	mScreenShiftNone,
};

class mRenderSystem {
protected:
	mRenderSystem();
	~mRenderSystem() {}

	void initSystem();
	void shutdownSystem() {}

public:
	static mRenderSystem* getInstance();
	static void destroy();
	
	//设置设备显示模式
	//mViewModeNormal->普通, 设备默认
	//mViewModeLandscapeLeft->普通状态下左旋转90度
	//mViewModeLandscapeRight->普通状态下右旋转90度
	void		setViewMode(mViewMode viewMode);
	mViewMode	getViewMode()						{return orientation; }
	
	//切换到2D渲染模式 **会把缓冲区内容渲染到屏幕
	void switchTo2D();
	
	//切换到3D渲染模式 **会把缓冲区内容渲染到屏幕
	void switchTo3D();

	//绑定一个贴图到OpenGL ES
	//texture->要绑定的贴图
	void bindTexture(SoraTexture* texture);

	//创建一个贴图
	//filename->贴图名称
	//directory->贴图在程序跟目录下的路径,一般为空
	mTexture* loadTexture(const char* filename, const char* directory=NULL);
	
	//创建一个PVR压缩贴图
	//filename->贴图名称
	//directory->贴图在程序跟目录下的路径,一般为空
	mTexture* loadPVRTexture(const char* filename, const char* directory=NULL);
	
	//创建一张空贴图
	//width->要创建的贴图宽度
	//heigth->要创建的贴图高度
	mTexture* createTexture(int width, int height);

	//把缓冲区内容刷新到屏幕
	//renderBegin和renderEnd以及渲染模式切换(2D,3D)时会自动调用
	void flush();

	//渲染一个图像
	//quad->要渲染的图像
	//angle->偏转角度
	//xScale->横向缩放比例
	//yScale->纵向缩放比例
	//vertex2D->图形位置矩阵
	void renderImage(mSprite* quad);
	void renderImage(mSprite* quad, float x, float y, float angle=0.0f);
	void renderImage(mSprite* quad, float x, float y, float xScale ,float yScale, float angle=0.0f);
	void renderImageBlur(mSprite* quad, float x, float y, int times, float increment);

	//设置整体界面颜色和透明度
	//r->红色
	//g->绿色
	//b->蓝色
	//a->透明度
	//参数取值范围(0..1)
	void setColor(float r, float g, float b, float a);
	
	//设置整体透明度
	//a->透明度
	//参数取值范围(0..1)
	void setAlpha(float a);
	
	//整体淡入/淡出
	//frame->消耗帧数
	//mode->方式,淡入或者淡出
//	void fade(size_t frame, mViewFadeMode mode);
//	mViewFadeMode getFadeMode()							{return fadeMode; }
	
	void shift(size_t frame, float max, mScreenShiftMode mode);
	
	bool isShifting() { if(shiftMode != mScreenShiftNone) return true; return false; }
	//设置颜色混合方式
	
	///////////////////////////////////////////////////////////////////////
	// 	if (flag)														 //
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);							 //
	//	else															 //
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);           //
	///////////////////////////////////////////////////////////////////////
	
	/*
	 OpenGL会把源颜色和目标颜色各自取出，并乘以一个系数（源颜色乘以的系数称为“源因子”，目标颜色乘以的系数称为“目标因子”），
	 然后相加，这样就得到了新的颜色。(减、取两者中较大的、取两者中较小的、逻辑运算等)
	 下面用数学公式来表达一下这个运算方式。
	 假设源颜色的四个分量（指红色，绿色，蓝色，alpha值）是(Rs, Gs, Bs, As)，
	 目标颜色的四个分量是(Rd, Gd, Bd, Ad)，又设源因子为(Sr, Sg, Sb, Sa)，
	 目标因子为(Dr, Dg, Db, Da)。
	 则混合产生的新颜色可以表示为：
	 (Rs*Sr+Rd*Dr, Gs*Sg+Gd*Dg, Bs*Sb+Bd*Db, As*Sa+Ad*Da)
	 如果颜色的某一分量超过了1.0，则它会被自动截取为1.0，不需要考虑越界的问题。
	 
	 源因子和目标因子是可以通过glBlendFunc函数来进行设置的。
	 glBlendFunc有两个参数，前者表示源因子，后者表示目标因子。这两个参数可以是多种值.
	 GL_ZERO：					表示使用0.0作为因子，实际上相当于不使用这种颜色参与混合运算。
	 GL_ONE：					表示使用1.0作为因子，实际上相当于完全的使用了这种颜色参与混合运算。
	 GL_SRC_ALPHA：				表示使用源颜色的alpha值来作为因子。
	 GL_DST_ALPHA：				表示使用目标颜色的alpha值来作为因子。
	 GL_ONE_MINUS_SRC_ALPHA：	表示用1.0减去源颜色的alpha值来作为因子。
	 GL_ONE_MINUS_DST_ALPHA：	表示用1.0减去目标颜色的alpha值来作为因子。
	*/
	void enableAddictiveDraw(bool flag);
	
	
	//设置绑定贴图时的纹理过滤方式
	//flag->是否允许线性过滤
	
	//////////////////////////////////////////////////////////////////////////////////////
	//	if (mLinearFiltering) {															//
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			//
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			//
	//	}																				//
	//	else {																			//
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);			//
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);			//
	//	}																				//
	//////////////////////////////////////////////////////////////////////////////////////
	
	/*
	 glTexParameteri()纹理过滤函数.
	 图象从纹理图象空间映射到帧缓冲图象空间(映射需要重新构造纹理图像,这样就会造成应用到多边形上的图像失真),
	 这时就可用glTexParmeteri()函数来确定如何把纹理象素映射成像素.
	 部分参数功能说明如下:
	 GL_TEXTURE_2D:				操作2D纹理.
	 GL_TEXTURE_MAG_FILTER:		放大过滤
	 GL_LINEAR:					线性过滤, 使用距离当前渲染像素中心最近的4个纹素加权平均值.
	*/
	void enableLinearFiltering(bool flag);
	
	//设置是否允许贴图颜色混合
	//flag->允许贴图颜色混合/不允许贴图颜色混合
	//只有在允许贴图颜色混合的情况下sprite类里的颜色才会被启用
	void enableColorPointer(bool flag);

	//通知准备开始渲染
	void renderBegin();
	
	//通知渲染结束,把缓冲区刷新到屏幕
	void renderEnd();

private:
	static mRenderSystem* mInstance;
	GLuint mCurrentTex;
	bool mLinearFiltering;
	bool mUseColorPointer;

	float mScreenWidth;
	float mScreenHeight;
		
	mViewMode orientation;
	
	mScreenShiftMode shiftMode;
	float shiftx, shifty;
	float shiftSpeed;
	bool up;
	float shiftMax;
	size_t of, cf;
};
	
} // namespace sora

#endif