/*
 *  rftdObjColor.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_OBJ_COLOR_H_
#define RFTD_OBJ_COLOR_H_

#include "SoraPlatform.h"
#include "llexer/llexer.h"

#include "SoraCore.h"

namespace rftd {
	
	// global obj color map;
	class ObjColorLoader: public sora::SoraSingleton<ObjColorLoader> {
		friend class sora::SoraSingleton<ObjColorLoader>;
		
		std::map<uint32, sora::SoraColorRGBA> OBJ_COLOR_MAP;

	public:
		sora::SoraColorRGBA get(uint32 id) {
			std::map<uint32, sora::SoraColorRGBA>::iterator itColor = OBJ_COLOR_MAP.find(id);
			if(itColor != OBJ_COLOR_MAP.end())
				return itColor->second;
			return sora::SoraColorRGBA(1.f, 1.f, 1.f, 1.f);
		}
		
		void readObjColorConf(const std::wstring& conf) {
			llexer* lexer = new llexer;
			
			ulong32 size;
			void* data = sora::SORA->getResourceFile(conf, size);
			if(data) {
				if(lexer->loadFileMem(data, size) == LEX_NO_ERROR) {
					Token tkToken = lexer->getNextToken();
					while(tkToken != TOKEN_TYPE_END_OF_STREAM) {
						
						if(tkToken == TOKEN_TYPE_IDENT) {
							if(strcmpnocase("ObjectColor", lexer->getCurrLexeme()) == 0) {
								tkToken = lexer->getNextToken();
								if(tkToken == TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE) {
									tkToken = lexer->getNextToken();
									sora::SoraColorRGBA color;
									uint32 cid;
									
									while(tkToken == TOKEN_TYPE_IDENT) {
										if(strcmpnocase("Color", lexer->getCurrLexeme()) == 0) {
											lexer->getNextToken();
											if(lexer->getNextToken() == TOKEN_TYPE_DELIM_OPEN_PAREN) {											
												lexer->getNextToken(); uint32 r = atoi(lexer->getCurrLexeme());
												lexer->getNextToken(); uint32 g = atoi(lexer->getCurrLexeme());
												lexer->getNextToken(); uint32 b = atoi(lexer->getCurrLexeme());
												
												color.Set(r/255.f, g/255.f, b/255.f, 1.0f);
											
												lexer->getNextToken();
											}
										} else if(strcmpnocase("Id", lexer->getCurrLexeme()) == 0) {
											lexer->getNextToken();
											lexer->getNextToken();
											
											cid = atoi(lexer->getCurrLexeme());
										}
										
										tkToken = lexer->getNextToken();
									}
									
									OBJ_COLOR_MAP[cid] = color;
								}	
							}
						}
						tkToken = lexer->getNextToken();
					}
				}
				
				delete lexer;
				lexer = NULL;
				sora::SORA->freeResourceFile(data);
			}
			
		}
	};
	
#define GET_COLOR(id) ObjColorLoader::Instance()->get(id)

} // namespace rftd;

#endif // RFTD_OBJ_COLOR_H_