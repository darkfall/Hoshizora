/*
 *  rftdSpriteConf.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_TEXTURE_CONF_H_
#define RFTD_TEXTURE_CONF_H_

#include "SoraCore.h"
#include "SoraPlatform.h"

#include "json/json.h"

namespace rftd {
	
	class TextureConfLoader: public sora::SoraSingleton<TextureConfLoader> {
		friend class sora::SoraSingleton<TextureConfLoader>;
		
		std::map<uint32, sora::HSORATEXTURE> GLOBAL_TEXTURE_MAP;

	public:
		sora::HSORATEXTURE get(uint32 id) {
			std::map<uint32, sora::HSORATEXTURE>::iterator itTex = GLOBAL_TEXTURE_MAP.find(id);
			if(itTex != GLOBAL_TEXTURE_MAP.end())
				return itTex->second;
			return 0;
		}
		
		void clear() {
			std::map<uint32, sora::HSORATEXTURE>::iterator itTex = GLOBAL_TEXTURE_MAP.begin();
			while(itTex != GLOBAL_TEXTURE_MAP.end()) {
				sora::SORA->releaseTexture(itTex->second);
			}
			GLOBAL_TEXTURE_MAP.clear();
		}
		
		void readTextureConf(const std::wstring& confPath) {
			ulong32 size;
			void* data = sora::SORA->getResourceFile(confPath, size);
			if(data) {
				llexer* lexer = new llexer;
				if(lexer->loadFileMem(data, size) == LEX_NO_ERROR) {
					Token tkNextToken = lexer->getNextToken();
					while(tkNextToken != TOKEN_TYPE_END_OF_STREAM) {
						if(tkNextToken == TOKEN_TYPE_IDENT) {
							if(strcmpnocase(lexer->getCurrLexeme(), "Texture") == 0) {
								Token ctoken = lexer->getNextToken();
								if(ctoken == TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE) {
									uint32 texid;
									
									tkNextToken = lexer->getNextToken();
									while(tkNextToken != TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE) {
										const char* lexeme = lexer->getCurrLexeme();
										if(strcmpnocase(lexeme, "TexId") == 0) {
											lexer->getNextToken();
											lexer->getNextToken();
											
											texid = atoi(lexer->getCurrLexeme());
										} else if(strcmpnocase(lexeme, "Path") == 0) {
											lexer->getNextToken();
											lexer->getNextToken();
											
											sora::HSORATEXTURE tex = sora::SORA->createTexture(sora::s2ws(lexer->getCurrLexeme()));
											if(tex)
												GLOBAL_TEXTURE_MAP[texid] = tex;
										}
										tkNextToken = lexer->getNextToken();
										if(tkNextToken == TOKEN_TYPE_END_OF_STREAM)
											break;
									} 
								} else {
									sora::SORA->messageBoxW(L"invalid texture definition file: "+confPath, L"Error", MB_OK | MB_ICONERROR);
								}
							}
						}
						tkNextToken = lexer->getNextToken();
					}
					
					delete lexer;
					sora::SORA->freeResourceFile(data);
				}
			}
		
		}
	};
	
#define GET_TEX(id) TextureConfLoader::Instance()->get(id)
	
	enum {
		PARAM_TYPE_INT = 1,
		PARAM_TYPE_FLOAT,
		PARAM_TYPE_MATRIX,
		PARAM_TYPE_TEXTURE,
	};
	
	inline int32 identToParamType(const std::string& ident) {
		if(ident.compare("int") == 0)
			return PARAM_TYPE_INT;
		else if(ident.compare("float") == 0)
			return PARAM_TYPE_FLOAT;
		else if(ident.compare("matrix") == 0)
			return PARAM_TYPE_MATRIX;
		else if(ident.compare("texture") == 0)
			return PARAM_TYPE_TEXTURE;
		return PARAM_TYPE_INT;
	}

	// produce a SoraSprite depends on a rftd texture json configuration value
	static sora::SoraSprite* PRODUCE_SPRITE(Json::Value& val) {
		int32 texid;
		if(val.isMember("texid"))
			texid = val["texid"].asInt();
		else
			return NULL;
		
		sora::HSORATEXTURE tex = GET_TEX(texid);
		if(tex != 0) {
			sora::SoraSprite* pSprite = new sora::SoraSprite(tex);
			
			if(val.isMember("texrect")) {
				std::string texRectStr = val["texrect"].asString();
				std::vector<std::string> params;
				sora::deliStr(params, texRectStr, ',');
				if(params.size() == 4) {
					pSprite->setTextureRect((float32)atof(params[0].c_str()), 
											(float32)atof(params[1].c_str()), 
											(float32)atof(params[2].c_str()), 
											(float32)atof(params[3].c_str()));
				}
			}
			
			if(val.isMember("colorid")) {
				pSprite->setColor(GET_COLOR(val["colorid"].asInt()).GetHWColor());
			}
			
			if(val.isMember("shader")) {
				Json::Value shaderArray = val["shader"];
				if(shaderArray.isArray()) {
					for(size_t i=0; i<shaderArray.size(); ++i) {
						Json::Value shaderVal = shaderArray[i];
						
						std::string shaderFile;
						std::string shaderEntry;
						sora::SORA_SHADER_TYPE shaderType;
						if(shaderVal.isMember("file")) 
							shaderFile = shaderVal["file"].asString();
						else
							continue;
						if(shaderVal.isMember("entry"))
							shaderEntry = shaderVal["entry"].asString();
						else
							continue;
						if(shaderVal.isMember("type")) {
							std::string strType = shaderVal["type"].asString();
							if(strType.compare("fragment_shader") == 0)
								shaderType = sora::FRAGMENT_SHADER;
							else
								shaderType = sora::VERTEX_SHADER;
						} else
							shaderType = sora::FRAGMENT_SHADER;
						
						sora::SoraShader* shader = pSprite->attachShader(sora::s2ws(shaderFile), shaderEntry, shaderType);
						
						if(shader) {
							Json::Value paramArray = shaderVal["params"];
							if(paramArray.isArray()) {
								for(size_t i=0; i<paramArray.size(); ++i) {
									Json::Value param = paramArray[i];
									
									std::string paramName;
									if(param.isMember("name"))
										paramName = param["name"].asString();
									else
										continue;
									
									int32 paramType = PARAM_TYPE_INT;
									if(param.isMember("type"))
										paramType = identToParamType(param["type"].asString());
									
									int32 paramSize;
									Json::Value paramList = param["param_list"];
									if(paramList.isArray()) {
										paramSize = paramList.size();
										
										if(paramType == PARAM_TYPE_INT) {
											int32* paramVals = new int32[paramSize];
											for(size_t j=0; j<paramList.size(); ++j)
												paramVals[j] = paramList[j].asInt();
											shader->setParameteriv(paramName.c_str(), paramVals, paramSize);
											delete paramVals;
									
										} else if(paramType == PARAM_TYPE_FLOAT) {
											float32* paramVals = new float32[paramSize];
											for(size_t j=0; j<paramList.size(); ++j)
												paramVals[j] = (float32)paramList[j].asDouble();
											shader->setParameterfv(paramName.c_str(), paramVals, paramSize);
											delete paramVals;
											
										} else if(paramType == PARAM_TYPE_TEXTURE) {
											int32 texid = paramList[paramSize-1].asInt();
											sora::HSORATEXTURE tex = GET_TEX(texid);
											if(tex != 0) {
												shader->setTexture(paramName.c_str(), tex);
											}
										}
										continue;
									}
								}
							}
						}
					}
				}
			}
			
			if(val.isMember("center")) {
				Json::Value centerVal = val["center"];
				if(centerVal.isMember("x") && centerVal.isMember("y"))
					pSprite->setCenter((float32)centerVal["x"].asInt(),
									   (float32)centerVal["y"].asInt());
			}
			
			return pSprite;
		}
		
		return NULL;
	}
	
} // namespace rftd


#endif // RFTD_SPRITE_CONF_H_