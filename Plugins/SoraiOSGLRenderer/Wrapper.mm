
#import "Wrapper.h"
#import "Apple/Texture2D.h"
#import "Apple/PVRTexture.h"

#include "SoraCore.h"

//#include "soil/SOIL.h"

namespace sora {
	
	std::string getPath(const char* s, const char* directory) {
		std::string fullPath(s);
		std::string sDir;
		if(fullPath.find('/') != std::string::npos) {
			if(!directory) sDir = fullPath.substr(0, fullPath.rfind('/'));
			else {
				sDir = directory;
				sDir += "/";
				sDir += fullPath.substr(0, fullPath.rfind('/'));
			}
			
			fullPath = fullPath.substr(fullPath.rfind('/')+1, fullPath.size());
		} else if(directory) {
			sDir = directory;
		}
	
		NSString *dir = nil;
		if(sDir.size() != 0)
			dir = [[NSString alloc] initWithUTF8String:sDir.c_str()];
		
		NSString *name = [[NSString alloc] initWithUTF8String:fullPath.c_str()];
		if(name) {
			NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:nil inDirectory:dir];
			
			if(path != nil) {
				std::string buffer([path UTF8String]);
				[name release];
				[dir  release];
				//	[path release];
				return buffer;
			}
		}
		
		[name release];
		[dir  release];
		//	[path release];
		
		return "\0";
	}
	
	
	std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
	NSString* string2NSString(const char* str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str];
		return buffer;
	}
	
	NSString *applicationDocumentsDirectory() {
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		return basePath;
	}
	
	std::string getPathInAppDocDir(const std::string& file) {
		std::string buffer(NSString2String(applicationDocumentsDirectory()));
		buffer += "/";
		buffer += file;
		return buffer;
	}
	

    SoraTexture *mTextureCreate(const char* fileName, bool isFullPath) {
        std::string fullPath;
        if(!isFullPath) {
            fullPath = fileName;
            if(fullPath.find('/') != std::string::npos) {
                std::string sDir = fullPath.substr(0, fullPath.rfind('/'));
                std::string sName = fullPath.substr(fullPath.rfind('/')+1, fullPath.size());
                
                fullPath = getPath(sName.c_str(), sDir.c_str());
            }
            else fullPath = getPath(fileName);
        }
        else fullPath=fileName;
        
        printf("%s, %s\n", fileName, fullPath.c_str());
        if(fullPath.size() == 0)
            return 0;
		
        NSString *name = [[NSString alloc] initWithUTF8String:fullPath.c_str()];
		
        Texture2D *tex = [[Texture2D alloc] initWithImagePath: name];
        if(tex) {
            SoraTexture *texture = new SoraTexture([tex name], tex.contentSize.width, tex.contentSize.height,
                                                   tex.pixelsWide, tex.pixelsHigh);
            [tex release];
            [name release];
            
            return texture;
            
        } else {
            [name release];
            return 0;
        }
    }

	SoraTexture *mTextureCreateWithData(void* pData, ulong32 size) {
		NSData* data = [[NSData alloc] initWithBytes:pData length:size];
		UIImage* image = [[UIImage alloc] initWithData: data];
		SoraTexture *texture = NULL;
		if(image) {
			Texture2D* tex = [[Texture2D alloc] initWithImage:image];
			if(tex) {
				texture = new SoraTexture([tex name], tex.contentSize.width, tex.contentSize.height,
													   tex.pixelsWide, tex.pixelsHigh);
				
				[tex release];
			}
			[image release];
		}
		[data release];
		return texture;
	}
	
	SoraTexture *mTextureCreateWithRawData(unsigned int* pData, uint32 width, uint32 height) {
		CGSize size;
		size.width = width;
		size.height = height;
		Texture2D* tex = [[Texture2D alloc] initWithData:pData pixelFormat:kTexture2DPixelFormat_RGBA8888 
											  pixelsWide:width 
											  pixelsHigh:height contentSize:size];
		if(tex) {
			SoraTexture* texture = new SoraTexture([tex name], tex.contentSize.width, tex.contentSize.height,
									  tex.pixelsWide, tex.pixelsHigh);
			
			[tex release];
			return texture;
		}
		return 0;
	}
						  
    SoraTexture *mPVRTextureCreate(const char* fileName) {
        NSString* path = [[NSString alloc] initWithUTF8String:fileName];
        
        PVRTexture *pvrTexture = [PVRTexture pvrTextureWithContentsOfFile:path];
        if(pvrTexture) {
            SoraTexture* tex = new SoraTexture([pvrTexture name], [pvrTexture width], [pvrTexture height],
                                               [pvrTexture width], [pvrTexture height]);
            return tex;
        }
        return nil;
    }
	
    
    SoraTexture *mFontTextureCreate(const char* fontName, float w, float h, float size, float *charWidths, float *actualHeight) {
        NSString* fn = [[NSString alloc] initWithUTF8String:fontName];
        Texture2D *tex = [[Texture2D alloc] makeASCIIBitmapFont:CGSizeMake(w, h) 
                                                       fontName:fn 
                                                       fontSize:size 
                                                 charWidthArray:charWidths 
                                                     fontHeight:actualHeight];
        
        SoraTexture *texture = new SoraTexture([tex name], 
                                               tex.contentSize.width, 
                                               tex.contentSize.height, 
                                               tex.pixelsWide, 
                                               tex.pixelsHigh);
        
        [tex release];
        
        return texture;
        
    }
    
    SoraTexture *mStringTextureCreate(const char* _str, const char* _fontName, float w, float h, float _fontSize) {
        NSString *str = [[NSString alloc] initWithUTF8String:_str];
        NSString *fn  = [[NSString alloc] initWithUTF8String:_fontName];
        Texture2D *tex = [[Texture2D alloc] initWithString:str dimensions:CGSizeMake(w, h)
                                                 alignment:UITextAlignmentLeft fontName:fn fontSize:_fontSize];
        
        SoraTexture *texture = new SoraTexture([tex name], tex.contentSize.width, tex.contentSize.height,
                                               tex.pixelsWide, tex.pixelsHigh);
        
        [tex release];
        //	[str release];
        //	[fn  release];
        
        return texture;
    }
    
    SoraTexture *mStringTextureCreate(const char* _str, const char* _fontName, float _fontSize) {
        NSString *str = [[NSString alloc] initWithUTF8String:_str];
        NSString *fn  = [[NSString alloc] initWithUTF8String:_fontName];
        Texture2D *tex = [[Texture2D alloc] initWithString:str fontName:fn fontSize:_fontSize];
        
        SoraTexture *texture = new SoraTexture([tex name], tex.contentSize.width, tex.contentSize.height,
                                               tex.pixelsWide, tex.pixelsHigh);
        
        [tex release];
        [str release];
        [fn  release];
        
        return texture;
    }

} // namespace sora
