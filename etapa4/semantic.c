
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
int SemanticErrors = 0;
//transform node type (int,char,real) to ASTDataType
ASTNodeDataType nodeTypeToAstDataType(AST *type_node){
    switch (type_node->type)
    {
    case AST_TYPE_INT:
        return AST_DATATYPE_INT;
    case AST_TYPE_REAL:
        return AST_DATATYPE_REAL;
    case AST_TYPE_CHAR:
        return AST_DATATYPE_CHAR;
    default:
        return AST_DATATYPE_ANY;
    }
}
void checkVectorInitialization(AST *node){
    
    int lenght = 0;
    //check size of vector intialization and homogeneity of initialization
    int type = 0;
    for (AST *vector_ini = node->sons[3];vector_ini; vector_ini = vector_ini->sons[1]){
        ++lenght;
        if (type == 0)
            type = vector_ini->sons[0]->symbol->type;
        else{
            if(type == SYMBOL_LIT_INT || type == SYMBOL_LIT_CHAR)
                if(vector_ini->sons[0]->symbol->type != SYMBOL_LIT_INT && vector_ini->sons[0]->symbol->type != SYMBOL_LIT_CHAR)
                    fprintf(stderr,"Semantic ERROR: vector '%s' is not homogeneous\n",node->sons[1]->symbol->text);
            if(type == SYMBOL_LIT_REAL)
                if(vector_ini->sons[0]->symbol->type != SYMBOL_LIT_REAL)
                    fprintf(stderr,"Semantic ERROR: vector '%s' is not homogeneous\n",node->sons[1]->symbol->text);
        }
    }
    if (lenght > atoi(node->sons[2]->symbol->text)){
        fprintf(stderr,"Semantic ERROR: initialization of vector '%s' is larger than the declared size of vector \n",node->sons[1]->symbol->text);
        ++SemanticErrors;
    }
        

}
void checkFunctionReturnType(ASTNodeDataType expected_return_type, AST *node){
    if (node == 0)
        return;
    if (node->type == AST_RETURN){
        setAstExpressionsDatatype(node);
        if(!isCompatibleASTDataType(expected_return_type,node->sons[0]->datatype)){
            fprintf(stderr,"Semantic ERROR: return type different from declared type of function\n");
        }
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkFunctionReturnType(expected_return_type,node->sons[i]);
}
void checkAndSetDeclaration(AST* node){

    if (node == 0)
        return;
    HashNode *symbol;
    switch (node->type){
        case AST_VAR_DEC:
            symbol = node->sons[1]->symbol; //readability
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
                symbol->type = SYMBOL_VARIABLE;
                symbol->datatype = getDeclarationDataType(node);
                if(symbol->datatype == DATATYPE_INT || symbol->datatype == DATATYPE_CHAR)
                    if (!(node->sons[2]->symbol->type == SYMBOL_LIT_INT || node->sons[2]->symbol->type == SYMBOL_LIT_CHAR)){
                        fprintf(stderr,"Semantic ERROR: variable '%s' has different type than its initialization\n",symbol->text);
                        
                    }      
                if(symbol->datatype == DATATYPE_REAL)
                    if (!(node->sons[2]->symbol->type  == SYMBOL_LIT_REAL)){
                        fprintf(stderr,"Semantic ERROR: variable '%s' has different type than its initialization\n",symbol->text);
                        ++SemanticErrors;
                    }
                        
            }

            break;
        case AST_FUNC_DEC:
            symbol = node->sons[1]->symbol; //readability
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
                symbol->type = SYMBOL_FUNCTION;
                symbol->datatype = getDeclarationDataType(node);
                ASTNodeDataType expected_return_type = nodeTypeToAstDataType(node->sons[0]);
                checkFunctionReturnType(expected_return_type,node);
            }   

            break;
        case AST_VEC_DEC:
            symbol = node->sons[1]->symbol; //readability
            if(symbol){
                if(symbol->type != SYMBOL_IDENTIFIER){
                    fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                    ++SemanticErrors;
                }
                symbol->type = SYMBOL_VECTOR;
                symbol->datatype = getDeclarationDataType(node);
                checkVectorInitialization(node);
            }

            break;
        default: 
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkAndSetDeclaration(node->sons[i]);
}
int isNumber(AST* operand_node){
    return
        (operand_node->type == AST_ADD || operand_node->type == AST_SUB ||
        operand_node->type == AST_DIV || operand_node->type == AST_MUL ||
        (operand_node->type == AST_SYMBOL && operand_node->symbol->type == SYMBOL_LIT_INT) ||
        (operand_node->type == AST_SYMBOL && operand_node->symbol->type == SYMBOL_LIT_REAL) ||
        (operand_node->type == AST_SYMBOL && operand_node->symbol->type == SYMBOL_LIT_CHAR) ||
        (operand_node->type == AST_SYMBOL && 
            (operand_node->symbol->type == SYMBOL_VARIABLE) && (
                (operand_node->symbol->datatype == DATATYPE_INT) || 
                (operand_node->symbol->datatype == DATATYPE_REAL) ||
                (operand_node->symbol->datatype == DATATYPE_CHAR)
            )
        ) ||
        (operand_node->type == AST_FUNC_CALL && (
            (operand_node->symbol->datatype == DATATYPE_INT) || 
            (operand_node->symbol->datatype == DATATYPE_REAL) ||
            (operand_node->symbol->datatype == DATATYPE_CHAR)
            )
        )
    );
}
int isCompatibleASTDataType(ASTNodeDataType datatype1, ASTNodeDataType datatype2){
    //equal case
    if (datatype1 == datatype2)
        return 1;
    //compatible cases
    if (datatype1 == AST_DATATYPE_INT)
        if (datatype2 == AST_DATATYPE_CHAR)
            return 1;
    if (datatype2 == AST_DATATYPE_INT)
        if (datatype1 == AST_DATATYPE_CHAR)
            return 1;
    //not compatible
    return 0;
}
void checkOperands(AST *node){
    if (node == 0){
        return;
    }
    
    switch (node->type){
        case AST_ADD: //IF OPERANDS ARE NOT VALID
            if(node->sons[0]){
                if(!isNumber(node->sons[0])){
                    fprintf(stderr,"Semantic ERROR: invalid left operand for ADD\n");
                }
            }
            if(node->sons[1]){
                if(!isNumber(node->sons[1])){
                    fprintf(stderr,"Semantic ERROR: invalid right operand for ADD\n");
                }
            }
            break;
        case AST_SUB: //IF OPERANDS ARE NOT VALID
            if(node->sons[0]){
                if(!isNumber(node->sons[0])){
                    fprintf(stderr,"Semantic ERROR: invalid left operand for SUB\n");
                }
            }
            if(node->sons[1]){
                if(!isNumber(node->sons[1])){
                    fprintf(stderr,"Semantic ERROR: invalid right operand for SUB\n");
                }
            }
            
            break;
        case AST_MUL: //IF OPERANDS ARE NOT VALID
            if(node->sons[0]){
                if(!isNumber(node->sons[0])){
                    fprintf(stderr,"Semantic ERROR: invalid left operand for MUL\n");
                }
            }
            if(node->sons[1]){
                if(!isNumber(node->sons[1])){
                    fprintf(stderr,"Semantic ERROR: invalid right operand for MUL\n");
                }
            }
            
            break;
        case AST_DIV: //IF OPERANDS ARE NOT VALID
            if(node->sons[0]){
                if(!isNumber(node->sons[0])){
                    fprintf(stderr,"Semantic ERROR: invalid left operand for DIV\n");
                }
            }
            if(node->sons[1]){
                if(!isNumber(node->sons[1])){
                    fprintf(stderr,"Semantic ERROR: invalid right operand for DIV\n");
                }
            }
            
            break;
        case AST_GREATER: //IF OPERANDS ARE NOT VALID
        case AST_GREATER_EQUAL: //IF OPERANDS ARE NOT VALID
        case AST_LESS:
        case AST_LESS_EQUAL:
        case AST_DIFFERENT: //IF OPERANDS ARE NOT VALID
        case AST_EQUAL:
            break;
        case AST_ATTRIB:
            //check datatype of left side attrib, check datatype of right AST_node side of attrib, if both are equal or equivalent type(char or int) than its ok
            if (!isCompatibleASTDataType(node->sons[0]->datatype,node->sons[1]->datatype)){
                fprintf(stderr,"Semantic ERROR: ATTRIB not compatible types\n");
            }
            break;
        case AST_VEC_IND:
            if(!isCompatibleASTDataType(AST_DATATYPE_INT,node->sons[1]->datatype))
                fprintf(stderr,"Semantic ERROR: can't index vector with another type other than integer or compatible type\n");
            break;
        default:
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkOperands(node->sons[i]);
}
void setSymbolASTDataType(AST *symbol_node){
    //if hash datatype not setted, then it is a literal
    if(symbol_node->symbol->datatype == 0){
        switch (symbol_node->symbol->type){
        case SYMBOL_LIT_INT:
            symbol_node->datatype = AST_DATATYPE_INT;
            break;
        case SYMBOL_LIT_REAL:
            symbol_node->datatype = AST_DATATYPE_REAL;
            break;
        case SYMBOL_LIT_CHAR:
            symbol_node->datatype = AST_DATATYPE_CHAR;
            break;
        case SYMBOL_LIT_STRING:
            symbol_node->datatype = AST_DATATYPE_STRING;
        default:
            break;
        }
    }
    //else is a variable, already setted previously by checkAndSetDeclaration
    else{
        switch (symbol_node->symbol->datatype){
        case DATATYPE_INT:
            symbol_node->datatype = AST_DATATYPE_INT;
            break;
        case DATATYPE_REAL:
            symbol_node->datatype = AST_DATATYPE_REAL;
            break;
        case DATATYPE_CHAR:
            symbol_node->datatype = AST_DATATYPE_CHAR;
            break;
        case DATATYPE_BOOL:
            symbol_node->datatype = AST_DATATYPE_BOOL;
            break;
        default:
            break;
        }
    }
}

void setAstExpressionsDatatype(AST *expr_node){
    if (expr_node == 0)
        return;
    for (int i = 0; i<MAX_SONS; ++i)
        setAstExpressionsDatatype(expr_node->sons[i]);

    switch (expr_node->type){
        case AST_SYMBOL:
            setSymbolASTDataType(expr_node);
            break;
        case AST_ADD: 
        case AST_SUB: 
        case AST_MUL: 
        case AST_DIV: 
            if(isCompatibleASTDataType(expr_node->sons[0]->datatype,expr_node->sons[1]->datatype)){
                if (expr_node->sons[0]->datatype == AST_DATATYPE_CHAR || expr_node->sons[1]->datatype == AST_DATATYPE_CHAR)
                    expr_node->datatype = AST_DATATYPE_INT; //arithmetic op between chars or char+int is int
                else
                    //else so just choose first one (both datatypes are equal in this case because the only compatibility is between char and int)
                    expr_node->datatype = expr_node->sons[0]->datatype; 
            }
            else
                fprintf(stderr,"Semantic ERROR: not compatible types\n");
            break;
        case AST_GREATER: 
        case AST_GREATER_EQUAL: 
        case AST_LESS:
        case AST_LESS_EQUAL:
        case AST_DIFFERENT: 
        case AST_EQUAL:
            if(isCompatibleASTDataType(expr_node->sons[0]->datatype,expr_node->sons[1]->datatype)){
                if (expr_node->sons[0]->datatype == AST_DATATYPE_CHAR || expr_node->sons[1]->datatype == AST_DATATYPE_CHAR)
                    expr_node->datatype = AST_DATATYPE_BOOL; //boolean op always result in bool
            }
            else
                fprintf(stderr,"Semantic ERROR: not compatible types on LOGIC OPERATION\n");
            break;
        case AST_VEC_IND:
            expr_node->datatype = expr_node->sons[0]->datatype;
            break;
        case AST_INPUT:
            expr_node->datatype = nodeTypeToAstDataType(expr_node->sons[0]);
            break;
        case AST_PARENTHESIS:
            expr_node->datatype = expr_node->sons[0]->datatype;
            break;
        case AST_FUNC_CALL:
            expr_node->datatype = expr_node->sons[0]->datatype;
            break;
        default:
            break;
    }
}

void checkUndeclared(){
    SemanticErrors += hashCheckUndeclared(g_hash_table,HASH_SIZE);
}
int getSemanticErrors(){
    return SemanticErrors;
}
int getDeclarationDataType(AST *declartion_node){
    /*set datatype of declared variables to its respective declared type*/
    AST *type_node = declartion_node->sons[0];
    if (type_node){
        switch (type_node->type)
        {
        case AST_TYPE_INT:
            return DATATYPE_INT;
            break;
        case AST_TYPE_CHAR:
            return DATATYPE_CHAR;
            break;
        case AST_TYPE_REAL:
            return DATATYPE_REAL;
            break;
        case AST_TYPE_BOOL: 
            return DATATYPE_BOOL; //TODO may be in disuse, remove it 
            break;
        default:
            return 0; //shouldnt happen if working properly
            break;
        }
    }
}

