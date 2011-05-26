/*
 *  SoraPNGOptimizer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraPNGOptimizer.h"
#include "SoraFileUtility.h"

#include "PNGWritter.h"
#include "RectPlacement.h"

namespace sora {
	
	struct color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
	
	bool SoraPNGOptimizer::optimizePNGTex(HSORATEXTURE tex) {
		int32 width = sora::SORA->getTextureWidth(tex, true);
		int32 height = sora::SORA->getTextureHeight(tex, true);
		int32 pitch = sora::SORA->getTextureWidth(tex, false);
		
		uint32* col = sora::SORA->textureLock(tex);
		if(col) {
			optimizePNGData(col, width, height, pitch);
			sora::SORA->textureUnlock(tex);
			return true;
		}
		return false;
	}
	
	void SoraPNGOptimizer::optimizePNGData(uint32* data, int32 width, int32 height, int32 pitch) {
		color* buf = (color*)data;
		if(buf) {
			for(int i=0; i<height; i++) {
				for(int j=0; j<width; j++) {
					if(!buf[i*pitch+j].a) {
						int count = 0;
						int r = 0, g = 0, b = 0;
						
						for(int k=-1; k<=1; k++) {
							for(int l=-1; l<=1; l++) {
								if(i+k >= 0 && i+k < height &&
								   j+l >= 0 && j+l < width &&
								   buf[(i+k)*pitch + (j+l)].a) {
									r += buf[(i+k)*pitch + (j+l)].r;
									g += buf[(i+k)*pitch + (j+l)].g;
									b += buf[(i+k)*pitch + (j+l)].b;
									count++;
								}
							} 
						}
						
						if(count) {
							buf[i*pitch+j].r = (unsigned char)(r / count);
							buf[i*pitch+j].g = (unsigned char)(g / count);
							buf[i*pitch+j].b = (unsigned char)(b / count);
						}
					}
				}
			}
		}
	}
	
	bool SoraPNGOptimizer::optimizePNGFromAndWriteToFile(const SoraWString& file, const SoraWString& output) {
		if(SoraFileUtility::fileExists(file)) {
			HSORATEXTURE tex = sora::SORA->createTexture(file, false);
			if(tex) {
				int32 width = sora::SORA->getTextureWidth(tex, true);
				int32 height = sora::SORA->getTextureHeight(tex, true);
				int32 pitch = sora::SORA->getTextureWidth(tex, false);
				
				uint32* col = sora::SORA->textureLock(tex);
				if(col) {
					optimizePNGData(col, width, height, pitch);
					
					FILE* pf = sora_fopenw(output, "wb");
					if(!pf) {
						sora::SORA->textureUnlock(tex);
						sora::SORA->releaseTexture(tex);
						return false;
					}

#ifdef OS_WIN32
					bool result = writePNGWithPitch(pf, col, width, height, pitch, true, true);
#else 
					bool result = writePNGWithPitch(pf, col, width, height, pitch, false, true);
#endif
					
					fclose(pf);
					sora::SORA->textureUnlock(tex);
					sora::SORA->releaseTexture(tex);
					return result;
				}
			} else {
				sora::SORA->messageBoxW(L"Error open file: "+file, L"Error", MB_OK | MB_ICONERROR);
			}
		} else {
			std::vector<std::wstring> filecont;
			sora::SORA->enumFilesInFolder(filecont, file);
			if(filecont.size() != 0) {
				for(size_t i=0; i<filecont.size(); ++i) {
					std::wstring op = sora::SoraFileUtility::getFileName(filecont[i]);
					size_t rppos = op.rfind(L".");
					if(rppos != std::wstring::npos)
						op.insert(rppos, L"_optd");
					else
						op += L"_optd.png";
					optimizePNGFromAndWriteToFile(filecont[i], op);
				}
				return true;
			}
		}
		return false;
	}
	
	bool SoraPNGOptimizer::optimizeTexAndWriteToFile(HSORATEXTURE tex, const SoraWString& output) {
		int32 width = sora::SORA->getTextureWidth(tex, true);
		int32 height = sora::SORA->getTextureHeight(tex, true);
		int32 pitch = sora::SORA->getTextureWidth(tex, false);
		
		uint32* col = sora::SORA->textureLock(tex);
		if(col) {
			optimizePNGData(col, width, height, pitch);
			
			FILE* pf = sora_fopenw(output, "wb");
			if(!pf) {
				sora::SORA->textureUnlock(tex);
				sora::SORA->releaseTexture(tex);
				return false;
			}
			
#ifdef OS_WIN32
			bool result = writePNGWithPitch(pf, col, width, height, pitch, true, true);
#else 
			bool result = writePNGWithPitch(pf, col, width, height, pitch, false, true);
#endif
			
			fclose(pf);
			sora::SORA->textureUnlock(tex);
			sora::SORA->releaseTexture(tex);
			return result;
		}
		return false;
	}
	
	bool SoraPNGOptimizer::writeTexToFile(HSORATEXTURE tex, const SoraWString& output) {
		int32 width = sora::SORA->getTextureWidth(tex, true);
		int32 height = sora::SORA->getTextureHeight(tex, true);
		int32 pitch = sora::SORA->getTextureWidth(tex, false);
		
		uint32* col = sora::SORA->textureLock(tex);
		if(col) {			
			FILE* pf = sora_fopenw(output, "wb");
			if(!pf) {
				sora::SORA->textureUnlock(tex);
				sora::SORA->releaseTexture(tex);
				return false;
			}
			
#ifdef OS_WIN32
			bool result = writePNGWithPitch(pf, col, width, height, pitch, true, true);
#else 
			bool result = writePNGWithPitch(pf, col, width, height, pitch, false, true);
#endif
			
			fclose(pf);
			sora::SORA->textureUnlock(tex);
			sora::SORA->releaseTexture(tex);
			return result;
		}
		return false;
	}
} // namespace sora