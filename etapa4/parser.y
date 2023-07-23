
%{
    #include "hash.h"
    #include "ast.h"
    #include "semantic.h"
    #include <stdlib.h>
    #include <stdio.h>
    int getLineNumber(void);
    int yylex();
    int yyerror(char* msg);

%}
%union
{
    HashNode *symbol;
    AST* ast;
}
%token KW_CHAR           
%token KW_INT            
%token KW_REAL           
%token KW_BOOL           

%token KW_IF             
%token KW_THEN           
%token KW_ELSE           
%token KW_LOOP           
%token KW_INPUT          
%token KW_OUTPUT         
%token KW_RETURN         

%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token<symbol> TK_IDENTIFIER     

%token<symbol>  LIT_INT           
%token<symbol>  LIT_REAL          
%token<symbol>  LIT_CHAR          
%token<symbol> LIT_STRING        

%token TOKEN_ERROR       

%type<ast> expr
cmd
lcmd
block
output_arguments_list
output_arg
output_tail
type
control_flow
literal
dec
decl
func_args_list
func_args_tail
func_param_list
func_param_tail
parenthesis_expr
vector_ini
%left '&' '|' 
%left '>' '<' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '+' '-'
%left '*' '/' 
%left '~' 

%%
programm: decl {
                checkAndSetDeclaration($1);
                setAstExpressionsDatatype($1);
                checkOperands($1);
                checkUndeclared();
                }
    ;
decl: dec decl {$$ = createAST(AST_DEC_LIST,0,$1,$2,0,0); }
    | {$$ = 0;}
    ;

