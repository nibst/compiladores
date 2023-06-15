
/*Níkolas Padão Schuster*/

%{
    #include "hash.c"
    #include "y.tab.h"
    Hash* g_hash_table[HASH_SIZE];

    int running = 1;
    int line_number = 1;

%}

%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT


%%
char    {return KW_CHAR;}
int     {return KW_INT;}
real    {return KW_REAL;}
bool    {return KW_BOOL;}
if      {return KW_IF;}
then    {return KW_THEN;}
else    {return KW_ELSE;}
loop    {return KW_LOOP;}
input   {return KW_INPUT;}
output  {return KW_OUTPUT;}
return  {return KW_RETURN;}
"\n"    {++line_number;}
"\\\\" {BEGIN(SINGLE_LINE_COMMENT);}

"\\\\\\" {BEGIN(MULTI_LINE_COMMENT);}



[-,;()[\]{}=+*/%<>&|~] {return yytext[0];}
"<="    {return OPERATOR_LE;}
">="    {return OPERATOR_GE;}
"=="    {return OPERATOR_EQ;}
"!="    {return OPERATOR_DIF;}

\"(\\.|[^\n"])*\" {insert(yytext,SYMBOL_LIT_STRING,g_hash_table,HASH_SIZE);return LIT_STRING;}
\'.\'           {insert(yytext,SYMBOL_LIT_CHAR,g_hash_table,HASH_SIZE);return LIT_CHAR;}
[a-zA-Z._]+     {insert(yytext,SYMBOL_IDENTIFIER,g_hash_table,HASH_SIZE);return TK_IDENTIFIER;}
[0-9]+[.][0-9]+ {insert(yytext,SYMBOL_LIT_REAL,g_hash_table,HASH_SIZE);return LIT_REAL;}
[0-9]+          {insert(yytext,SYMBOL_LIT_INT,g_hash_table,HASH_SIZE);return LIT_INT;}

[ \t]     {}

. {return TOKEN_ERROR;}

<SINGLE_LINE_COMMENT>"\n" {++line_number;BEGIN(INITIAL);}
<SINGLE_LINE_COMMENT>.      {}

<MULTI_LINE_COMMENT>"///" {BEGIN(INITIAL);}
<MULTI_LINE_COMMENT>"\n"    {++line_number;}
<MULTI_LINE_COMMENT>.      {}

%%
int getLineNumber(void){
    return line_number;
}
int isRunning(void){
    return running;
}

void initMe(void){
    init(g_hash_table,HASH_SIZE);
}
int yywrap(){
    running = 0;
    return 1;
}
#include "y.tab.c"
#include "main.c"