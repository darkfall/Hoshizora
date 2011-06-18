/*
 *  rftdBulletConf.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/13/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_BULLET_CONF_H_
#define RFTD_BULLET_CONF_H_

#include "SoraPlatform.h"
#include "json/json.h"

#include "SoraColor.h"

#include "rftdTextureConf.h"

namespace rftd {
	
	typedef struct BulletSpriteConfNode_ {
		sora::SoraSprite* mSprite;
		hgeRect mTextureRect;
		
		BulletSpriteConfNode_(sora::SoraSprite* sprite, const hgeRect& rect):
		mSprite(sprite), mTextureRect(rect) {}
		BulletSpriteConfNode_(): mSprite(NULL) {}
		
	} BulletSpriteConfNode;
	
	class BulletConfLoader: public sora::SoraSingleton<BulletConfLoader> {
	protected:
		friend class sora::SoraSingleton<BulletConfLoader>;
		
		typedef sora::hash_map<uint32/*bulletid*/, BulletSpriteConfNode_> BULLET_CONFIG_MAP;
		BULLET_CONFIG_MAP mBulletConfigs;
		
		typedef sora::hash_map<uint32/*spriteid*/, sora::SoraSprite*> BULLET_SPRITE_MAP;
		BULLET_SPRITE_MAP mBulletSprites;
		
		~BulletConfLoader() {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.begin();
			while(itSprite != mBulletSprites.end()) {
				delete itSprite->second;
				itSprite->second = NULL;
				++itSprite;
			}
		}

	public:
		BulletSpriteConfNode get(int32 bid) {
			BULLET_CONFIG_MAP::iterator itConf = mBulletConfigs.find(bid);
			if(itConf != mBulletConfigs.end()) {
				return itConf->second;
			}
		}
								   
		bool readBulletConf(const std::wstring& confPath) {
			ulong32 size;
			void* data = sora::SORA->getResourceFile(confPath, size);
			if(data) {
				llexer* lexer = new llexer;
				if(lexer->loadFileMem(data, size) == LEX_NO_ERROR) {
					Token tkNextToken = lexer->getNextToken();
					while(tkNextToken != TOKEN_TYPE_END_OF_STREAM) {
						if(tkNextToken == TOKEN_TYPE_IDENT) {
							if(strcmpnocase(lexer->getCurrLexeme(), "BulletData") == 0) {
								Token ctoken = lexer->getNextToken();
								if(ctoken == TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE) {
									BulletSpriteConfNode node;
									uint32 bulletId;
									
									tkNextToken = lexer->getNextToken();
									while(tkNextToken != TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE) {
										const char* lexeme = lexer->getCurrLexeme();
										if(strcmpnocase(lexeme, "TexId") == 0) {
											lexer->getNextToken();
											lexer->getNextToken();
											
											// bullet config must be loaded after texture map
											int32 texId = atoi(lexer->getCurrLexeme());
											BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(texId);
											if(itSprite == mBulletSprites.end()) {
												sora::SoraSprite* spr = new sora::SoraSprite(TextureConfLoader::Instance()->get(texId));
												mBulletSprites[texId] = spr;
												
												node.mSprite = spr;
											} else {
												node.mSprite = itSprite->second;
											}
										} else if(strcmpnocase(lexeme, "TexRect") == 0) {
											lexer->getNextToken();
											if(lexer->getNextToken() == TOKEN_TYPE_DELIM_OPEN_PAREN) {
												hgeRect rect;
												lexer->getNextToken(); rect.x1 = (float32)atof(lexer->getCurrLexeme());
												lexer->getNextToken(); rect.y1 = (float32)atof(lexer->getCurrLexeme());
												lexer->getNextToken(); rect.x2 = (float32)atof(lexer->getCurrLexeme());
												lexer->getNextToken(); rect.y2 = (float32)atof(lexer->getCurrLexeme());
												
												node.mTextureRect = rect;
												
												lexer->getNextToken();
											}
										} else if(strcmpnocase(lexeme, "BulletId") == 0) {
											lexer->getNextToken();
											lexer->getNextToken();
											
											bulletId = atoi(lexer->getCurrLexeme());
										}
										tkNextToken = lexer->getNextToken();
										if(tkNextToken == TOKEN_TYPE_END_OF_STREAM)
											break;
									} 
									
									mBulletConfigs[bulletId] = node;
								} else {
									sora::SORA->messageBoxW(L"invalid bullet definition file: "+confPath, L"Error", MB_OK | MB_ICONERROR);
								}
							}
						}
						tkNextToken = lexer->getNextToken();
					}
					
					sora::SORA->freeResourceFile(data);
					delete lexer;
					return true;
				}
			}
			
			return false;
		}
	};
	
#define GET_BULLET_SPRITE(id) BulletConfLoader::Instance()->get(id)
	
} // namespace rftd


#endif // RFTD_BULLET_CONF_H_