literal: LIT_CHAR {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    |LIT_INT      {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    |LIT_REAL     {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    |LIT_STRING   {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    ;

type: KW_INT    {$$ = createAST(AST_TYPE_INT,0,0,0,0,0);}
    | KW_CHAR   {$$ = createAST(AST_TYPE_CHAR,0,0,0,0,0);}
    | KW_BOOL   {$$ = createAST(AST_TYPE_BOOL,0,0,0,0,0);}
    | KW_REAL   {$$ = createAST(AST_TYPE_REAL,0,0,0,0,0);}

dec: type TK_IDENTIFIER '=' literal ';'     {$$ = createAST(AST_VAR_DEC,0,$1,createAST(AST_SYMBOL,$2,0,0,0,0),$4,0);}
    |type TK_IDENTIFIER'(' func_param_list ')' block {$$ = createAST(AST_FUNC_DEC,0,$1,createAST(AST_SYMBOL,$2,0,0,0,0),$4,$6);}
    |type TK_IDENTIFIER '[' LIT_INT ']' vector_ini ';'  {$$ = createAST(AST_VEC_DEC,0,$1,createAST(AST_SYMBOL,$2,0,0,0,0),createAST(AST_SYMBOL,$4,0,0,0,0),$6);}
    ;

func_param_list: type TK_IDENTIFIER func_param_tail  {$$ = createAST(AST_FUNC_PARAMS,0,$1,createAST(AST_SYMBOL,$2,0,0,0,0),$3,0);}
    | {$$ = 0;}
    ;
func_param_tail: ',' type TK_IDENTIFIER func_param_tail {$$ = createAST(AST_FUNC_PARAMS,0,$2,createAST(AST_SYMBOL,$3,0,0,0,0),$4,0);}
    |{$$ = 0;}
    ;

vector_ini: literal vector_ini {$$ = createAST(AST_VEC_INIT,0,$1,$2,0,0);}
    |{$$ = 0;}
    ;


block: '{' lcmd '}'  {$$ = createAST(AST_BLOCK,0,$2,0,0,0);}
    ;

//COMMAND LIST-------------------------------------------------------------------------------
lcmd: cmd lcmd      {$$ = createAST(AST_LCMD,0,$1,$2,0,0);}
    |               {$$ = 0;}
    ;

cmd: TK_IDENTIFIER '=' expr ';'             {$$ = createAST(AST_ATTRIB,0,createAST(AST_SYMBOL,$1,0,0,0,0),$3,0,0);}
    |TK_IDENTIFIER '['expr']' '=' expr ';'  {$$ = createAST(AST_ATTRIB,0,createAST(AST_VEC_IND,0,createAST(AST_SYMBOL,$1,0,0,0,0),$3,0,0),$6,0,0);}
    |KW_OUTPUT output_arguments_list ';'    {$$ = $2;}
    |KW_RETURN expr ';'                     {$$ = createAST(AST_RETURN,0,$2,0,0,0);}
    |block                                  {$$ = $1;}
    |control_flow                           {$$ = $1;}
    |';'                                    {$$ = createAST(AST_EMPTY_CMD,0,0,0,0,0);;}
    ;

//ARGUMENTS OF OUTPUT COMMAND---------------------------------------------------------------
output_arguments_list: output_arg output_tail {$$ = createAST(AST_OUTPUT,0,$1,$2,0,0);}
    |   {$$ = 0;}
    ;
output_arg: LIT_STRING {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
            | expr     {$$ = $1;}
            ;

output_tail: ',' output_arg output_tail {$$ = createAST(AST_OUTPUT_ARGS,0,$2,$3,0,0);}
    |   {$$ = 0;}
    ;

//ARGUMENTS OF FUNCTION---------------------------------------------------------------------
func_args_list: expr func_args_tail    {$$ = createAST(AST_FUNC_ARGS,0,$1,$2,0,0);}
                | {$$ = 0;}
                ;

func_args_tail: ',' expr func_args_tail {$$ = createAST(AST_FUNC_ARGS,0,$2,$3,0,0);}
                | {$$ = 0;}
                ;

// EXPRESSIONS ------------------------------------------------------------------------------
expr: LIT_INT       {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    | LIT_CHAR      {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    | LIT_REAL      {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    | TK_IDENTIFIER {$$ = createAST(AST_SYMBOL,$1,0,0,0,0);}
    | TK_IDENTIFIER '['expr']' {$$ = createAST(AST_VEC_IND,0,createAST(AST_SYMBOL,$1,0,0,0,0),$3,0,0);}
    | expr '+' expr {$$ = createAST(AST_ADD,0,$1,$3,0,0);}
    | expr '-' expr {$$ = createAST(AST_SUB,0,$1,$3,0,0);}
    | expr '*' expr {$$ = createAST(AST_MUL,0,$1,$3,0,0);}
    | expr '/' expr {$$ = createAST(AST_DIV,0,$1,$3,0,0);}
    | expr '&' expr {$$ = createAST(AST_AND,0,$1,$3,0,0);}
    | expr '|' expr {$$ = createAST(AST_OR,0,$1,$3,0,0);}
    | '~' expr {$$ = createAST(AST_NOT,0,$2,0,0,0);}
    | expr OPERATOR_DIF expr {$$ = createAST(AST_DIFFERENT,0,$1,$3,0,0);}
    | expr OPERATOR_EQ expr  {$$ = createAST(AST_EQUAL,0,$1,$3,0,0);}
    | expr OPERATOR_GE expr  {$$ = createAST(AST_GREATER_EQUAL,0,$1,$3,0,0);}
    | expr OPERATOR_LE expr  {$$ = createAST(AST_LESS_EQUAL,0,$1,$3,0,0);}
    | expr '<' expr {$$ = createAST(AST_LESS,0,$1,$3,0,0);}
    | expr '>' expr {$$ = createAST(AST_GREATER,0,$1,$3,0,0);}

    | KW_INPUT '('type')'    {$$ = createAST(AST_INPUT,0,$3,0,0,0);}
    | parenthesis_expr  {$$ = $1;} 
    | TK_IDENTIFIER '('func_args_list')' {$$ = createAST(AST_FUNC_CALL,0,createAST(AST_SYMBOL,$1,0,0,0,0),$3,0,0);}

parenthesis_expr: '(' expr ')'  {$$ = createAST(AST_PARENTHESIS,0,$2,0,0,0);}

//CONTROL FLOWS ------------------------------------------------------------------------------                   
control_flow: KW_IF parenthesis_expr cmd             {$$ = createAST(AST_IF,0,$2,$3,0,0);}
            | KW_IF parenthesis_expr cmd KW_ELSE cmd {$$ = createAST(AST_IF_ELSE,0,$2,$3,$5,0);}
            | KW_IF parenthesis_expr KW_LOOP cmd     {$$ = createAST(AST_IF_LOOP,0,$2,$4,0,0);}


%%
int yyerror(char* msg){
    fprintf(stderr,"Syntax error at %d\n",getLineNumber());
    exit(3);
}