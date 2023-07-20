
#include "semantic.h"
#include <stdio.h>
void checkAndSetDeclaration(AST* node){


    switch (node->type){
        case AST_VAR_DEC:
            HashNode *symbol = node->sons[0]->symbol; //readability
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
            }
            symbol->type = SYMBOL_VARIABLE;
            break;
        case AST_FUNC_DEC:
            HashNode *symbol = node->sons[0]->symbol; //readability
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
            }
            symbol->type = SYMBOL_FUNCTION;
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
            printAST(node->sons[i],level+1);
}