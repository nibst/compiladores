#include "TAC.h"
#include <stdio.h>
#include <stdlib.h>
// TAC = [code | res | op1 | op2 ]
TACType ast_to_tac[] = {
    [AST_SYMBOL] = TAC_SYMBOL,
    [AST_ADD] = TAC_ADD,
    [AST_SUB] = TAC_SUB,
    [AST_MUL] = TAC_MUL,
    [AST_DIV] = TAC_DIV,
    [AST_AND] = TAC_AND,
    [AST_OR] = TAC_OR,
    [AST_GREATER] = TAC_GREATER,
    [AST_LESS] = TAC_LESS,
    [AST_EQUAL] = TAC_EQUAL,
    [AST_NOT_EQUAL] = TAC_NOT_EQUAL,
    [AST_LESS_EQUAL] = TAC_LESS_EQUAL,
    [AST_GREATER_EQUAL] = TAC_GREATER_EQUAL,  
    [AST_NOT] = TAC_NOT,
    [AST_ATTRIB] = TAC_COPY,
    [AST_INPUT] = TAC_INPUT,
    [AST_OUTPUT] = TAC_OUTPUT,

};
const char* tac_type_to_str[] ={
    [TAC_SYMBOL] = "TAC_SYMBOL",
    [TAC_ADD] = "TAC_ADD",
    [TAC_SUB] = "TAC_SUB",
    [TAC_MUL] = "TAC_MUL",
    [TAC_DIV] = "TAC_DIV",
    [TAC_AND] = "TAC_AND",
    [TAC_OR] = "TAC_OR",
    [TAC_GREATER] = "TAC_GREATER",
    [TAC_LESS] = "TAC_LESS",
    [TAC_EQUAL] = "TAC_EQUAL",
    [TAC_NOT_EQUAL] = "TAC_NOT_EQUAL",
    [TAC_LESS_EQUAL] = "TAC_LESS_EQUAL",
    [TAC_GREATER_EQUAL] = "TAC_GREATER_EQUAL",
    [TAC_NOT] = "TAC_NOT",
    [TAC_COPY] = "TAC_COPY",
    [TAC_INPUT] = "TAC_INPUT",
    [TAC_OUTPUT] = "TAC_OUTPUT",
    [TAC_LABEL] = "TAC_LABEL",
    [TAC_JUMP] = "TAC_JUMP",
    [TAC_JFALSE] = "TAC_JFALSE",
    [TAC_RETURN] = "TAC_RETURN",
    [TAC_VEC_IND] = "TAC_VEC_IND",
    [TAC_RETURN] = "TAC_RETURN",
    [TAC_ARGUMENTS] = "TAC_ARGUMENTS",
    [TAC_BEGIN_FUN] = "TAC_BEGIN_FUN",
    [TAC_END_FUN] = "TAC_END_FUN",
    [TAC_FUNC_CALL] = "TAC_FUNC_CALL",
    [TAC_VEC_DEC] = "TAC_VEC_DEC",
    [TAC_OUTPUT_ARG] = "TAC_OUTPUT_ARG",
    [TAC_FUNC_ARG] = "TAC_FUNC_ARG",
    [TAC_VEC_INIT] = "TAC_VEC_INIT",
    [TAC_VEC_READ] = "TAC_VEC_READ",
    [TAC_VEC_WRITE] = "TAC_VEC_WRITE",
};


TAC *tacCreate(int type, HashNode *res, HashNode *op1, HashNode *op2)
{
    TAC *newtac;
    newtac = (TAC *)calloc(1, sizeof(TAC));
    newtac->type = type;
    newtac->result = res;
    newtac->operator1 = op1;
    newtac->operator2 = op2;
    newtac->prev = 0;
    newtac->next = 0;
    return newtac;
}
void tacPrint(TAC *tac)
{   
    if (!tac)
        return;
    if(tac->type == TAC_SYMBOL)
        return;
    fprintf(stderr, "TAC(");
    fprintf(stderr,"%s",tac_type_to_str[tac->type]);
    fprintf(stderr, ",%s", (tac->result) ? tac->result->text : "0");
    fprintf(stderr, ",%s", (tac->operator1) ? tac->operator1->text : "0");
    fprintf(stderr, ",%s", (tac->operator2) ? tac->operator2->text : "0");
    fprintf(stderr, ");\n");
}

