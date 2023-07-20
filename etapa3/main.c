//Níkolas Padão Schuster

#include <stdio.h>
#include <stdlib.h>

// lex.yy.h
int yylex();
//y.tab.h
int yyparse();
extern char *yytext;
extern FILE *yyin,*yyout;

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

    if (argc < 3)
    {
        printf("call: ./etapa3 input.txt out.txt\n");
        exit(1);
    }
    if (0 == (yyin = fopen(argv[1], "r")))
    {
        printf("Cannot open file %s... \n", argv[1]);
        exit(1);
    }
    if (0 == (yyout = fopen(argv[2], "w")))
    {
        printf("Cannot open file %s... \n", argv[2]);
        exit(1);
    }
    initMe();
    yyparse();
    fclose(yyout);
    
    fprintf(stderr,"compilation success\n");
}