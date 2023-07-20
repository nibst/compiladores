
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
int SemanticErrors = 0;
void checkAndSetDeclaration(AST* node){
    if (node == 0)
        return;
    HashNode *symbol;
    switch (node->type){
        case AST_VAR_DEC:
            symbol = node->sons[1]->symbol; //readability
            fprintf(stderr,"var\n");
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
            }
            symbol->type = SYMBOL_VARIABLE;
            break;
        case AST_FUNC_DEC:
            symbol = node->sons[1]->symbol; //readability

            fprintf(stderr,"fun\n");

            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
            }
            symbol->type = SYMBOL_FUNCTION;
            break;
        case AST_VEC_DEC:
            symbol = node->sons[1]->symbol; //readability
            fprintf(stderr,"vec\n");

            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
            }
            symbol->type = SYMBOL_VECTOR;
            break;
        default: 
            fprintf(stderr,"default\n");
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkAndSetDeclaration(node->sons[i]);
}