/*
 *  sqlitecpp.h
 *  sqlitecpp
 *  
 *  a simple c++ wrapper class for sqlite3
 *
 *  Created by griffin clare on 10年2月7日.
 *  Modifed for sora on 11.7.7
 *  Copyright 2010 Ring0Dev. All rights reserved.
 *
 */

#ifndef _SORA_SQLITE_H_
#define _SORA_SQLITE_H_

#include "sqlite3.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "SoraPlatform.h"

namespace sora {

    typedef std::vector<std::wstring> SoraSQLRow;
    typedef int (*SoraSQLCallbackFunc)(void*, int, char**, char**);
    
    class SoraSQLite {
    public:
        SoraSQLite();
        ~SoraSQLite();
        
        /*
         a sqlite database may only exist on a disk file
         no SoraResourceFile implemented here
         if the inMemory flag is true, then we'll create a pure memory file, when you close the database, the data would loss
         */
        int  opendb(const SoraString& sFileName, bool inMemory=false);
        void closedb();
        
        int exec(const SoraString& sCommnad);
        
        SoraString getLastErrorMssg();
        
        int beginTransaction();
        int commitTransaction();
        
        void createTable(const SoraString& sTableName, const SoraString& sTableIdentifier);
        void insertValue(const SoraString& sTableName, const SoraString& sValueDescription);
        
        int         prepare(const SoraString& sCommand);
        SoraString	getText(int iCol);
        int         getType(int iCol);
        int         step();
        int         insertBlob(const SoraString& sBlob, int iIndex);
        SoraString  getBlob(int iCol); 
        void        finalize();
        
        char* getResult(size_t nRows=2, size_t nColumns=1);
        
        void setCallbackFunction(SoraSQLCallbackFunc func);
        void disableCallBackFunction();
        
        int getColumns();
        int getRows();
		
    private:
        static void printSqlErrorMssg(sqlite3* db);
        static void printSqlErrorMssg(const char* pstrErrorMssg);
        
        int stepBlob();
        
        sqlite3* db;
        sqlite3_stmt* pStmt;
        
        bool bOpened;
        bool bIsCallbackFunc;
        bool bPrepared;
        bool bSteped;
        
        SoraSQLCallbackFunc pCallbackFunc;
        
        SoraString sErrorMssg;
        
        int nColumns;
        int nRows;
        
        char** ppstrResult;
    };
    
} // namespace sora


#endif