#include <iostream>
using namespace std;

#include "lexer.h"

int main (int argc, char * const argv[]) {
    LSH_lexer* lexer = new LSH_lexer;
	lexer->loadFile("untitled.txt");
/*	
	Token tok = lexer->getNextToken();
	while(tok != TOKEN_TYPE_END_OF_STREAM) {
		if(tok != TOKEN_TYPE_OP)
			cout<<"Token Type: "<<tok<<"   lexeme:"<<lexer->getCurrLexeme()<<endl;
		else
			cout<<"Token Type: "<<tok<<" opType: "<<lexer->getCurrOp()<<" Op: "<<lexer->getCurrLexeme()<<endl;
		tok = lexer->getNextToken();
	}
*/
	Token CurrToken;
	
	// The token count
	
	int iTokenCount = 0;
	
	// String to hold the token type
	
	char pstrToken [ 128 ];
	
	// Tokenize the entire source file
	
	while ( TRUE )
	{
		// Get the next token
		
		CurrToken = lexer->getNextToken();
		
		// Make sure the token stream hasn't ended
		
		if ( CurrToken == TOKEN_TYPE_END_OF_STREAM )
			break;
		
		// Convert the token code to a descriptive string
		
		switch ( CurrToken )
		{
                // Integer
				
			case TOKEN_TYPE_INT:
				strcpy ( pstrToken, "Integer" );
				break;
				
                // Float
				
			case TOKEN_TYPE_FLOAT:
				strcpy ( pstrToken, "Float" );
				break;
				
                // Identifier
				
			case TOKEN_TYPE_IDENT:
				strcpy ( pstrToken, "Identifier" );
				break;
				
                // Reserved Words
				
			case TOKEN_TYPE_RSRVD_VAR:
				strcpy ( pstrToken, "var" );
				break;
				
			case TOKEN_TYPE_RSRVD_TRUE:
				strcpy ( pstrToken, "true" );
				break;
				
			case TOKEN_TYPE_RSRVD_FALSE:
				strcpy ( pstrToken, "false" );
				break;
				
			case TOKEN_TYPE_RSRVD_IF:
				strcpy ( pstrToken, "if" );
				break;
				
			case TOKEN_TYPE_RSRVD_ELSE:
				strcpy ( pstrToken, "else" );
				break;
				
			case TOKEN_TYPE_RSRVD_BREAK:
				strcpy ( pstrToken, "break" );
				break;
				
			case TOKEN_TYPE_RSRVD_CONTINUE:
				strcpy ( pstrToken, "continue" );
				break;
				
			case TOKEN_TYPE_RSRVD_FOR:
				strcpy ( pstrToken, "for" );
				break;
				
			case TOKEN_TYPE_RSRVD_WHILE:
				strcpy ( pstrToken, "while" );
				break;
				
			case TOKEN_TYPE_RSRVD_FUNC:
				strcpy ( pstrToken, "func" );
				break;
				
			case TOKEN_TYPE_RSRVD_RETURN:
				strcpy ( pstrToken, "return" );
				break;
				
                // Operators
				
			case TOKEN_TYPE_OP:
				sprintf ( pstrToken, "Operator %d", lexer->getCurrOp() );
				break;
				
                // Delimiters
				
			case TOKEN_TYPE_DELIM_COMMA:
				strcpy ( pstrToken, "Comma" );
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_PAREN:
				strcpy ( pstrToken, "Opening Parenthesis" );
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_PAREN:
				strcpy ( pstrToken, "Closing Parenthesis" );
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_BRACE:
				strcpy ( pstrToken, "Opening Brace" );
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_BRACE:
				strcpy ( pstrToken, "Closing Brace" );
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
				strcpy ( pstrToken, "Opening Curly Brace" );
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE:
				strcpy ( pstrToken, "Closing Curly Brace" );
				break;
				
			case TOKEN_TYPE_DELIM_SEMICOLON:
				strcpy ( pstrToken, "Semicolon" );
				break;
				
                // Strings
				
			case TOKEN_TYPE_STRING:
				strcpy ( pstrToken, "String" );
				break;
		}
		
		// Print the token and the lexeme
		
		printf ( "%d: Token: %s, Lexeme: \"%s\"\n", iTokenCount, pstrToken, lexer->getCurrLexeme() );
		
		// Increment the token count
		
		++ iTokenCount;
	}
	
	// Print the token count
	
	printf ( "\n" );
	printf ( "\tToken count: %d\n", iTokenCount );
    return 0;
}
