//AST
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

extern FILE *yyout;
const char* type_to_str[] =
{
    [AST_SYMBOL] = "AST_SYMBOL",
    [AST_ADD] = "AST_ADD",
    [AST_SUB] = "AST_SUB",
    [AST_DIV] = "AST_DIV",
    [AST_MUL] = "AST_MUL",
    [AST_LCMD] = "AST_LCMD",
    [AST_ATTRIB] = "AST_ATTRIB",
    [AST_RETURN] = "AST_RETURN",
    [AST_BLOCK] = "AST_BLOCK",
    [AST_OUTPUT] = "AST_OUTPUT",
    [AST_INPUT] = "AST_INPUT",
    [AST_OUTPUT_ARGS] = "AST_OUTPUT_ARGS",
    [AST_IF] = "AST_IF",
    [AST_IF_ELSE] = "AST_IF_ELSE",
    [AST_IF_LOOP] = "AST_IF_LOOP",
    [AST_VEC_IND] = "AST_VEC_IND",
    [AST_DIFFERENT] = "AST_DIFFERENT",
    [AST_EQUAL] = "AST_EQUAL",
    [AST_GREATER_EQUAL] = "AST_GREATER_EQUAL",
    [AST_LESS_EQUAL] = "AST_LESS_EQUAL",
    [AST_LESS] = "AST_LESS",
    [AST_GREATER] = "AST_GREATER",
    [AST_TYPE_INT] = "AST_TYPE_INT",
    [AST_TYPE_CHAR] = "AST_TYPE_CHAR",
    [AST_TYPE_BOOL] = "AST_TYPE_BOOL",
    [AST_TYPE_REAL] = "AST_TYPE_REAL",
    [AST_FUNC_CALL] = "AST_FUNC_CALL",
    [AST_FUNC_ARGS] = "AST_FUNC_ARGS",
    [AST_FUNC_DEC] = "AST_FUNC_DEC",
    [AST_FUNC_PARAMS] = "AST_FUNC_PARAMS",
    [AST_DEC_LIST] = "AST_DEC_LIST",
    [AST_VEC_DEC] = "AST_VEC_DEC",
    [AST_VAR_DEC] = "AST_VAR_DEC",
    [AST_VEC_INIT] = "AST_VEC_INIT",
    [AST_ID_DEC] = "AST_ID_DEC",
    [AST_EMPTY_CMD] = "AST_EMPTY_CMD",
    [AST_PARENTHESIS] = "AST_PARENTHESIS",
    [AST_NOT] = "AST_NOT" ,
    [AST_AND] = "AST_AND",
    [AST_OR] = "AST_OR"
};

const char* type_to_source[] =
{
    [AST_ADD] = "+",
    [AST_SUB] = "-",
    [AST_DIV] = "/",
    [AST_MUL] = "*",
    [AST_ATTRIB] = "=",
    [AST_RETURN] = "return",
    [AST_BLOCK] = "{",
    [AST_OUTPUT] = "output",
    [AST_INPUT] = "input",
    [AST_DIFFERENT] = "!=",
    [AST_EQUAL] = "==",
    [AST_GREATER_EQUAL] = ">=",
    [AST_LESS_EQUAL] = "<=",
    [AST_LESS] = "<",
    [AST_GREATER] = ">",
    [AST_TYPE_INT] = "int",
    [AST_TYPE_CHAR] = "char",
    [AST_TYPE_BOOL] = "bool",
    [AST_TYPE_REAL] = "real",
    [AST_EMPTY_CMD] = ";",
    [AST_PARENTHESIS] = "(",

};
AST* createAST(ASTNodeType type, HashNode *symbol, AST* s0,  AST* s1,  AST* s2,  AST* s3){
    AST* new_node = (AST*)calloc(1,sizeof(AST));
    new_node->type = type;
    new_node->symbol = symbol;
    new_node->sons[0] = s0;
    new_node->sons[1] = s1;
    new_node->sons[2] = s2;
    new_node->sons[3] = s3;
    return new_node;
}

void printAST(AST *node, int level){
    if (node == 0)
        return;
    for (int i =0; i<level; i++)
        fprintf(stderr,"  ");

    
    if (0){
        fprintf(stderr,"\n");
        for (int i = 0; i<MAX_SONS; ++i)
            printAST(node->sons[i],level);
    }
    else{
        fprintf(stderr,"ast(");
        fprintf(stderr,"%s",type_to_str[node->type]);

        if (node->symbol != 0)
            fprintf(stderr,",%s\n",node->symbol->text);
        else
            fprintf(stderr,",0\n");
        for (int i = 0; i<MAX_SONS; ++i)
            printAST(node->sons[i],level+1);

    }

}
int endOfList(AST* node){return node->sons[1] == 0;};

