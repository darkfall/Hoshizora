/*
 *  rftdMapConf.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/9/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_MAP_CONF_H_
#define RFTD_MAP_CONF_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraCore.h"

namespace rftd {
	
	typedef struct {
		std::wstring mapName;
		
		std::wstring mapPath;
	} MapConf;
	
	class MapPackLoader: public sora::SoraSingleton<MapPackLoader> {
		friend class sora::SoraSingleton<MapPackLoader>;
		
		typedef std::vector<MapConf> MAP_CONT;
		typedef sora::hash_map<std::wstring, MAP_CONT > MAP_PACKS;
		
		MAP_PACKS availableMaps;
		
	public:
		MapConf get(const std::wstring& mapPack, uint32 mapId) {
			MAP_PACKS::iterator itMapPack = availableMaps.find(mapPack);
			if(itMapPack != availableMaps.end()) {
				if(mapId < itMapPack->second.size())
					return itMapPack->second[mapId];
			}
			return MapConf();
		}
		
		void readMapPack(const std::wstring& mappack) {
			ulong32 size;
			void* data = sora::SORA->getResourceFile(mappack, size);
			if(data) {
				Json::Reader reader;
				Json::Value root;
				if(reader.parse((const char*)data, (const char*)data+size, root)) {
					MAP_CONT mapCont;
					
					std::wstring mapPackName;
					if(root.isMember("name"))
						mapPackName = sora::s2ws(root["name"].asString());
					else {
						sora::SORA->messageBoxW(L"Error loading map pack, cannot find map pack name, file: "+mappack, L"Error", MB_OK | MB_ICONERROR);
						return;
					}
					
					Json::Value mapArray = root["maps"];
					if(mapArray.isArray()) {
						for(size_t i=0; i<mapArray.size(); ++i) {
							Json::Value mapObj = mapArray[i];
							
							MapConf conf;
							if(mapObj.isMember("mapname"))
								conf.mapName = sora::s2ws(mapObj["mapname"].asString());
							else
								continue;
							if(mapObj.isMember("mappath"))
								conf.mapPath = sora::s2ws(mapObj["mappath"].asString());
							else
								continue;
							
							printf("%s, %s, %s\n", sora::ws2s(mapPackName).c_str(), sora::ws2s(conf.mapName).c_str(), sora::ws2s(conf.mapPath).c_str());

							mapCont.push_back(conf);
						}
					}
					
					availableMaps[mapPackName] = mapCont;
				} else {
					sora::SORA->messageBox(reader.getFormatedErrorMessages(), "Json Error", MB_OK | MB_ICONERROR);
				}
				
				sora::SORA->freeResourceFile(data);
			} else {
				sora::SORA->messageBoxW(L"Cannot find map pack, file: "+mappack, L"Error", MB_OK | MB_ICONERROR);
			}
		}
	};
	
#define GET_MAP(pack, id) MapPackLoader::Instance()->get(pack, id);
	
} // namespace rftd


#endif // RFTD_MAP_CONF_H_