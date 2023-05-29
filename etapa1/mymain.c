#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
// lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
void initMe(void);

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
