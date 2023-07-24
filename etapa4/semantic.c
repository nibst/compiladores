
#include "semantic.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>


int SemanticErrors = 0;
/* --------------------------- AUXILIAR FUNCTIONS  -----------------------------------*/
int getDataTypeOfTypeNode(AST *type_node){
    /*get datatype of a type node : AST_TYPE_INT, AST_TYPE_REAL... */
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
            return DATATYPE_NA; //shouldnt happen if working properly
            break;
        }
    }
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
            (operand_node->sons[0]->symbol->datatype == DATATYPE_INT) || 
            (operand_node->sons[0]->symbol->datatype == DATATYPE_REAL) ||
            (operand_node->sons[0]->symbol->datatype == DATATYPE_CHAR)
            )
        )||
        (operand_node->type == AST_VEC_IND && (
            (operand_node->sons[0]->symbol->datatype == DATATYPE_INT) || 
            (operand_node->sons[0]->symbol->datatype == DATATYPE_REAL) ||
            (operand_node->sons[0]->symbol->datatype == DATATYPE_CHAR)
            )
        )
    );
}
void setSymbolDataType(AST *symbol_node){
    //if hash datatype not setted, then it is a literal
    if(symbol_node->symbol->datatype == 0){
        switch (symbol_node->symbol->type){
        case SYMBOL_LIT_INT:
            symbol_node->datatype = DATATYPE_INT;
            break;
        case SYMBOL_LIT_REAL:
            symbol_node->datatype = DATATYPE_REAL;
            break;
        case SYMBOL_LIT_CHAR:
            symbol_node->datatype = DATATYPE_CHAR;
            break;
        case SYMBOL_LIT_STRING:
            symbol_node->datatype = DATATYPE_STRING;
        default:
            break;
        }
    }
    //else is a variable, already setted previously by checkAndSetDeclaration
    else{
        symbol_node->datatype =  symbol_node->symbol->datatype;
        }
}
void checkFunctionArguments(AST* func_dec, AST* current_node){
    if (current_node == 0)
        return;

    if(current_node->type == AST_FUNC_CALL){
        if (current_node->sons[0]->symbol == func_dec->sons[1]->symbol){
            int parameters_length = 0;
            int arguments_length = 0;
            //go through func_dec parameters and func_call args
            for(AST* args_node = current_node->sons[1]; args_node; args_node = args_node->sons[1]){
                ++arguments_length;
            }
            AST* args_node = current_node->sons[1];
            for(AST* parameters_node = func_dec->sons[2]; parameters_node; parameters_node = parameters_node->sons[2]){
                ++parameters_length;
                if(args_node){
                    if(!isCompatibleDataType(parameters_node->sons[1]->datatype, current_node->sons[0]->datatype)){
                        fprintf(stderr,"Semantic ERROR: argument %d of function call %s is not compatible type to function parameter\n",arguments_length,func_dec->sons[1]->symbol->text);
                    }
                    args_node=args_node->sons[1];
                }
            }
            if(parameters_length != arguments_length)
                fprintf(stderr,"Semantic ERROR: mismatch of arguments amout in function call %s\n",func_dec->sons[1]->symbol->text);
        }
    }

    for (int i = 0; i<MAX_SONS; ++i)
        checkFunctionArguments(func_dec,current_node->sons[i]);
}
void checkFunction(AST* top_node,AST *node){
    if (node == 0)
        return;
    if (node->type == AST_FUNC_DEC)
        checkFunctionArguments(node,top_node);
    for (int i = 0; i<MAX_SONS; ++i)
        checkFunction(top_node,node->sons[i]);
}
/*-----------------------------------------------------END OF AUXILIAR FUNCTIONS ------------------------------------------------------*/

