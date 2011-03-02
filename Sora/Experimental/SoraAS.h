/*
 *  SoraAS.h
 *  Sora
 *
 *  Created by GriffinBu on 1/21/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_AS_H_
#define SORA_AS_H_

#include "AngelScript/angelscript.h"
#include "AngelScript/Add_On/scriptstdstring/scriptstdstring.h"
#include "AngelScript/Add_On/scriptbuilder/scriptbuilder.h"
#include "SoraCore.h"

#include <map>

namespace sora {
	
	/*
	 a simple pack of asScriptEngine
	 */
	void asMessageCallback(const asSMessageInfo *msg, void *param)
	{
		const char *type = "AS ERR ";
		if( msg->type == asMSGTYPE_WARNING ) 
			type = "AS WARN";
		else if( msg->type == asMSGTYPE_INFORMATION ) 
			type = "AS INFO";
		
#ifdef _DEBUG
		printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
#else
		SORA->logP("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
#endif
	}
	 
	class SoraASScriptEngine {
	public:
		SoraASScriptEngine() {
			/*
			 initializing script engine and register message callback stdstring to it
			 */
			scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
			scriptEngine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL);

			RegisterStdString(scriptEngine);
		}
		~SoraASScriptEngine() {
			scriptEngine->Release();
		}
		
		asIScriptEngine* engine() { return scriptEngine; }
		asIScriptModule* getModule(const SoraString& moduleName) { return scriptEngine->GetModule(moduleName.c_str()); }
		
		void addModule(const SoraString& module) {
			SCRIPT_BUILDER_MAP::iterator itBuilder = builders.find(module);
			if(itBuilder != builders.end()) {
				itBuilder->second->StartNewModule(scriptEngine, module.c_str());
				return;
			}
			CScriptBuilder* builder = new CScriptBuilder;
			int r = builder->StartNewModule(scriptEngine, module.c_str());
			if(r < 0) {
				SORA->log("SoraASCriptEngine: unrecoverable error while starting a new module");
				return ;
			}
			builders[module] = builder;
		}
		
		void addScriptToModule(const SoraString& module, const SoraString& scriptPath) {
			SCRIPT_BUILDER_MAP::iterator itBuilder = builders.find(module);
			if(itBuilder != builders.end()) {
				int r = itBuilder->second->AddSectionFromFile(scriptPath.c_str());
				if(r < 0) {
					SORA->logf("SoraASCriptEngine: script compile error %s.", scriptPath.c_str());
					return ;
				}
			}
		}
		
		void buildModule(const SoraString& module) {
			SCRIPT_BUILDER_MAP::iterator itBuilder = builders.find(module);
			if(itBuilder != builders.end()) {
				int r = itBuilder->second->BuildModule();
				if(r < 0) {
					SORA->logf("SoraASCriptEngine: script compile error, module=%s.", module.c_str());
					return ;
				}
			}
		}
		
		void delModule(const SoraString& module) {
			SCRIPT_BUILDER_MAP::iterator itBuilder = builders.find(module);
			if(itBuilder != builders.end()) {
				delete itBuilder->second;
				itBuilder->second = 0;
				builders.erase(itBuilder);
			}
		}
		
		void execUpdate(const SoraString& module) {
			execFunction(module, "void update()");
		}
		
		void execRender(const SoraString& module) {
			execFunction(module, "void render()");
		}
		
		void execMain(const SoraString& module) {
			execFunction(module, "void main()");
		}
		
		void execFunction(const SoraString& module, const SoraString& function) {
			asIScriptModule* mod = scriptEngine->GetModule(module.c_str());
			int funcId = mod->GetFunctionIdByDecl(function.c_str());
			if(funcId < 0) 
				return;
			
			asIScriptContext* ctx = scriptEngine->CreateContext();
			ctx->Prepare(funcId);
			int r = ctx->Execute();
			if( r != asEXECUTION_FINISHED ) {
				if( r == asEXECUTION_EXCEPTION ) {
					SORA->logf("SoraAScriptEngine::execFunction: function=%s, exception=%s", function.c_str(), ctx->GetExceptionString());
				}
			}
		}
		
	private:
		asIScriptEngine* scriptEngine;
		
		typedef std::map<SoraString, CScriptBuilder*> SCRIPT_BUILDER_MAP;
		SCRIPT_BUILDER_MAP builders;
	};
	
} // namespace sora

#endif