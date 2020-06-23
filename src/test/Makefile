lex : test.y test.l
	bison -d test.y
	flex test.l
	gcc -o $@ test.tab.c lex.yy.c -lfl
