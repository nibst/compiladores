
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER
#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void checkAndSetDeclaration(AST* node);//set variables that are declared, undeclared variable are not setted
void checkUndeclared();
int getSemanticErrors();//return SemanticErrors
int getDeclarationDataType(AST *declartion_node);
void checkOperands(AST *node);
void checkNature(AST *node); //check if function is used as function, vector is used as vector and so on
void setAstExpressionsDatatype(AST *expr_node);
int isCompatibleASTDataType(ASTNodeDataType datatype1, ASTNodeDataType datatype2);
#endif /*SEMANTIC_HEADER*/