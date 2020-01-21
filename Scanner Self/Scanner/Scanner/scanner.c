#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank()
{
    while (currentChar != -1 && charCodes[currentChar] == CHAR_SPACE)
        readChar();
}

void skipComment1()
{
    
    while (1)
    {
        //Read next character
        readChar();
        if (currentChar == -1)
        {                                           // End of file
            error(ERR_ENDOFCOMMENT, lineNo, colNo); // comment end with EOF
        }
        else if (charCodes[currentChar] == CHAR_TIMES)
        {               //If next is asteric character
            readChar(); //get next character
        }
        // Note: (* then continue to get next character until meet next if
        if (currentChar == -1)
        {                                           // End of file
            error(ERR_ENDOFCOMMENT, lineNo, colNo); // comment end with EOF
        }
        else if (charCodes[currentChar] == CHAR_RPAR)
        { // Next is right parathesis
            // End of comment
            readChar();
            return;
        }
    }
}

// Skip comment style '//'
void skipComment2() {
    while (1) {
        if (currentChar == '\n') break; // meet '\n' then end cmt
        readChar();
    }
    
}

Token *readIdentKeyword(void)
{
    int count = 0;
    int flag = 0;
    Token *token = makeToken(TK_IDENT, lineNo, colNo);
    token->string[0] = currentChar;
    //while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_UNDERSCORE) // read underscore
    while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT)
    {
        if (count < MAX_IDENT_LEN) {
            token->string[count] = toupper(currentChar); //not distinguish upper and lower
            count++;
        } else {
            if (!flag) {
                flag = 1;
                error(ERR_IDENTTOOLONG, lineNo, colNo - count);
            }
        }
        // Add current character to identifier
        //token->string[count] = currentChar;
        // Increase identifier length
        //count++;

        // Get next character
        readChar();
    }
    // End string
    token->string[count] = '\0';

    // Limit identifier length
    //if (count > MAX_IDENT_LEN) // max identifier length in token.h
    //{
        // Error
    //    error(ERR_IDENTTOOLONG, lineNo, colNo - count);
    //}
    //else
    //{
        // If this identifier is a keyword
        TokenType type = checkKeyword(token->string); // checkKeyword in token.c

        // Otherwise
        if (type != TK_NONE)
        {
            token->tokenType = type;
        }
    //}
    return token;
}

Token *readNumber(void)
{
    Token *token = makeToken(TK_NUMBER, lineNo, colNo);
	int count = 0;

	while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT)) {
		token->string[count++] = (char)currentChar;

    if (count > NUMBER_LEN) {
  		error(ERR_NUMBERTOOLONG, token->lineNo, token->colNo);
  		return token;
  	}

    readChar();
	}

	token->string[count] = '\0';
	token->value = atoi(token->string);
	return token;
}

Token *readConstChar(void) // example 'A', 'B', 'C', one single quote - letter - one single quote 
// already have single quote
{
    Token *token = makeToken(TK_CHAR, lineNo, colNo); // make token character

    // Read next character
    readChar();

    if (currentChar == -1)
    { // End of file
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    }
    else
    {
        switch (charCodes[currentChar])
        {
            // Escape character for Single quote:
        case CHAR_SINGLEQUOTE: // second single quote
            // Read next character
            token->string[0] = currentChar;
            readChar();

            if (charCodes[currentChar] == CHAR_SINGLEQUOTE) 
            {
                token->string[1] = '\0';

                readChar();
                return token;
            }
            else
            {
                error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
            }
            break;

        default:
            // Add the character to the token string
            token->string[0] = currentChar;

            // Read next character
            readChar();

            switch (charCodes[currentChar])
            {
            case CHAR_SINGLEQUOTE:
                // End token
                token->string[1] = '\0';

                readChar();
                return token;

            default:
                error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
                break;
            }
            break;
        }
    }
    return token;
}

