
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER
#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void semanticAnalysis(AST* node);//perform all semantic analysis
void checkAndSetDeclaration(AST* node);//set variables that are declared, undeclared variable are not setted
void checkUndeclared();
void checkFunctionReturnType(DataType expected_return_type, AST *node);
void checkOperands(AST *node);
void checkNature(AST *node); //check if function is used as function, vector is used as vector and so on
void setAstExpressionsDatatype(AST *expr_node);
void checkFunctionParameterVersusArguments(AST *node);
void checkVectorInitialization(AST *node);

int getSemanticErrors();//return SemanticErrors

#endif /*SEMANTIC_HEADER*/