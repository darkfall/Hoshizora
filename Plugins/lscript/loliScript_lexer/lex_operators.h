/*
 *  Operators.h
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月8日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */
#ifndef _OPERATORS_H_
#define _OPERATORS_H_

#include "lex_defines.h"

#include <cstdio>

#define MAX_OP_STATE_COUNT				32
#define MAX_DELIM_COUNT					24

#define OP_TYPE_ADD					0		// +
#define OP_TYPE_SUB					1		// -
#define OP_TYPE_MUL					2		// *
#define OP_TYPE_DIV					3		// /
#define OP_TYPE_MOD					4		// %
#define OP_TYPE_EXP					5		// ^

#define	OP_TYPE_INC					15		// ++
#define OP_TYPE_DEC					17		// --

#define OP_TYPE_ASSIGN_ADD			14		// +=
#define OP_TYPE_ASSIGN_SUB			16		// -=
#define OP_TYPE_ASSIGN_MUL			18		// *=
#define OP_TYPE_ASSIGN_DIV			19		// /=
#define OP_TYPE_ASSIGN_MOD			20		// %=
#define OP_TYPE_ASSIGN_EXP			21		// ^=

#define OP_TYPE_BITWISE_AND			6		// &
#define OP_TYPE_BITWISE_OR			7		// |
#define OP_TYPE_BITWISE_XOR			8		// #
#define OP_TYPE_BITWISE_NOT			9		// ~
#define OP_TYPE_BITWISE_SHIFT_LEFT	30		// <<
#define	OP_TYPE_BITWISE_SHIFT_RIGHT 32		// >>

#define OP_TYPE_ASSIGN_AND			22		// &=
#define OP_TYPE_ASSIGN_OR			24		// |=
#define OP_TYPE_ASSIGN_XOR			26		// #=
#define OP_TYPE_ASSIGN_SHIFT_LEFT	33		// <<=
#define OP_TYPE_ASSIGN_SHIFT_RIGHT  34		// >>=

#define OP_TYPE_LOGICAL_AND			23		// &&
#define OP_TYPE_LOGICAL_OR			25		// ||
#define OP_TYPE_LOGICAL_NOT			10		// !

#define OP_TYPE_EQUAL				28		// ==
#define OP_TYPE_NOT_EQUAL			27		// !=
#define OP_TYPE_LESS				12		// <
#define OP_TYPE_GREATER				13		// >
#define OP_TYPE_LESS_EQUAL			29		// <=
#define OP_TYPE_GREATER_EQUAL		31		// >=

typedef struct _opState {
	char	cChar;
	int		iSubStateIndex;
	
	int		iSubStateCount;
	int		iIndex;
} opState;

int getOpStateIndex	(char, int, int, int);
int isCharOpChar	(char, int);

opState getOpState(int iCharIndex, int iStateIndex);
#endif