// Doc 1 token tinh tu vi tri hien tai
Token *getToken(void)
{
    Token *token;
    // int ln, cn;

    if (currentChar == EOF)                      // If EOF
        return makeToken(TK_EOF, lineNo, colNo); // return token EOF

    switch (charCodes[currentChar])
    {
    case CHAR_SPACE:
        skipBlank(); // skip blank, read next token
        return getToken();
    case CHAR_LETTER:
        return readIdentKeyword(); // token is identifier
    case CHAR_DIGIT:
        return readNumber(); // token is number
    case CHAR_PLUS:
        // Token plus
        token = makeToken(SB_PLUS, lineNo, colNo); // return token '+'
        readChar();                                // read next character
        return token;

    case CHAR_MINUS:
        // Token minus
        token = makeToken(SB_MINUS, lineNo, colNo); // return token '-'
        readChar();                                 // read next character
        return token;

    case CHAR_TIMES:
        // Token Times
        token = makeToken(SB_TIMES, lineNo, colNo); // return token '*'
        readChar();                                 // read next character
        return token;

    case CHAR_SLASH:
        // Token Slash
        token = makeToken(SB_SLASH, lineNo, colNo); // return token '/'
        readChar();                                 // read next character
        if (charCodes[currentChar] == CHAR_SLASH) {
            skipComment2();
            return getToken();
        }
        return token;

        // Skip comment style '//', uncomment when meet '\n'
         
        //token = makeToken(TK_NONE,lineNo,colNo);
        //readChar();
        //if (charCodes[currentChar] == CHAR_SLASH) {
        //    skipComment();
        //    return getToken();
        //}
            
        

    case CHAR_EQ:
        // Token Equal
        token = makeToken(SB_EQ, lineNo, colNo);
        readChar();
        return token;

    case CHAR_COMMA:
        // Token Comma
        token = makeToken(SB_COMMA, lineNo, colNo);
        readChar();
        return token;

    case CHAR_SEMICOLON:
        // Token Semicolon
        token = makeToken(SB_SEMICOLON, lineNo, colNo);
        readChar();
        return token;

    case CHAR_RPAR:
        // Token right parathesis
        token = makeToken(SB_RPAR, lineNo, colNo);
        readChar();
        return token;

    case CHAR_LPAR: // meet '('
        // Empty token
        token = makeToken(TK_NONE, lineNo, colNo);
        // Get next character first
        readChar();

        switch (charCodes[currentChar])
        {
        case CHAR_PERIOD: // meet '.'
            // This is token LSEL
            token->tokenType = SB_LSEL; // return '(.' , LSEL: mark index of array
            readChar();                 // read next character
            return token;

        case CHAR_TIMES: // meet '(*', this is cmt
            // This is a comment so free the allocated token first then skip comments
            free(token);
            skipComment1();
            return getToken();

        case CHAR_SPACE:
            readChar();
            return getToken();

        default:
            // Token Left Parenthesis
            token->tokenType = SB_LPAR;
            //       readChar();
            return token;
        }

    case CHAR_GT:
        // Token Greater
        token = makeToken(SB_GT, lineNo, colNo);

        // If next character is '='
        readChar();
        if (charCodes[currentChar] == CHAR_EQ)
        {
            // Token is Greater Than
            token->tokenType = SB_GE;
            readChar();
        }
        return token;

    case CHAR_LT: // meet '<'
        // Empty token
        token = makeToken(TK_NONE, lineNo, colNo);

        // Check next character
        readChar();

        switch (charCodes[currentChar])
        {

        case CHAR_EQ: // if next character is '='
            // Token Lest Than or Equal
            token->tokenType = SB_LE; // token is '<='
            readChar();
            return token;

        default:
            // Token Least Than
            token->tokenType = SB_LT; // token is '<'
            return token;
        }

    case CHAR_EXCLAIMATION: // '!'
        // Make empty token
        token = makeToken(TK_NONE, lineNo, colNo);

        // If next character is not '='
        readChar();
        if (charCodes[currentChar] != CHAR_EQ)
        {
            // it is an invalid token
            error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
        }
        else
        {
            // else, it's token Not Equal
            token->tokenType = SB_NEQ; // '!='
        }
        return token;

    case CHAR_PERIOD:
        // Token Period
        token = makeToken(SB_PERIOD, lineNo, colNo);

        // If next character is Right Parenthesis
        readChar();
        if (charCodes[currentChar] == CHAR_RPAR)
        {
            // it is token Right Parenthesis
            token->tokenType = SB_RSEL;
            readChar();
        }
        return token;

    case CHAR_COLON:
        // Token Semicolon
        token = makeToken(SB_SEMICOLON, lineNo, colNo);

        // If next character is Equal
        readChar();
        if (charCodes[currentChar] == CHAR_EQ)
        {
            // it is token Assignment
            token->tokenType = SB_ASSIGN; // ':='
            readChar();
        }
        return token;

    case CHAR_SINGLEQUOTE:
        return readConstChar(); // when meet single quote, start to read const char
    
    
    case CHAR_BACKSLASH:
        token = makeToken(SB_BACKSLASH,lineNo,colNo);
        readChar();
        return token;


    default:
        token = makeToken(TK_NONE, lineNo, colNo);
        error(ERR_INVALIDSYMBOL, lineNo, colNo);
        readChar();
        return token;
    }
}

