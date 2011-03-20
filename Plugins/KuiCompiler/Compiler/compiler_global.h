/*
 *  defines.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月9日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _COMPILER_GLOBAL_H_
#define _COMPILER_GLOBAL_H_

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

#include <sys/types.h>
#include <sys/timeb.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//#define DEBUG

#define OS_VER_MAC

#define MAIN_FUNC_NAME "_MAIN"

#define PARSER_VERBOSE_MODE

#define LSH_VERSION_MAJOR	2
#define LSH_VERSION_MINOR	0

#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

typedef struct _LSH_ScriptHeader {
	int iStackSize;
	int iIsMainFuncPresent;
	int iMainFuncIndex;
	
	int iPriorityType;
	int iUserPriority;
	
	_LSH_ScriptHeader(): iStackSize(1024), iIsMainFuncPresent(FALSE), iMainFuncIndex(-1), iPriorityType(-1), iUserPriority(0) {}
} LSH_ScriptHeader;

inline timeb getCurrentTime() {
	timeb ctime;
	ftime(&ctime);
	return ctime;
}

inline void addMillistm(timeb& time, int millitm) {
	time.millitm += millitm;
	if(time.millitm >= 1000) {
		time.time += (int)(time.millitm / 1000);
		time.millitm %= 1000;
	}
}

inline string int_to_str(int n)
{
	char *str = NULL;                  
	int length = 1;                     
	int temp = 0;
	int sign = 1;                      
	
	if(n<0) {
		sign = -1;
		n = -n;
		++length;                         
	}
	
	temp = n;
	do {
		++length;
	}
	while((temp /= 10)>0);
	
	str = (char*)malloc(length);        
	
	if(sign<0)                      
		str[0] = '-';                    
	
	str[--length] = '\0';             
	
	do {
		str[--length] = '0'+n%10;
	}while((n /= 10) > 0);
	
	string buffer(str);
	delete str;
	return buffer;
}

inline string fp_to_str(double x)
{
	if(x == 0) return "\0";

	char *str = NULL;            
	char *integral = NULL;           
	char *fraction = NULL;          
	size_t length = 0;   
	
	integral = const_cast<char*>(int_to_str((int)x).c_str());    
	
	if(x<0)
		x = -x;
	fraction = const_cast<char*>(int_to_str((int)(100.0*(x+0.005-(int)x))).c_str());
	
	length = strlen(integral)+strlen(fraction)+2;  /* Total length including point and terminator */
	
	/* Fraction must be two digits so allow for it */
	if(strlen(fraction)<2)
		++length;
	
	str = (char*)malloc(length);        /* Allocate memory for total */
	strcpy(str, integral);              /* Copy the integral part    */
	strcat(str, ".");                   /* Append decimal point      */
	
	if(strlen(fraction)<2)              /* If fraction less than two digits */
		strcat(str,"0");                  /* Append leading zero       */
	
	strcat(str, fraction);              /* Append fractional part    */
	
	/* Free up memory for parts as strings */
	free(integral);
	free(fraction);
	
	string buffer(str);
	delete str;
	return buffer;
}

#endif
