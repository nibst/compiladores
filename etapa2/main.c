//Níkolas Padão Schuster

#include <stdio.h>
#include <stdlib.h>

// lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
void initMe(void);

int main(int argc, char **argv)
{
    FILE *gold = 0;
    int token = 0;
    int answar = 0;
    int nota = 0;
    int i = 1;
    fprintf(stderr, "Rodando main do prof. \n");

    if (argc < 2)
    {
        printf("call: ./etapa2 input.txt \n");
        exit(1);
    }
    if (0 == (yyin = fopen(argv[1], "r")))
    {
        printf("Cannot open file %s... \n", argv[1]);
        exit(1);
    }
    initMe();
    yyparse();
    fprintf(stderr,"compilation success\n");
}