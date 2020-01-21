/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__
#include "token.h"

typedef enum {
  ERR_ENDOFCOMMENT, // (* comment
  ERR_IDENTTOOLONG, // Charrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  ERR_INVALIDCHARCONSTANT, // ' A'
  ERR_INVALIDSYMBOL, // !!!
  ERR_INVALIDCONSTANT, // 
  ERR_INVALIDTYPE, // ok
  ERR_INVALIDBASICTYPE, //ok
  ERR_INVALIDPARAM, // ok
  ERR_INVALIDSTATEMENT, //ok
  ERR_INVALIDARGUMENTS, //ok
  ERR_INVALIDCOMPARATOR, //ok
  ERR_INVALIDEXPRESSION, // ok
  ERR_INVALIDTERM, // ok
  ERR_INVALIDFACTOR //ok
} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_INVALIDCONSTANT "Invalid constant!"
#define ERM_INVALIDTYPE "Invalid type!"
#define ERM_INVALIDBASICTYPE "Invalid basic type!"
#define ERM_INVALIDPARAM "Invalid parameter!"
#define ERM_INVALIDSTATEMENT "Invalid statement!"
#define ERM_INVALIDARGUMENTS "Invalid arguments!"
#define ERM_INVALIDCOMPARATOR "Invalid comparator!"
#define ERM_INVALIDEXPRESSION "Invalid expression!"
#define ERM_INVALIDTERM "Invalid term!"
#define ERM_INVALIDFACTOR "Invalid factor!"

void error(ErrorCode err, int lineNo, int colNo);
void missingToken(TokenType tokenType, int lineNo, int colNo);
void assert(char *msg);

#endif
