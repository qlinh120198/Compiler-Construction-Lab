/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__
#include "token.h"


typedef enum {
  ERR_END_OF_COMMENT, // add a comment at the end of the file, and remove the comment end symbol
  ERR_IDENT_TOO_LONG, // add more letters in a random identifier (check log to fix. Eg: EXAMPLE4 is a good candidate)
  ERR_INVALID_CONSTANT_CHAR, // add another character in char declaration to create this error
  ERR_INVALID_SYMBOL, // remove any symbol with a undeclared one (example: remove : with ~)
  ERR_INVALID_IDENT,
  ERR_INVALID_CONSTANT, // const a = 10 to const a = : => error
  ERR_INVALID_TYPE, // similar to constant, type T = INTEGER => type T = :
  ERR_INVALID_BASICTYPE, // change parameter type to a random string
  ERR_INVALID_VARIABLE, // add a constant and change the variable in for loop into that constant
  ERR_INVALID_FUNCTION,
  ERR_INVALID_PROCEDURE, // error when calling not a procedure identifier => change identifer of call to a const
  ERR_INVALID_PARAMETER, // change the identifier in the param into a random symbol 
  ERR_INVALID_STATEMENT, // change the CALL KW into a colon symbol
  ERR_INVALID_COMPARATOR, // in if else, change the comparator into an operator like + or - because these can pass compileTerm
  ERR_INVALID_EXPRESSION, // cannot get this
  ERR_INVALID_TERM, // fix in for loop / if else change comparator to :. Eg if n = 1 to if n : 1
  ERR_INVALID_FACTOR, // fix in for loop / if else change identifier to ). Eg if n = 1 to if ) = 1
  ERR_INVALID_LVALUE, // when in an assign statement, change left value statement into a constant.
  ERR_INVALID_ARGUMENTS, // remove LPAR from a call statement
  ERR_UNDECLARED_IDENT, // change the left side of LValue expression into an undeclared variable
  ERR_UNDECLARED_CONSTANT, // when declaring a new constant, assign it to an undeclared constant. Eg: const a = b;
  ERR_UNDECLARED_INT_CONSTANT, // Const a = 'a'; b = +a;
  ERR_UNDECLARED_TYPE, // Type t = A; where A is undeclared
  ERR_UNDECLARED_VARIABLE, // change a variable in for loop in a undeclared one
  ERR_UNDECLARED_FUNCTION,
  ERR_UNDECLARED_PROCEDURE, // Change in CALL <procedure name> to a random character.
  ERR_DUPLICATE_IDENT, // in var / const / type / func / proc / param declaration, add a duplicate name
  ERR_TYPE_INCONSISTENCY, // change in parameter when passing to func / proc, if int => pass char and reverse
  ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY,
  ERR_INVALIDNUMBER
} ErrorCode;

void error(ErrorCode err, int lineNo, int colNo);
void missingToken(TokenType tokenType, int lineNo, int colNo);
void assert(char *msg);

#endif
