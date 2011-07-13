/*
 *  SoraGifSprite.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGifSprite.h"
#include "gif_lib.h"

#include "SoraResourceFile.h"
#include "SoraCore.h"

#ifdef OS_WIN32
#pragma comment(lib, "giflib.lib")
#endif

namespace sora {
    
    static int InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
    static int InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
    
    SoraGifSprite::SoraGifSprite() {
        init();
    }
    
    SoraGifSprite::SoraGifSprite(const SoraWString& path) {
        init();
        
        load(path);
    }
    
    SoraGifSprite::~SoraGifSprite() {
        clear();
    }
    
    void SoraGifSprite::init() {
        mFrameRate = 24;
        mCurrFrame = 0;
        mFrameCount = 0;
    }

    static int image_gif_read(GifFileType* ft, GifByteType* buf, int size) {
        SoraMemoryBuffer* memBuffer = (SoraMemoryBuffer*)ft->UserData;
        if(memBuffer) {
            ulong32 rsize = size;
            if(memBuffer->read(buf, rsize) && rsize == size)
                return size;
            return 0;
        }
        return 0;
    }
    
    void SoraGifSprite::clear() {
        GIF_FRAME_CONT::iterator itFrame = mFrames.begin();
        while(itFrame != mFrames.end()) {
            SORA->releaseTexture(itFrame->mFrameTex);
            ++itFrame;
        }
        mFrames.clear();
    }
    
    bool SoraGifSprite::load(const SoraWString& path) {
        GifRecordType RecordType;
        GifByteType *Extension;
        GifFileType *GifFileIn = NULL;
        ColorMapObject *palette;
        
#ifndef OS_WIN32
#define gif_color32(c) CARGB(255,palette->Colors[c].Blue, palette->Colors[c].Green, palette->Colors[c].Red)
#else
#define gif_color32(c) CARGB(255, palette->Colors[c].Red, palette->Colors[c].Green, palette->Colors[c].Blue)
#endif
        int ExtCode;
       
        SoraResourceFileAuto resourceFile(path);
        if(!resourceFile.isValid())
            return false;
        
        SoraMemoryBuffer memBuffer(static_cast<void*>(resourceFile), resourceFile.getSize());
        if ((GifFileIn = DGifOpen(&memBuffer, image_gif_read)) == NULL)
            return false;
        
        clear();
        
        do {
            if (DGifGetRecordType(GifFileIn, &RecordType) == GIF_ERROR) {
                DGifCloseFile(GifFileIn);
                return false;
            }
            
            switch (RecordType) {
                case IMAGE_DESC_RECORD_TYPE: {	
                    GifFrameNode node;
                    
					if (DGifGetImageDesc(GifFileIn) == GIF_ERROR) {
						DGifCloseFile(GifFileIn);
						return false;
					}
					if((palette = (GifFileIn->Image.ColorMap != NULL) ? GifFileIn->Image.ColorMap : GifFileIn->SColorMap) == NULL) {
						DGifCloseFile(GifFileIn);
						return false;
					}
                    					
                    node.mFrameWidth = GifFileIn->Image.Width;
					node.mFrameHeight = GifFileIn->Image.Height;
					
                    uint32 bgColor = gif_color32(GifFileIn->SBackGroundColor);

                    GifRowType ScreenBuffer;
                    if ((ScreenBuffer = (GifRowType)malloc(node.mFrameWidth * node.mFrameHeight * sizeof(GifRowType*))) == NULL) {
						DGifCloseFile(GifFileIn);
						return false;
					}

					int32 size = node.mFrameWidth * node.mFrameHeight * sizeof(uint32);
					uint32* bits = (uint32*)malloc(size);
					uint32* buffer = bits;
                    
                    if (GifFileIn->Image.Interlace) {
                        /* Need to perform 4 passes on the images: */
                        for (int i = 0; i < 4; i++) {
                            for (int j = GifFileIn->Image.Top + InterlacedOffset[i]; 
                                 j < GifFileIn->Image.Top + GifFileIn->Image.Height;
                                 j += InterlacedJumps[i]) {
            
                                if (DGifGetLine(GifFileIn, &ScreenBuffer[j * node.mFrameWidth], GifFileIn->Image.Width) == GIF_ERROR) {
                                    free(bits);
                                    DGifCloseFile(GifFileIn);
                                    return false;
                                }
                            }
                        }
                    } else {
                        for (int i = 0; i < GifFileIn->Image.Height; i++) {
                            if (DGifGetLine(GifFileIn, &ScreenBuffer[i * node.mFrameWidth], GifFileIn->Image.Width) == GIF_ERROR) {
                                free(bits);
                                DGifCloseFile(GifFileIn);
                                return false;
                            }
                        }
                    }
                    
                    for (int i=0; i<GifFileIn->Image.Height; ++i) {
                        for (int j=0; j<GifFileIn->Image.Width; ++j) {
                            *buffer = gif_color32(ScreenBuffer[(int)(i * GifFileIn->Image.Width + j)]);
                            if(*buffer == bgColor)
                                *buffer = 0x00000000;
                            buffer++;
                        }
                    }
                    
                    free(ScreenBuffer);
                    
                    HSORATEXTURE frameTex = SORA->createTextureFromRawData(bits, node.mFrameWidth, node.mFrameHeight);
                    if(frameTex)
                        node.mFrameTex = frameTex;
                    else {
                        free(bits);
                        DGifCloseFile(GifFileIn);
                        return false;
                    }
                    
                    free(bits);
                    
                    mFrames.push_back(node);
					break;
				}
                case EXTENSION_RECORD_TYPE:
                    if (DGifGetExtension(GifFileIn, &ExtCode, &Extension) == GIF_ERROR) {
                        DGifCloseFile(GifFileIn);
                        return false;
                    }
                    while (Extension != NULL) {
                        if (DGifGetExtensionNext(GifFileIn, &Extension) == GIF_ERROR) {
                            DGifCloseFile(GifFileIn);
                            return false;
                        }
                    }
                    break;
                case TERMINATE_RECORD_TYPE:
                    break;
                default:
                    break;
            }
        }
        while (RecordType != TERMINATE_RECORD_TYPE);

        DGifCloseFile(GifFileIn);
        
        if(mFrames.size() == 0)
            return false;
        
        setFrame(0);
        
        return true;
    }
    
    void SoraGifSprite::setFrameRate(int32 frameRate) {
        mFrameRate = frameRate;
        mFrameInterval = 1.f / frameRate;
        mFrameCount = 0;
    }
    
    int32 SoraGifSprite::getFrameRate() const {
        return mFrameRate;
    }
    
    uint32 SoraGifSprite::update(float32 dt) {
        SoraSprite::update(dt);
        
        if(mFrames.size() == 0)
            return 0;
        
        mFrameCount += dt;
        if(mFrameCount >= mFrameInterval) {
            toNextFrame();
            mFrameCount = 0.f;
        }
        
        return 0;
    }
    
    void SoraGifSprite::setFrame(int32 frameIndex) {
        if(frameIndex < 0)
            frameIndex = mFrames.size() - 1;
        else if(frameIndex >= mFrames.size())
            frameIndex = 0;
        
        mCurrFrame = frameIndex;
        setTexture(mFrames[mCurrFrame].mFrameTex);
    }
    
    void SoraGifSprite::toNextFrame() {
        setFrame(mCurrFrame+1);
    }
    
    void SoraGifSprite::toPrevFrame() {
        setFrame(mCurrFrame-1);
    }
    
    void SoraGifSprite::toFirstFrame() {
        setFrame(0);
    }
    
    void SoraGifSprite::toLastFrame() {
        setFrame(mFrames.size()-1);
    }
    
    int32 SoraGifSprite::getFrameSize() const {
        return mFrames.size();
    }
    
    int32 SoraGifSprite::getCurrFrame() const {
        return mCurrFrame;
    }

} // namespace sora