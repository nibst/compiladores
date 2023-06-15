
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

%token TK_IDENTIFIER     

%token LIT_INT           
%token LIT_REAL          
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR       
%{
    int yyerror(char* msg);
%} 
%left '&' '|' 
%left '>' '<' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '+' '-'
%left '*' '/' 
%left '~' 

%%
programa: decl
    ;
decl: dec decl
    |
    ;

literal: LIT_CHAR
    |LIT_INT
    |LIT_REAL
    |LIT_STRING
    ;
type: KW_INT
    | KW_CHAR
    | KW_BOOL
    | KW_REAL

dec: type TK_IDENTIFIER '=' literal ';'
    |type TK_IDENTIFIER '(' func_param_list ')' block
    |type TK_IDENTIFIER '[' LIT_INT ']' vector_ini ';'
    ;

func_param_list: func_param func_tail
    |
    ;
func_param: type TK_IDENTIFIER
    ;
func_tail: ',' func_param func_tail
    |
    ;

vector_ini: literal vector_ini
    |
    ;


block: '{' lcmd '}'
    ;

lcmd: cmd lcmd
    |
    ;

cmd: TK_IDENTIFIER '=' expr ';'
    |TK_IDENTIFIER '['expr']' '=' expr ';'
    |KW_OUTPUT output_arguments_list ';'
    |KW_RETURN expr ';'
    |block
    |control_flow
    |';'
    ;

output_arguments_list: output_arg output_tail
    |
    ;
output_arg: LIT_STRING
            | expr
            ;
output_tail: ',' output_arg output_tail
    |
    ;

func_arguments_list: expr 
                    |expr ',' func_arguments_list
                    ;
expr: LIT_INT
    | LIT_CHAR
    | LIT_REAL
    | TK_IDENTIFIER
    | TK_IDENTIFIER '['expr']'
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '&' expr
    | expr '|' expr
    | expr '~' expr
    | expr OPERATOR_DIF expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_LE expr
    | expr '<' expr
    | expr '>' expr

    | KW_INPUT '('type')'
    | '(' expr ')'
    | TK_IDENTIFIER '('func_arguments_list')'

control_flow: KW_IF '('expr')' cmd
            | KW_IF '('expr')' cmd KW_ELSE cmd 
            | KW_IF '('expr')' KW_LOOP cmd 


%%
int yyerror(char* msg){
    fprintf(stderr,"Syntax error at %d\n",getLineNumber());
    exit(3);
}