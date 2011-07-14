//
//  SoraSpriteParser.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraSpriteParser.h"

#include "SoraCore.h"
#include "SoraSprite.h"

#include "json/json.h"

namespace sora {
    sora::SoraSprite* parseSprite(Json::Value& val);
    
    SoraSpriteParser::SoraSpriteParser() {
        
    }
    
    SoraSpriteParser::~SoraSpriteParser() {
        clear();
    }
    
    SoraSprite* SoraSpriteParser::getSprite(const SoraString& name) {
        SPRITE_MAP::iterator itSprite = mSprites.find(str2id(name));
        if(itSprite != mSprites.end()) {
            SpriteStore store = itSprite->second;
            return store.mSprite;
        }
        return NULL;
    }
    
    SoraSpriteAnimation* SoraSpriteParser::getAnimation(const SoraString& name) {
        SPRITE_MAP::iterator itSprite = mSprites.find(str2id(name));
        if(itSprite != mSprites.end()) {
            SpriteStore store = itSprite->second;
            if(store.mIsAnimation) 
                return dynamic_cast<SoraSpriteAnimation*>(store.mSprite);
        }
        return NULL;
    }
    
    void SoraSpriteParser::delSprite(const SoraString& name) {
        SPRITE_MAP::iterator itSprite = mSprites.find(str2id(name));
        if(itSprite != mSprites.end()) {
            mSprites.erase(itSprite);
        }
    }
    
    bool SoraSpriteParser::parseFile(const SoraWString& path) {
        Json::Reader reader;
        SoraResourceFileAuto data(path);
        if(data.isValid()) {
            const char* cdata = (const char*)data;
            Json::Value root;
            
            if(reader.parse(cdata, cdata+data.getSize(), root)) {
                return parse(root);
            }
        }
        return false;
    }
    
    bool SoraSpriteParser::parse(const SoraWString& file, bool isFolder) {
        if(!isFolder) {
            return parseFile(file);
        } else {
            std::vector<SoraWString> files;
            SORA->enumFilesInFolder(files, file);
            if(files.size() != 0) {
                bool isSuccess = true;
                for( size_t i=0; i<files.size(); ++i )
                    if(!parseFile(files[i]))
                        isSuccess = false;
                return isSuccess;
            }
        }
        return false;
    }
    
