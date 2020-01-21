/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

// Program, function, procedure = scope
// outer vs owner, vi du 2 vong for long nhau. Variable cua vong for trong co owner la vong for trong, outer la vong for ngoai
// tham so hinh thuc, vi du: int sum(int num1,int num2)
// tham so thuc su, VD: val1 = 1, val2 = 2, sumVal = sum(val1,val2)

Object *lookupObject(char *name)
{
  Object *obj;
  Scope *currentScope = symtab->currentScope; // get current scope of symtab
  while (currentScope != NULL)
  {
    obj = findObject(currentScope->objList, name); // call find object in this scope's objlist
    if (obj != NULL)
    {
      return obj; // if we find the object then return it
    }
    else
    {
      currentScope = currentScope->outer; // else get to the outer scope.
    }
  }
  // if the object is null, so maybe it's in the global object list (writeln, writec, etc...)
  obj = findObject(symtab->globalObjectList, name);
  if (obj != NULL)
    return obj;
  return NULL;
  // This lookup function needs to scan all object lists of different scopes instead of one.
}

// Kiem tra khai bao ident trong pham vi hien tai
void checkFreshIdent(char *name) {
  // call findObject to look up names in the current scope only !!
  // this is performed when in the scope of program at most parameters
  // meaning constant, type, variable in program scope, procedures, functions and their parameters will be checked here.
  Object *obj = findObject(symtab->currentScope->objList, name); // tim trong pham vi hien tai
  if (obj != NULL) {
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }
}

// Kiem tra ident trong pham vi hien tai va ca pham vi ben ngoai
Object* checkDeclaredIdent(char* name) {
  Object *obj = lookupObject(name); // tim kiem trong cac pham vi co the
  if (obj == NULL) {
    error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

// Kiem tra hang da khai bao hay chua 
Object* checkDeclaredConstant(char* name) {
  // check if a is declared or not / declared as an constant or not
  // maybe declared in the current scope or outer scope.
  // need to duplicate the constant value calling duplicateConstant
  Object *obj = lookupObject(name); // look for the constant object
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->kind != OBJ_CONSTANT)
  {
    error(ERR_INVALID_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->constAttrs->value->type != TP_INT) {
    error(ERR_UNDECLARED_INT_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

// Kiem tra kieu da khai bao hay chua
Object* checkDeclaredType(char* name) {
  // same as constant
  // duplicateType
  Object *obj = lookupObject(name); // look for the type object
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->kind != OBJ_TYPE)
  {
    error(ERR_INVALID_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

// Kiem tra bien da khai bao hay chua
Object* checkDeclaredVariable(char* name) { // chi dung trong vong for vi chi o vong for vi for chi allow variable 
                                            // con cac cho khac con kiem tra cac loai khac
  // When in for statement or analysing factor
  // also search for current and outer blocks
  Object *obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->kind != OBJ_VARIABLE) {
    error(ERR_INVALID_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

// Kiem tra ham da khai bao hay chua 
Object* checkDeclaredFunction(char* name) {
  // checking when in a left hand side of an assign statement - check current function / factor
  // if in factor, check if it's declared or not.
  // also need to check global function
  Object *obj = lookupObject(name);
  if (obj == NULL) {
      error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->kind != OBJ_FUNCTION) {
    error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

// Kiem tra thu tuc da khai bao hay chua
Object* checkDeclaredProcedure(char* name) {
  // only need to check when CALL keyword
  Object *obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  else if (obj->kind != OBJ_PROCEDURE) {
    error(ERR_INVALID_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredLValueIdent(char* name) { // kiem tra ident trong lenh gan
  // we have a check LHS variable in an assign statement
  // left side can be a current function - owner of the scope or a declared variable. If array type => array index must follow variable's name
  Object *obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);

  switch (obj->kind)
  {
  case OBJ_VARIABLE: // neu la bien -> OK
  case OBJ_PARAMETER: // neu la parameter -> OK
    break;
  case OBJ_FUNCTION: // neu lenh gan cho ham thi phai ktra lenh gan do co nam trong ham do hay khong
    if (obj != symtab->currentScope->owner) // vi du function A, owner la A, A := 1 => return
      error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
    break;
  default:
    error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
  }

  return obj;
}
