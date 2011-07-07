/*
 *  sqlitecpp.cpp
 *  sqlitecpp
 *
 *  Created by griffin clare on 10年2月7日.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */
#include "SoraSQLite.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {
    
    void SoraSQLite::printSqlErrorMssg(sqlite3* db) {
        DebugPtr->log(std::string("SQLite3: ") + sqlite3_errmsg(db));
    }
    
    void SoraSQLite::printSqlErrorMssg(const char* pstrErrorMssg) {
        DebugPtr->log(std::string("SQLite3: ") + pstrErrorMssg);
    }
    
    static int sql_callback(void* notUsed, int argc, char** argv, char** azColName) {
        
        return 0;
    }
    
    SoraSQLite::SoraSQLite(): 
    bOpened(false), 
    bIsCallbackFunc(false) {
        pCallbackFunc = 0;
        ppstrResult = 0;
        
        pStmt = 0;
        nRows = nColumns = 0;
    }
    
    int SoraSQLite::getColumns() {
        return nColumns;
    }
    
    int SoraSQLite::getRows() { 
        return nRows;
    }
    
    void SoraSQLite::setCallbackFunction(SoraSQLCallbackFunc func) { 
        pCallbackFunc = func;
    }
    
    void SoraSQLite::disableCallBackFunction() { 
        bIsCallbackFunc = false;
    }
    
    int SoraSQLite::opendb(const SoraString& sFileName, bool inMemory) {
        if(bOpened)
            sqlite3_close(db);
        
        int iResult;
        
        if(!inMemory)
            iResult = sqlite3_open(sFileName.c_str(), &db);
        else
            iResult = sqlite3_open(":memory:", &db);
        if(iResult != SQLITE_OK) {
            printSqlErrorMssg(db);
            sqlite3_close(db);
            
            sErrorMssg = sqlite3_errmsg(db);
            return 0;
        }
        bOpened = true;
        return 1;
    }
    
    void SoraSQLite::closedb() {
        if(bOpened) {
            sqlite3_close(db);
            bOpened = false;
        }
    }
    
    int SoraSQLite::exec(const SoraString& sCommand) {
        if(!bOpened)
            return 0;
        
        if(!ppstrResult)
            sqlite3_free_table(ppstrResult);
        
        char* pstrErrorMssg;
        int iResult;
        
        if(!bIsCallbackFunc)
            iResult = sqlite3_get_table(db, sCommand.c_str(), &ppstrResult, &nRows, &nColumns, &pstrErrorMssg);
        else 
            iResult = sqlite3_exec(db, sCommand.c_str(), pCallbackFunc, 0, &pstrErrorMssg);
        
        if(iResult != SQLITE_OK) {
            printSqlErrorMssg(pstrErrorMssg);
            sqlite3_free(pstrErrorMssg);
            
            sErrorMssg = pstrErrorMssg;
            return 0;
        }
        return 1;
    }
    
    int SoraSQLite::beginTransaction() {
        if(!bOpened)
            return 0;
        
        return exec("begin;");
    }
    
    int SoraSQLite::commitTransaction() {
        if(!bOpened)
            return 0;
        
        return exec("end;");
    }
    
    char* SoraSQLite::getResult(size_t row, size_t col) {
        if(row > nRows || col > nColumns || !bOpened) {
            DebugPtr->log("SQLiteCpp::GetResult: No Result Found");
            return '\0';
        }
        return ppstrResult[(row-1)*nColumns+col];
    }
    
    void SoraSQLite::createTable(const SoraString& sTableName, const SoraString& sIdent) {
        if(!bOpened)
            return;
        
        SoraString buffer("create table ");
        buffer += sTableName;
        buffer += "(";
        buffer += sIdent;
        buffer += ")";
        
        exec(buffer);
    }
    
    void SoraSQLite::insertValue(const SoraString& sTableName, const SoraString& sValue) {
        if(!bOpened)
            return;
        
        SoraString buffer("insert into ");
        buffer += sTableName;
        buffer += " values(";
        buffer += sValue;
        buffer += ")";
        
        exec(buffer);
    }
    
    int SoraSQLite::prepare(const SoraString& sCommand) {
        if(!bOpened)
            return 0;
        
        int iResult;
        
        iResult = sqlite3_prepare(db, sCommand.c_str(), -1, &pStmt, 0);
        if(iResult != SQLITE_OK || pStmt == 0) {
            printSqlErrorMssg(db);
            return 0;
        }
        return 1;
    }
    
    int SoraSQLite::stepBlob() {
        if(!bOpened || pStmt == 0)
            return 0;
        
        int iResult = sqlite3_step(pStmt);
        if(iResult != SQLITE_DONE) {
            printSqlErrorMssg(db);
            return 0;
        }
        sqlite3_finalize(pStmt);
        pStmt = 0;
        
        return 1;
    }
    
    int SoraSQLite::insertBlob(const SoraString& sBlob, int iIndex) {
        if(!bOpened || pStmt == 0)
            return 0;
        
        int iResult = sqlite3_bind_blob(pStmt, iIndex, sBlob.c_str(), sBlob.size(), 0);
        if(iResult == SQLITE_OK)
            return stepBlob();
        else {
            printSqlErrorMssg(db);
            return 0;
        }
        return 1;
    }
    
    int SoraSQLite::step() {
        if(!bOpened || pStmt == 0) {
            printSqlErrorMssg("SQLiteCpp::step: invalid sql database statement");
            return 0;
        }
        
        int iResult = sqlite3_step(pStmt);
        if(iResult != SQLITE_ROW) return 0;
        
        bSteped = true;
        
        return 1;
    }
    
    SoraString SoraSQLite::getText(int iCol) {
        if(!bOpened || pStmt == 0 || !bSteped) {
            printSqlErrorMssg("SQLiteCpp::getText: invalid sql database statement");
            return "\0";
        }
        
        const unsigned char* pText = sqlite3_column_text(pStmt, iCol);
        if(pText != 0) {
            SoraString buffer((char*)pText);
            return buffer;
        }
        return "\0";
    }
    
    int SoraSQLite::getType(int iCol) {
        if(!bOpened || pStmt == 0 || !bSteped) {
            printSqlErrorMssg("SQLiteCpp::getType: invalid sql database statement");
            return -1;
        }
        
        return sqlite3_column_type(pStmt, iCol);
    }
    
    SoraString SoraSQLite::getBlob(int iCol) {
        if(!bOpened || pStmt == 0 || !bSteped) {
            printSqlErrorMssg("SQLiteCpp::getBlob: invalid sql database statement");
            return "\0";
        }
        
        const void* pBlob = sqlite3_column_blob(pStmt, iCol);
        int iLen = sqlite3_column_bytes(pStmt, iCol);
        
        SoraString buffer((char*)pBlob, iLen);
        return buffer;
    }
    
    void SoraSQLite::finalize() {
        if(bOpened && pStmt != 0) sqlite3_finalize(pStmt);
    }

} // namespace sora