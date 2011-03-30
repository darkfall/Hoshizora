/*
 *  SoraGlobalProfiler.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_GLOBAL_PROFILER_H_
#define SORA_GLOBAL_PROFILER_H_

#include "SoraCore.h"
#include "SoraSingleton.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "SoraInternalLogger.h"

namespace sora {
	
	class SoraGlobalProfiler: public SoraSingleton<SoraGlobalProfiler> {
		friend class SoraSingleton<SoraGlobalProfiler>;
	public:
		struct s_profile {
			SoraString sName;
			s_int64 elapsedTime;
			
			s_profile(const char* name, s_int64 t): sName(name), elapsedTime(t) {}
		};
		typedef std::map<SoraString, s_profile> PROFILE_CONT;

	public:
		void storeProfile(const char* name, s_int64 elapsedTime) {
			profiles.insert(std::make_pair<SoraString, s_profile>(name, s_profile(name, elapsedTime)));
			LOG->debugPrintf("ProfileName=%s, time=%llu\n", name, elapsedTime);
		}
		
		void printProfiles() {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				LOG->debugPrintf("ProfileName=%s, time=%llu\n", itprofile->second.sName.c_str(), itprofile->second.elapsedTime);
				++itprofile;
			}
		}
		
		void logProfiles() {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				LOG->debugLogf("ProfileName=%s, time=%llu", itprofile->second.sName.c_str(), itprofile->second.elapsedTime);
				++itprofile;
			}
		}
		
		std::ostream& operator<<(std::ostream& stream) {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				stream << itprofile->second.sName.c_str() << itprofile->second.elapsedTime << std::endl;
				++itprofile;
			}
			return stream;
		}
		
		void flush() {
			profiles.clear();
		}
		
		const PROFILE_CONT& getProfiles() const { return profiles; }
		
	private:
		PROFILE_CONT profiles;
	};
	
	static SoraGlobalProfiler* SORA_PROFILER = SoraGlobalProfiler::Instance();
	
	
} // namespace sora

#endif