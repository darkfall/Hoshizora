/*
 *  error.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "lex_errors.h"

#include "stdlib.h"

void ExitOnInvalidInputError(char cInput) {
	printf("Error: '%c' unexpected.\n", cInput);
	exit(0);
}