void tacPrintBackwards(TAC *tac)
{
    if (!tac)
        return;
    else
    {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
}
//add next to all tacs (we will have from first TAC to last TAC)
TAC* tacReverse(TAC *tac){
    TAC* t = tac;
    for (t=tac; t->prev; t = t->prev){
        t->prev->next = t;
    }
    return t;
}
TAC* makeIf(TAC* code0, TAC* code1);
TAC* makeIfElse(TAC *code0, TAC *code1, TAC* code2);
TAC* makeIfLoop(TAC *code0, TAC *code1);
// code generation
TAC *generateCode(AST *node)
{
    TAC *result = 0;
    TAC *code[MAX_SONS];
    if (!node)
    {
        return 0; // to initialize code[i] with something
    }
    // Process children
    for (int i = 0; i < MAX_SONS; ++i)
    {
        // each ast son generate a code
        code[i] = generateCode(node->sons[i]);
    }
    // Process this node
    switch (node->type)
    {
    case AST_SYMBOL:
        result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;
    case AST_VEC_IND:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_VEC_IND, makeTemp(), (code[0]) ? code[0]->result : 0, (code[1]) ? code[1]->result : 0));
        break;
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_AND:
    case AST_OR:
    case AST_NOT_EQUAL:
    case AST_EQUAL:
    case AST_LESS:
    case AST_LESS_EQUAL:
    case AST_GREATER:
    case AST_GREATER_EQUAL:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(ast_to_tac[node->type], makeTemp(), (code[0]) ? code[0]->result : 0, (code[1]) ? code[1]->result : 0));
        break;
    case AST_INPUT:
    case AST_NOT:
        result = tacJoin(code[0],tacCreate(ast_to_tac[node->type],makeTemp(), (code[0]) ? code[0]->result : 0, 0));
        break;
    case AST_ATTRIB:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(ast_to_tac[node->type], (code[0])? code[0]->result:0, (code[1]) ? code[1]->result : 0, 0));
        break;
    case AST_IF:
        result = makeIf(code[0],code[1]);
        break;
    case AST_IF_ELSE:
        result = makeIfElse(code[0],code[1],code[2]);
        break;
    case AST_IF_LOOP:
        result = makeIfLoop(code[0],code[1]);
        break;
    case AST_RETURN:
        result = tacJoin(code[0],tacCreate(TAC_RETURN,(code[0]) ? code[0]->result : 0, 0,0));
        break;
    case AST_VAR_DEC:
        //probably dont need this tacJoin(code[1], code[2]) but just in case (because there is no expr on declarations)
        result = tacJoin(tacJoin(code[1], code[2]), tacCreate(TAC_COPY, (code[1])? code[1]->result:0, (code[2]) ? code[2]->result : 0, 0));
        break;
    case AST_VEC_DEC:
        //probably dont need to join with code[1] and code[2] but just in case
        result = tacJoin(tacJoin(code[1], tacJoin(code[2],code[3])),tacCreate(TAC_VEC_DEC, code[1]?code[1]->result:0, code[2]?code[2]->result:0,0));
        break;
    //vec dec will receive the N args of vec init, like it would be done with a function
    case AST_VEC_INIT:
        result = tacJoin(code[0],tacJoin(tacCreate(TAC_VEC_INIT,code[0]?code[0]->result:0,0,0),code[1]));
        break;
    case AST_FUNC_ARGS: 
        result = tacJoin(code[0],tacJoin(tacCreate(TAC_FUNC_ARG,code[0]?code[0]->result:0,0,0),code[1]));
        break;
    case AST_OUTPUT_ARGS:
        result = tacJoin(code[0],tacJoin(tacCreate(TAC_OUTPUT_ARG,code[0]?code[0]->result:0,0,0),code[1]));
        break;
    case AST_OUTPUT:
        //weird thing to get first argument from output 
        result = tacJoin(tacJoin(tacJoin(code[0],tacCreate(TAC_OUTPUT_ARG,code[0]?code[0]->result:0,0,0)),code[1]),tacCreate(TAC_OUTPUT,0,0,0));
        break;
    case AST_FUNC_DEC:
        result = tacJoin(tacJoin(tacCreate(TAC_BEGIN_FUN,code[1]?code[1]->result:0,0,0),tacJoin(code[2],code[3])),tacCreate(TAC_END_FUN,code[1]?code[1]->result:0,0,0));
        break;
    case AST_FUNC_CALL:
        result = tacJoin(tacJoin(code[0],code[1]),tacCreate(TAC_FUNC_CALL,makeTemp(),code[0]?code[0]->result:0,0));
        break;
    default:
        result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
        break;
    }
    return result;
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    TAC *point;
    if (!l1)
        return l2;
    if (!l2)
        return l1;
    for (point = l2; point->prev != 0; point = point->prev)
        ;
    point->prev = l1;
    return l2;
}

