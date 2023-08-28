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
    int i = 1;

    if (argc < 2)
    {
        printf("call: ./etapa4 input.txt\n");
        exit(1);
    }
    if (0 == (yyin = fopen(argv[1], "r")))
    {
        printf("Cannot open file %s... \n", argv[1]);
        exit(2);
    }
    initMe();
    yyparse();
    fclose(yyout);
    
    fprintf(stderr,"compilation success\n");
    return 0;
}