/******************************************************************/

void printToken(Token *token)
{

    printf("%d-%d:", token->lineNo, token->colNo);

    switch (token->tokenType)
    {
    case TK_NONE:
        printf("TK_NONE\n");
        break;
    case TK_IDENT:
        printf("TK_IDENT(%s)\n", token->string);
        break;
    case TK_NUMBER:
        printf("TK_NUMBER(%s)\n", token->string);
        break;
    case TK_CHAR:
        printf("TK_CHAR(\'%s\')\n", token->string);
        break;
    case TK_EOF:
        printf("TK_EOF\n");
        break;

    case KW_PROGRAM:
        printf("KW_PROGRAM\n");
        break;
    case KW_CONST:
        printf("KW_CONST\n");
        break;
    case KW_TYPE:
        printf("KW_TYPE\n");
        break;
    case KW_VAR:
        printf("KW_VAR\n");
        break;
    case KW_INTEGER:
        printf("KW_INTEGER\n");
        break;
    case KW_CHAR:
        printf("KW_CHAR\n");
        break;
    case KW_ARRAY:
        printf("KW_ARRAY\n");
        break;
    case KW_OF:
        printf("KW_OF\n");
        break;
    case KW_FUNCTION:
        printf("KW_FUNCTION\n");
        break;
    case KW_PROCEDURE:
        printf("KW_PROCEDURE\n");
        break;
    case KW_BEGIN:
        printf("KW_BEGIN\n");
        break;
    case KW_END:
        printf("KW_END\n");
        break;
    case KW_CALL:
        printf("KW_CALL\n");
        break;
    case KW_IF:
        printf("KW_IF\n");
        break;
    case KW_THEN:
        printf("KW_THEN\n");
        break;
    case KW_ELSE:
        printf("KW_ELSE\n");
        break;
    case KW_WHILE:
        printf("KW_WHILE\n");
        break;
    case KW_DO:
        printf("KW_DO\n");
        break;
    case KW_FOR:
        printf("KW_FOR\n");
        break;
    case KW_TO:
        printf("KW_TO\n");
        break;

    case SB_SEMICOLON:
        printf("SB_SEMICOLON\n");
        break;
    case SB_COLON:
        printf("SB_COLON\n");
        break;
    case SB_PERIOD:
        printf("SB_PERIOD\n");
        break;
    case SB_COMMA:
        printf("SB_COMMA\n");
        break;
    case SB_ASSIGN:
        printf("SB_ASSIGN\n");
        break;
    case SB_EQ:
        printf("SB_EQ\n");
        break;
    case SB_NEQ:
        printf("SB_NEQ\n");
        break;
    case SB_LT:
        printf("SB_LT\n");
        break;
    case SB_LE:
        printf("SB_LE\n");
        break;
    case SB_GT:
        printf("SB_GT\n");
        break;
    case SB_GE:
        printf("SB_GE\n");
        break;
    case SB_PLUS:
        printf("SB_PLUS\n");
        break;
    case SB_MINUS:
        printf("SB_MINUS\n");
        break;
    case SB_TIMES:
        printf("SB_TIMES\n");
        break;
    case SB_SLASH:
        printf("SB_SLASH\n");
        break;
    case SB_LPAR:
        printf("SB_LPAR\n");
        break;
    case SB_RPAR:
        printf("SB_RPAR\n");
        break;
    case SB_LSEL:
        printf("SB_LSEL\n");
        break;
    case SB_RSEL:
        printf("SB_RSEL\n");
        break;

    case SB_BACKSLASH:
        printf("SB_BACKSLASH\n");
        break;
    }
}

int scan(char *fileName)
{
    Token *token;

    if (openInputStream(fileName) == IO_ERROR)
        return IO_ERROR;

    token = getToken();
    while (token->tokenType != TK_EOF)
    {
        printToken(token);
        free(token);
        token = getToken();
    }

    free(token);
    closeInputStream();
    return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("scanner: no input file.\n");
        return -1;
    }

    if (scan(argv[1]) == IO_ERROR)
    {
        printf("Can\'t read input file!\n");
        return -1;
    }

    return 0;
}

// Others