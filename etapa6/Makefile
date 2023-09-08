# Makefile for single compiler call

etapa6: main.o lex.yy.o y.tab.o hash.o ast.o semantic.o types.o TAC.o asm.o stringList.o
	gcc main.o lex.yy.o y.tab.o hash.o ast.o semantic.o types.o TAC.o  asm.o  stringList.o -o etapa6
	gcc -c -Wall main.c

hash.o: hash.c
	gcc -c hash.c
ast.o: ast.c
	gcc -c ast.c
semantic.o: semantic.c
	gcc -c semantic.c
types.o: types.c
	gcc -c types.c
TAC.o: TAC.c
	gcc -c TAC.c
asm.o: asm.c
	gcc -c asm.c
stringList.o: stringList.c
	gcc -c stringList.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.c: parser.y
	yacc -d -v parser.y 
lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c y.output *.o etapa6 