void printCommand(AST *node){
    if (node == 0)
        return;
    switch (node->type)
    {
    case AST_SYMBOL:
        fprintf(yyout,"%s",node->symbol->text);
        break;
    case AST_VEC_IND:
        printCommand(node->sons[0]);
        fprintf(yyout," [");
        printCommand(node->sons[1]);
        fprintf(yyout,"]");
        break;       
    case AST_VAR_DEC:
        printCommand(node->sons[0]);//type
        printCommand(node->sons[1]);//id
        fprintf(yyout," = ");
        printCommand(node->sons[2]);//init
        fprintf(yyout,";\n");
        break;
    case AST_VEC_DEC:
        printCommand(node->sons[0]);//type
        printCommand(node->sons[1]);//id
        fprintf(yyout,"[");
        printCommand(node->sons[2]);//size
        fprintf(yyout,"]");
        printCommand(node->sons[3]);//init
        fprintf(yyout,";\n");
        break;
    case AST_VEC_INIT:
        fprintf(yyout," ");
        printCommand(node->sons[0]);
        printCommand(node->sons[1]);
        break;
    case AST_FUNC_DEC:
        printCommand(node->sons[0]);//type
        printCommand(node->sons[1]);//id
        fprintf(yyout,"(");
        printCommand(node->sons[2]);//params
        fprintf(yyout,")\n");
        printCommand(node->sons[3]);//block
        break;
    case AST_FUNC_PARAMS:
        printCommand(node->sons[0]);
        printCommand(node->sons[1]);
        if (node->sons[2] != 0)
            fprintf(yyout,", ");
        printCommand(node->sons[2]);
        break;
    case AST_ATTRIB:
        printCommand(node->sons[0]); //print left side
        fprintf(yyout," = ");
        printCommand(node->sons[1]);//print right side
        fprintf(yyout,";\n");
        break;
    case AST_OUTPUT:
        fprintf(yyout,"output ");
        printCommand(node->sons[0]);
        printCommand(node->sons[1]);
        fprintf(yyout,";\n");
        break;
    case AST_OUTPUT_ARGS:
        fprintf(yyout,", ");
        printCommand(node->sons[0]);
        printCommand(node->sons[1]);
        break;
    case AST_INPUT:
        fprintf(yyout,"input (");
        printCommand(node->sons[0]);
        fprintf(yyout,")");
        break;
    case AST_RETURN:
        fprintf(yyout,"return ");
        printCommand(node->sons[0]);
        fprintf(yyout,";\n");
        break;
    case AST_PARENTHESIS:
        fprintf(yyout,"(");
        printCommand(node->sons[0]);
        fprintf(yyout,")");
        break;
    case AST_FUNC_CALL:
        printCommand(node->sons[0]);
        fprintf(yyout,"(");
        printCommand(node->sons[1]);
        fprintf(yyout,")");
        break;
    case AST_FUNC_ARGS:
        printCommand(node->sons[0]);
        if (node->sons[1] != 0)
            fprintf(yyout,", ");
        printCommand(node->sons[1]);
        break;
    //BINOPS
    case AST_ADD:
    case AST_SUB:
    case AST_DIV:
    case AST_MUL:
    case AST_AND:
    case AST_OR:
    case AST_EQUAL:
    case AST_DIFFERENT:
    case AST_LESS_EQUAL:
    case AST_LESS:
    case AST_GREATER_EQUAL:
    case AST_GREATER:
        printCommand(node->sons[0]);
        fprintf(yyout,"%s",type_to_source[node->type]);
        printCommand(node->sons[1]);
        break;
    //UNARY
    case AST_NOT:
        fprintf(yyout,"~");
        printCommand(node->sons[0]);
        break;
    //CONTROL FLOW
    case AST_IF:
        fprintf(yyout,"if");
        printCommand(node->sons[0]);
        fprintf(yyout,"\n");
        printCommand(node->sons[1]);
        break;
    case AST_IF_ELSE:
        fprintf(yyout,"if");
        printCommand(node->sons[0]);
        fprintf(yyout,"\n");
        printCommand(node->sons[1]);
        fprintf(yyout,"else\n");
        printCommand(node->sons[2]);
        break;
    case AST_IF_LOOP:
        fprintf(yyout,"if");
        printCommand(node->sons[0]);
        fprintf(yyout,"loop\n");
        printCommand(node->sons[1]);
        break;
    case AST_EMPTY_CMD:
        fprintf(yyout,";");
        break;
    case AST_BLOCK:
        fprintf(yyout,"{");
        printCommandList(node->sons[0]);
        fprintf(yyout,"}");
        break;
    //TYPES
    case AST_TYPE_CHAR:
    case AST_TYPE_BOOL:
    case AST_TYPE_INT:
    case AST_TYPE_REAL:
        fprintf(yyout,"%s ",type_to_source[node->type]);
        break;
    default:
        break;
    }
}
void printCommandList(AST* node){
    if(node == 0)
        return;
    while (node!=0){
        printCommand(node->sons[0]);
        node = node->sons[1];
    }
    printAST(node,0);
}

void decompile(AST *node){   
    if(node == 0)
        return;
    while (node!=0){
        if (node->sons[0]->type == AST_BLOCK){
            fprintf(yyout,"{\n");
            printCommandList(node->sons[0]->sons[0]);
            fprintf(yyout,"}");
        }
        else{
            printCommand(node->sons[0]);
        }
        node = node->sons[1];
    }    
}


    