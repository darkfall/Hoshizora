/*
 *  lex_Operators.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月8日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "lex_operators.h"


// ---- First operator characters

opState opChars0[MAX_OP_STATE_COUNT] = 
{ 
	{ '+', 0,  2, 0  }, { '-', 2,  3, 1  }, { '*', 4,  1, 2  }, { '/', 5,  1, 3 },
	{ '%', 6,  1, 4  }, { '^', 7,  1, 5  }, { '&', 8,  2, 6  }, { '|', 10, 2, 7 },
	{ '#', 12, 1, 8  }, { '~', 0,  0, 9  }, { '!', 13, 1, 10 }, { '=', 14, 1, 11 },
	{ '<', 15, 2, 12 }, { '>', 17, 2, 13 }, { '$', 19, 0, 35 },
};

// ---- Second operator characters

opState opChars1[MAX_OP_STATE_COUNT] = 
{
	{ '=', 0, 0, 14 }, { '+', 0, 0, 15 },						 // +=, ++
	{ '=', 0, 0, 16 }, { '-', 0, 0, 17 },  { '>', 0, 0, 40 }     // -=, --, ->	
	{ '=', 0, 0, 18 },											 // *=
	{ '=', 0, 0, 19 },											 // /=
	{ '=', 0, 0, 20 },											 // %=
	{ '=', 0, 0, 21 },                       					 // ^=
	{ '=', 0, 0, 22 }, { '&', 0, 0, 23 },    					 // &=, &&
	{ '=', 0, 0, 24 }, { '|', 0, 0, 25 },    					 // |=, ||
	{ '=', 0, 0, 26 },                       					 // #=
	{ '=', 0, 0, 27 },                       					 // !=
	{ '=', 0, 0, 28 },                       					 // ==
	{ '=', 0, 0, 29 }, { '<', 0, 1, 30 },    					 // <=, <<
	{ '=', 0, 0, 31 }, { '>', 1, 1, 32 },	 					 // >=, >>	
};   

// ---- Third operator characters

opState opChars2[MAX_OP_STATE_COUNT] = 
{ 
	{ '=', 0, 0, 33 },  // <<=, 
	{ '=', 0, 0, 34 }	// >>=
}; 



int isCharOpChar(char cChar, int iCharIndex) {
	for(int iCurrOpStateIndex = 0; iCurrOpStateIndex < MAX_OP_STATE_COUNT; ++iCurrOpStateIndex) {
		char cOpChar;
		switch (iCharIndex) {
			case 0:
				cOpChar = opChars0[iCurrOpStateIndex].cChar;
				break;
				
			case 1:
				cOpChar = opChars1[iCurrOpStateIndex].cChar;
				break;
				
			case 2:
				cOpChar = opChars2[iCurrOpStateIndex].cChar;
				break;
				
			default:
				break;
		}
		if(cChar == cOpChar)
			return TRUE;
	}
	return FALSE;
}

int getOpStateIndex(char cChar, int iCharIndex, int iSubStateIndex, int iSubStateCount) {
	int iStartStateIndex;
	int iEndStateIndex;
	
	if(iCharIndex == 0) {
		iStartStateIndex = 0;
		iEndStateIndex = MAX_OP_STATE_COUNT;
	} else {
		iStartStateIndex = iSubStateIndex;
		iEndStateIndex = iSubStateIndex + iSubStateCount;
	}
	
	for(int iCurrOpStateIndex = iStartStateIndex; iCurrOpStateIndex < iEndStateIndex; ++iCurrOpStateIndex) {
		char cOpChar;
		switch (iCharIndex) {
			case 0:
				cOpChar = opChars0[iCurrOpStateIndex].cChar;
				break;
				
			case 1:
				cOpChar = opChars1[iCurrOpStateIndex].cChar;
				break;
				
			case 2:
				cOpChar = opChars2[iCurrOpStateIndex].cChar;
				break;
				
			default:
				break;
		}
		if(cChar == cOpChar)
			return iCurrOpStateIndex;
	}
	return -1;	
}

opState getOpState(int iCharIndex, int iStateIndex) {
	opState state;
	switch (iCharIndex) {
		case 0:
			state = opChars0[iStateIndex];
			break;
			
		case 1:
			state = opChars1[iStateIndex];
			break;
			
		case 2:
			state = opChars2[iStateIndex];
			break;
			
		default:
			break;
	}
	return state;
}