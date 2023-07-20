
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER
#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void checkAndSetDeclaration(AST* node);
void checkUndeclared();

#endif /*SEMANTIC_HEADER*/