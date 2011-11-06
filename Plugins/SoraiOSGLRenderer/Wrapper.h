/*
 *  Wrapper.h
 *  OpenGLES Study 1
 *
 *  Created by DarkfallGriffin on 7/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _GLES_WRAPPER_H_
#define _GLES_WRAPPER_H_

#include <iostream>
#include <string>

#include "SoraTexture.h"

namespace sora {

//创建普通贴图
//fileName->贴图名称
//isFullPath->贴图名称是否为绝对路径
SoraTexture *mTextureCreate(const char* fileName, bool isFullPath=false);
SoraTexture *mTextureCreateWithData(void* pData, uint32 size);
SoraTexture *mTextureCreateWithRawData(unsigned int* pData, uint32 width, uint32 height);

//创建PVR压缩贴图
//fileName->贴图名称
SoraTexture *mPVRTextureCreate(const char* fileName);

//创建字符串贴图
//_str->要创建贴图的字符串
//_fontName->字符串字体
//w->贴图宽度(没有则自适应)
//h->贴图宽度(没有则自适应)
//_fontSize->字体大小
SoraTexture *mStringTextureCreate(const char* _str, const char* _fontName, float w, float h, float _fontSize);
SoraTexture *mStringTextureCreate(const char* _str, const char* _fontName, float _fontSize);

//创建英文ASCII表贴图
//fontName->字体名称
//w->贴图宽度
//h->贴图高度
//size->字体大小
//charWidths->创建完毕后会被设置为各个字符的宽度
//actualHeight->创建完毕后会被设置为各个字符的高度
SoraTexture *mFontTextureCreate(const char* fontName, float w, float h, float size, float *charWidths, float *actualHeight);

	std::string getPath(const char* s, const char* directory=0);
	
} // namespace sora


#endif



	