    bool SoraSpriteParser::parse(const Json::Value& root) {
        if(root.isMember("sprites")) {
            Json::Value sprites = root["sprites"];
            if(sprites.isArray()) {
                for(size_t i=0; i<sprites.size(); ++i) {
                    Json::Value sprVal = sprites[i];
                    if(sprVal.isMember("name")) {
                        std::string name = sprVal["name"].asString();
                        if(!name.empty()) {
                            SoraSprite* sprite = parseSprite(sprVal);
                            bool isAnimation = false;
                            if(sprVal.isMember("animation"))
                                isAnimation = true;
                            
                            if(sprite != NULL) 
                                mSprites.insert(std::make_pair(str2id(name), SpriteStore(sprite, isAnimation)));
                            else
                                THROW_SORA_EXCEPTION(vamssg("Failed to parse sprite with val name = %s", name.c_str()));
                            return true;
                        }
                    }  
                }
            }
            return true;
        } else {
            if(root.isMember("sprite")) {
                Json::Value sprVal = root["sprite"];
                if(sprVal.isMember("name")) {
                    std::string name = sprVal["name"].asString();
                    if(!name.empty()) {
                        SoraSprite* sprite = parseSprite(sprVal);
                        bool isAnimation = false;
                        if(sprVal.isMember("animation"))
                            isAnimation = true;
                        
                        if(sprite != NULL) 
                            mSprites.insert(std::make_pair(str2id(name), SpriteStore(sprite, isAnimation)));
                        else
                            THROW_SORA_EXCEPTION(vamssg("Failed to parse sprite with val name = %s", name.c_str()));
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    void SoraSpriteParser::clear() {
        SPRITE_MAP::iterator itSprite = mSprites.begin();
        while(itSprite != mSprites.end()) {
            delete itSprite->second.mSprite;
            ++itSprite;
        }
        mSprites.clear();
    }
    
    int32 SoraSpriteParser::size() const {
        return mSprites.size();
    }
    
    SoraSprite* SoraSpriteParser::operator[](const SoraString& name) {
        return getSprite(name);
    }
    
    enum {
		PARAM_TYPE_INT = 1,
		PARAM_TYPE_FLOAT,
		PARAM_TYPE_MATRIX,
		PARAM_TYPE_TEXTURE,
	};
	
	inline int32 identToParamType(const std::string& ident) {
		if(strcmpnocase(ident.c_str(), "int") == 0)
			return PARAM_TYPE_INT;
		else if(strcmpnocase(ident.c_str(), "float") == 0)
			return PARAM_TYPE_FLOAT;
		else if(strcmpnocase(ident.c_str(), "matrix") == 0)
			return PARAM_TYPE_MATRIX;
		else if(strcmpnocase(ident.c_str(), "texture") == 0)
			return PARAM_TYPE_TEXTURE;
		return PARAM_TYPE_INT;
	}
	
	inline void parseShader(sora::SoraSprite* pSprite, Json::Value& shaderArray) {
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
                                    std::string texpath = paramList[paramSize-1].asString();
									HSORATEXTURE tex = SORA->createTexture(s2ws(texpath));
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
	
	enum {
		IE_TYPE_ROTATION = 1,
		IE_TYPE_TRANSITION,
		IE_TYPE_FADE,
		IE_TYPE_COLOR,
		IE_TYPE_SCALE,
	};
	
	inline int32 identToIEType(const std::string& str) {
		if(strcmpnocase(str.c_str(), "fade") == 0)
			return IE_TYPE_FADE;
		else if(strcmpnocase(str.c_str(), "color") == 0)
			return IE_TYPE_COLOR;
		else if(strcmpnocase(str.c_str(), "rotation") == 0)
			return IE_TYPE_ROTATION;
		else if(strcmpnocase(str.c_str(), "transition") == 0)
			return IE_TYPE_TRANSITION;
		else if(strcmpnocase(str.c_str(), "scale") == 0)
			return IE_TYPE_SCALE;
		return 0;
	}
	
	inline sora::IMAGE_EFFECT_MODE identToIEPlayType(const std::string& str) {
		if(strcmpnocase(str.c_str(), "repeat") == 0)
			return sora::IMAGE_EFFECT_REPEAT;
		else if(strcmpnocase(str.c_str(), "once") == 0)
			return sora::IMAGE_EFFECT_ONCE;
		else if(strcmpnocase(str.c_str(), "pingpong") == 0)
			return sora::IMAGE_EFFECT_PINGPONG;
		return sora::IMAGE_EFFECT_REPEAT;
	}
    
	inline void parseEffects(sora::SoraSprite* pSprite, Json::Value& effectArray) {
		if(effectArray.isArray()) {
			for(size_t i=0; i<effectArray.size(); ++i) {
				Json::Value effectObj = effectArray[i];
				
				int32 ieType = 0;
				sora::IMAGE_EFFECT_MODE ieMode = sora::IMAGE_EFFECT_REPEAT;
				if(effectObj.isMember("type")) 
					ieType = identToIEType(effectObj["type"].asString());
				if(ieType == 0)
					continue;
				
				if(effectObj.isMember("mode"))
					ieMode = identToIEPlayType(effectObj["mode"].asString());
				
				float32 time = 1.0;
				if(effectObj.isMember("time"))
					time = (float32)effectObj["time"].asDouble();
				
				Json::Value startParam = effectObj["start"];
				Json::Value endParam = effectObj["end"];
				if(startParam.isArray() && endParam.isArray()) {
					switch (ieType) {
						case IE_TYPE_FADE:
							if(startParam.size() >= 1 && endParam.size() >= 1) {
								pSprite->addEffect(new sora::SoraImageEffectFade((float32)startParam[(Json::UInt)0].asDouble(),
																				 (float32)endParam[(Json::UInt)0].asDouble(),
																				 time,
																				 ieMode));
							}
							break;
                            
						case IE_TYPE_ROTATION:
							if(startParam.size() >= 1 && endParam.size() >= 1) {
								pSprite->addEffect(new sora::SoraImageEffectRotation((float32)startParam[(Json::UInt)0].asDouble(),
																					 (float32)endParam[(Json::UInt)0].asDouble(),
																					 time,
																					 ieMode));
							}
							break;
                            
						case IE_TYPE_TRANSITION:
							if(startParam.size() >= 2 && endParam.size() >= 2) {
								pSprite->addEffect(new sora::SoraImageEffectTransitions((float32)startParam[(Json::UInt)0].asDouble(),
																						(float32)startParam[1].asDouble(),
																						(float32)endParam[(Json::UInt)0].asDouble(),
																						(float32)endParam[1].asDouble(),
																						time,
																						ieMode));
							}
							break;
                            
						case IE_TYPE_COLOR:
							if(startParam.size() >=3 && endParam.size() >= 3) {
								sora::SoraColorRGBA startCol((float32)startParam[(Json::UInt)0].asDouble(),
															 (float32)startParam[1].asDouble(),
															 (float32)startParam[2].asDouble(),
															 1.0);
								sora::SoraColorRGBA endCol((float32)endParam[(Json::UInt)0].asDouble(),
														   (float32)endParam[1].asDouble(),
														   (float32)endParam[2].asDouble(),
														   1.0);
								
								pSprite->addEffect(new sora::SoraImageEffectColorTransitions(startCol,
																							 endCol,
																							 time,
																							 ieMode));
								break;
							}
							
						case IE_TYPE_SCALE:
							if(startParam.size() == 1 && endParam.size() == 1) {
								pSprite->addEffect(new sora::SoraImageEffectScale((float32)startParam[(Json::UInt)0].asDouble(),
																				  (float32)endParam[(Json::UInt)0].asDouble(),
																				  time,
																				  ieMode));
							} else if(startParam.size() >= 2 && endParam.size() >= 2) {
								pSprite->addEffect(new sora::SoraImageEffectScale((float32)startParam[(Json::UInt)0].asDouble(),
																				  (float32)startParam[1].asDouble(),
																				  (float32)endParam[(Json::UInt)0].asDouble(),
																				  (float32)endParam[1].asDouble(),
																				  time,
																				  ieMode));
							}
							break;
                            
						default:
							break;
					}
				}
			}
		}
	}
	
	inline sora::ANIMATION_SPRITE_ANCHOR identToAnchor(const std::string& anchor) {
		if(strcmpnocase("center", anchor.c_str()) == 0)
			return sora::ANCHOR_MIDDLE;
		else if(strcmpnocase("upperleft", anchor.c_str()) == 0)
			return sora::ANCHOR_UPPER_LEFT;
		else if(strcmpnocase("upperright", anchor.c_str()) == 0)
			return sora::ANCHOR_UPPER_RIGHT;
		else if(strcmpnocase("lowerleft", anchor.c_str()) == 0)
			return sora::ANCHOR_LOWER_LEFT;
		else if(strcmpnocase("lowerright", anchor.c_str()) == 0)
			return sora::ANCHOR_LOWER_RIGHT;
		return sora::ANCHOR_UPPER_LEFT;
	}
	
	inline void parseSpriteConf(sora::SoraSprite* pSprite, Json::Value& val) {
		if(val.isMember("color")) {
            uint32 color;
            sscanf(val["color"].asCString(), "%x", &color);
            pSprite->setColor(color);
		}
		
		if(val.isMember("shader")) {
			parseShader(pSprite, val["shader"]);
		}
		
		if(val.isMember("effects")) {
			parseEffects(pSprite, val["effects"]);
		}
		
		if(val.isMember("center")) {
			Json::Value centerVal = val["center"];
			if(centerVal.isMember("x") && centerVal.isMember("y"))
				pSprite->setCenter((float32)centerVal["x"].asInt(),
								   (float32)centerVal["y"].asInt());
		}
		
		if(val.isMember("scale")) {
			Json::Value scale = val["scale"];
			if(scale.isMember("v") && scale.isMember("h"))  {
				pSprite->setScale((float32)scale["v"].asDouble(), 
								  (float32)scale["h"].asDouble());
			}
		}
		
		if(val.isMember("rotation")) {
			pSprite->setRotation(sora::DGR_RAD((float32)val["rotation"].asDouble()));
		}
	}
	
    sora::SoraSprite* parseSprite(Json::Value& val) {
		if(val.isMember("texture")) {
            std::string strtex = val["texture"].asString();
            
			sora::HSORATEXTURE tex = SORA->createTexture(s2ws(strtex));
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
				
				parseSpriteConf(pSprite, val);
				
				return pSprite;
			}
		} else if(val.isMember("animation")) {
			std::wstring anmPath = sora::s2ws(val["animation"].asString());
			sora::SoraSpriteAnimation* pAnm = sora::UNPACK_ANIMATION(anmPath);
			if(pAnm != NULL) {
				parseSpriteConf(pAnm, val);
				
				if(val.isMember("anchor")) {
					pAnm->setAnchor(identToAnchor(val["anchor"].asString()));
				}
				return pAnm;
			}
		}
		return NULL;
	}
  
    
} // namespace sora
