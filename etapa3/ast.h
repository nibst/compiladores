//AST

#ifndef AST_HEADER
#define AST_HEADER
#include "hash.h"

#define MAX_SONS 4 
// Os tipos possiveis de nodo da arvore
typedef enum node_type
{
    AST_SYMBOL,
    AST_ADD,
    AST_SUB,
    AST_DIV,
    AST_MUL,
    AST_LCMD,
    AST_ATTRIB,
    AST_OUTPUT,
    AST_INPUT,
    AST_RETURN,
    AST_BLOCK,
    AST_OUTPUT_ARGS,
    AST_IF,
    AST_IF_ELSE,
    AST_IF_LOOP,
    AST_VEC_IND,
    AST_NOT,
    AST_AND,
    AST_OR,
    AST_DIFFERENT,
    AST_EQUAL,
    AST_GREATER_EQUAL,
    AST_LESS_EQUAL,
    AST_LESS,
    AST_GREATER,
    AST_TYPE_INT,
    AST_TYPE_CHAR,
    AST_TYPE_BOOL,
    AST_TYPE_REAL,
    AST_FUNC_CALL,
    AST_FUNC_ARGS,
    AST_FUNC_DEC,
    AST_FUNC_PARAMS,
    AST_DEC_LIST,
    AST_VEC_DEC,
    AST_VAR_DEC,
    AST_VEC_INIT,
    AST_ID_DEC,
    AST_EMPTY_CMD,
    AST_PARENTHESIS
} ASTNodeType;

typedef struct ast_node{
    ASTNodeType type;
    HashNode *symbol;
    struct ast_node *sons[MAX_SONS];
}AST;
AST* createAST(ASTNodeType type, HashNode *symbol, AST* s0,  AST* s1,  AST* s2,  AST* s3);
void printAST(AST *node,int level);
void decompile(AST *node);
void printCommand(AST *node);
void printCommandList(AST* node);
#endif /* AST_HEADER */