TAC *makeIf(TAC *code0, TAC *code1){
    TAC* jump_tac = 0;
    TAC* label_tac = 0;
    HashNode* new_label = 0;

    new_label = makeLabel();
    jump_tac = tacCreate(TAC_JFALSE,new_label,code0?code0->result:0,0);
    jump_tac->prev = code0;
    label_tac = tacCreate(TAC_LABEL,new_label,0,0);
    label_tac->prev = code1;
    return tacJoin(jump_tac,label_tac);
}
TAC* makeIfElse(TAC *code0, TAC *code1, TAC* code2){
    /*  
    code0
    TAC_JF else_label code0
    code1
    TAC_JUMP after_else
    TAC_LABEL else_label
    code2
    TAC_LABEL after_else
    */
    TAC* jump_false= 0;
    TAC* jump_unconditional = 0;
    TAC* label_else = 0;
    TAC* label_after_else = 0;
    HashNode* else_label_name = 0;
    HashNode* after_else_label_name = 0;

    //if
    else_label_name = makeLabel();
    jump_false = tacCreate(TAC_JFALSE,else_label_name,code0?code0->result:0,0);
    jump_false->prev = code0;
    //jump to after else statement
    after_else_label_name = makeLabel();
    jump_unconditional =tacCreate(TAC_JUMP,after_else_label_name,0,0);
    jump_unconditional->prev = code1;
    //else
    label_else = tacCreate(TAC_LABEL,else_label_name,0,0);
    label_after_else = tacCreate(TAC_LABEL,after_else_label_name,0,0);
    label_after_else->prev = tacJoin(label_else,code2); //label else before code2;

    return tacJoin(jump_false,tacJoin(jump_unconditional,label_after_else));
}
TAC* makeIfLoop(TAC *code0, TAC *code1){
    /*
    LABEL loop_start
    code0
    JFALSE after_loop res code0
    code1
    JUMP loop_start
    LABEL after_loop
    */
    TAC* jump_unconditional= 0;
    TAC* jump_false = 0;
    TAC* label_loop_start = 0;
    TAC* label_after_loop = 0;
    HashNode* loop_start_label_name = 0;
    HashNode* after_loop_label_name = 0;
    //LABEL loop_start
    //code0
    //JFALSE after_loop rescode0
    loop_start_label_name = makeLabel();
    after_loop_label_name = makeLabel();
    label_loop_start = tacCreate(TAC_LABEL,loop_start_label_name,0,0);
    jump_false = tacCreate(TAC_JFALSE,after_loop_label_name,code0?code0->result:0,0);
    jump_false->prev = tacJoin(label_loop_start,code0);
    //code1
    //JUMP loop_start
    jump_unconditional = tacCreate(TAC_JUMP,loop_start_label_name,0,0);
    jump_unconditional->prev = code1;
    //LABEL after_loop
    label_after_loop = tacCreate(TAC_LABEL,after_loop_label_name,0,0);
    return tacJoin(jump_false,tacJoin(jump_unconditional,label_after_loop));
}

int doesTacExpectArguments(TAC *tac){
    if (tac==NULL) 
        return 0;
    if (tac->type == TAC_OUTPUT || tac->type == TAC_FUNC_CALL || tac->type == TAC_VEC_DEC) 
        return 1;
    return 0;
}