/*-----------------------------------------------------semantic.h functions--------------------------------------0-----------------------*/
void semanticAnalysis(AST* node){
    checkAndSetDeclaration(node);
    setAstExpressionsDatatype(node);
    checkUndeclared();
    checkNature(node);
    checkFunctionParameterVersusArguments(node);
    checkOperands(node);
}
void checkVectorInitialization(AST *node){
    
    int length = 0;
    //check size of vector intialization and homogeneity of initialization
    int type = 0;
    for (AST *vector_ini = node->sons[3];vector_ini; vector_ini = vector_ini->sons[1]){
        ++length;
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
    if (length > atoi(node->sons[2]->symbol->text)){
        fprintf(stderr,"Semantic ERROR: initialization of vector '%s' is larger than the declared size of vector \n",node->sons[1]->symbol->text);
        ++SemanticErrors;
    }
        

}
void checkFunctionReturnType(DataType expected_return_type, AST *node){
    if (node == 0)
        return;
    if (node->type == AST_RETURN){
        setAstExpressionsDatatype(node);
        if(!isCompatibleDataType(expected_return_type,node->sons[0]->datatype)){
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
                symbol->datatype = getDataTypeOfTypeNode(node->sons[0]);
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
                symbol->datatype = getDataTypeOfTypeNode(node->sons[0]);
                checkFunctionReturnType(symbol->datatype,node);
                //check for parameters declared on function 
                for(AST* parameters_node = node->sons[2]; parameters_node; parameters_node = parameters_node->sons[2]){
                    symbol = parameters_node->sons[1]->symbol;
                    if(symbol){
                        if(symbol->type != SYMBOL_IDENTIFIER){
                            fprintf(stderr,"Semantic ERROR: variable %s already declared\n",symbol->text);
                            ++SemanticErrors;
                        }
                        symbol->type = SYMBOL_VARIABLE;
                        symbol->datatype = getDataTypeOfTypeNode(parameters_node->sons[0]);
                    }   
                }
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
                symbol->datatype = getDataTypeOfTypeNode(node->sons[0]);
                checkVectorInitialization(node);
            }

            break;
        default: 
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkAndSetDeclaration(node->sons[i]);
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
        case AST_GREATER: 
        case AST_GREATER_EQUAL: 
        case AST_LESS:
        case AST_LESS_EQUAL:
        case AST_DIFFERENT: 
        case AST_EQUAL:
            if(node->sons[0]){
                if(!isNumber(node->sons[0])){
                    fprintf(stderr,"Semantic ERROR: invalid left operand for binary logic operation\n");
                }
            }
            if(node->sons[1]){
                if(!isNumber(node->sons[1])){
                    fprintf(stderr,"Semantic ERROR: invalid right operand for binary logic operation\n");
                }
            }
            
            break;
        case AST_ATTRIB:
            //check datatype of left side attrib, check datatype of right AST_node side of attrib, if both are equal or equivalent type(char or int) than its ok
            if (!isCompatibleDataType(node->sons[0]->datatype,node->sons[1]->datatype)){
                fprintf(stderr,"Semantic ERROR: ATTRIB not compatible types\n");
            }
            break;
        case AST_VEC_IND:
            if(!isCompatibleDataType(DATATYPE_INT,node->sons[1]->datatype))
                fprintf(stderr,"Semantic ERROR: can't index vector with another type other than integer or compatible type\n");
            break;
        default:
            break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkOperands(node->sons[i]);
}
void setAstExpressionsDatatype(AST *expr_node){
    if (expr_node == 0)
        return;
    for (int i = 0; i<MAX_SONS; ++i)
        setAstExpressionsDatatype(expr_node->sons[i]);

    switch (expr_node->type){
        case AST_SYMBOL:
            setSymbolDataType(expr_node);
            break;
        case AST_ADD: 
        case AST_SUB: 
        case AST_MUL: 
        case AST_DIV: 
            if(isCompatibleDataType(expr_node->sons[0]->datatype,expr_node->sons[1]->datatype)){
                if (expr_node->sons[0]->datatype == DATATYPE_CHAR || expr_node->sons[1]->datatype == DATATYPE_CHAR)
                    expr_node->datatype = DATATYPE_INT; //arithmetic op between chars or char+int is int
                else
                    //else so just choose first one (both datatypes are equal in this case because the only compatibility is between char and int)
                    expr_node->datatype = expr_node->sons[0]->datatype; 
            }
            else
                fprintf(stderr,"Semantic ERROR: not compatible types of arithmetic expression\n");
            break;
        case AST_GREATER: 
        case AST_GREATER_EQUAL: 
        case AST_LESS:
        case AST_LESS_EQUAL:
        case AST_DIFFERENT: 
        case AST_EQUAL:
            if(isCompatibleDataType(expr_node->sons[0]->datatype,expr_node->sons[1]->datatype)){
                expr_node->datatype = DATATYPE_BOOL; //boolean op always result in bool
            }
            else
                fprintf(stderr,"Semantic ERROR: not compatible types on LOGIC OPERATION\n");
            break;
        case AST_VEC_IND:
            expr_node->datatype = expr_node->sons[0]->datatype;
            break;
        case AST_INPUT:
            expr_node->datatype = getDataTypeOfTypeNode(expr_node->sons[0]);
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
void checkNature(AST *node){
    if (node == 0)
        return;

    switch (node->type)
    {
    //it has return in each case to reach AST_SYMBOL only when it is a AST_SYMBOL outside of AST_Declarations,AST_VEC_IND and AST_FUNC_CALL
    case AST_VEC_DEC:
        return; //nothing to check here
    case AST_VAR_DEC:
        return; //nothing to check here
    case AST_FUNC_DEC:
        node = node->sons[3]; //check inside function block
        break;
    case AST_VEC_IND:
        if (node->sons[0]->symbol->type != SYMBOL_VECTOR){
            fprintf(stderr,"Semantic ERROR: usage of vector is incorrect\n");
            ++SemanticErrors;
        }
        return; //dont check further down this tree
    case AST_FUNC_CALL:
        if (node->sons[0]->symbol->type != SYMBOL_FUNCTION){
            fprintf(stderr,"Semantic ERROR:  usage of function is incorrect\n");
            ++SemanticErrors;
        }
        else
            checkNature(node->sons[1]); // check nature of function args
        return; //dont check further down this tree
    case AST_SYMBOL:
        if (node->symbol->type == SYMBOL_FUNCTION ||  node->symbol->type == SYMBOL_VECTOR){
            fprintf(stderr,"Semantic ERROR: usage of variable is incorrect\n");
            ++SemanticErrors;
        }
        break;
    default:
        break;
    }
    for (int i = 0; i<MAX_SONS; ++i)
        checkNature(node->sons[i]);
}
void checkFunctionParameterVersusArguments(AST *node){
    checkFunction(node,node);
}
int getSemanticErrors(){
    return SemanticErrors;
}






