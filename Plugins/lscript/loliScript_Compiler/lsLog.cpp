#include "lsLog.h"


using std::string;

void exitWithMssgBox(int iExitCode, const char* format, ...) {
	cout<<format<<endl;
	exit(iExitCode);
}

void logMssg(const char* format, ...) {
//	char sBuffer[10240] = {0};
//	char *lpsArg=(char*)&format+sizeof(format);
//	vsprintf(sBuffer, format, lpsArg);
		
	cout<<"Log: "<<format<<endl;
	//	mLog::getInstance()->writelog(sBuffer, mLogData);	
}

void logMssg(const string& str) {
	cout<<"Log: "<<str<<endl;
}