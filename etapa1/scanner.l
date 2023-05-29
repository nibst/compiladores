
%{

    #include "tokens.h"
    int running = 1;
    int line_number = 1;

%}

%x COMMENT

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

"//".* {}
"\\\\\\" {BEGIN(COMMENT);}
<COMMENT>"\n"    {++line_number;}
<COMMENT>"///" {BEGIN(INITIAL);}

[-,;()[\]{}=+*/%<>&|~] {return yytext[0];}
"<="    {return OPERATOR_LE;}
">="    {return OPERATOR_GE;}
"=="    {return OPERATOR_EQ;}
"!="    {return OPERATOR_DIF;}

\".*\"          {return LIT_STRING;}
\'.\'           {return LIT_CHAR;}
[a-zA-Z._]+     {return TK_IDENTIFIER;}
[0-9]+[.][0-9]+ {return LIT_REAL;}
[0-9]+          {return LIT_INT;}

[ \t]     {}

. {return TOKEN_ERROR;}

%%

#include "hash.h"
int getLineNumber(void){
    return line_number;
}
int isRunning(void){
    return running;
}

void initMe(void){
    Hash* g_hash_table[HASH_SIZE];
    init(g_hash_table,HASH_SIZE);
}

int main(int argc, char **argv)
{
    int token = 0;
    initMe();
    while (isRunning())
    {
        token = yylex();
        if (!isRunning())
            break;    
        printf("token: %d\n",token);
    }
}


int yywrap(){
    running = 0;
    return 1;
}