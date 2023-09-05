#ifndef TAC_HEADER
#define TAC_HEADER
#include "hash.h"
#include "ast.h"
// TAC = [code | res | op1 | op2 ]
typedef enum tac_type
{
    TAC_SYMBOL,
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_EQUAL,
    TAC_GREATER_EQUAL,
    TAC_LESS_EQUAL,
    TAC_GREATER,
    TAC_LESS,
    TAC_NOT_EQUAL,
    TAC_AND,
    TAC_OR,
    TAC_NOT,
    TAC_COPY,
    TAC_INPUT,
    TAC_OUTPUT,
    TAC_LABEL,
    TAC_JUMP,   // unconditional jump
    TAC_JFALSE, // jump false
    TAC_RETURN,
    TAC_VEC_IND,
    TAC_BEGIN_FUN,
    TAC_END_FUN,
    TAC_ARGUMENTS,
    TAC_FUNC_CALL,
    TAC_VEC_DEC
} TACType;
typedef struct tac_node
{
    TACType type;
    HashNode *result;
    HashNode *operator1;
    HashNode *operator2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC *tacCreate(int type, HashNode *res, HashNode *op1, HashNode *op2);
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC *tacJoin(TAC *l1, TAC *l2);
TAC* tacReverse(TAC *tac);//used on ASM generation, it adds next to all tacs (we will have from first TAC to last TAC)
int doesTacExpectArguments(TAC *tac);
// code generation
TAC *generateCode(AST *node);
#endif